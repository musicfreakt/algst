#include <iostream>
#include <fstream>
#include <sys/msg.h>
#define PID 1
#define FNAME "file.txt"

// Общая очередь - для запросов
// структура сообщения
struct ask_msg
{
    long mtype; // какой программе посылается
    int sender; // какая программа послала
    int answer_queue; // очередь для ответа
    int ask_time; // время запроса
};

// Локальная очередь - для разрешений
// структура сообщения
struct answer_msg
{
    long mtype; // кому отправляли доступ
    int sender; // кто отправил доступ
};

int main()
{
    bool is_creator = false;
    int proc_end = 0;

    int local_queue;
    int global_queue;

    ask_msg ask;
    ask_msg buffer[2];
    int buf_i = 0;

    answer_msg answer; 
    answer_msg access;

    int num_get_access = 0; 
    int num_send_access = 0; 
    
    // Создаем глобальную очередь
    global_queue = msgget(777, 0606|IPC_CREAT|IPC_EXCL);

    if(global_queue != -1)
    {
        is_creator = true;
        std::cout<<"Создана общая очередь"<<std::endl;
    }
    else
    {
        global_queue = msgget(777, 0606|IPC_CREAT);
        std::cout<<"Подключен к общей очереди"<<std::endl;
    }
    
    // создаем локальную очередь
    local_queue = msgget(IPC_PRIVATE, 0606|IPC_CREAT);
    std::cout << "Создана локальная очередь" << std::endl;
    
    // ФОРМИРУЕМ ЗАПРОСЫ И ОТПРАВЛЯЕМ В ОБЩУЮ ОЧЕРЕДЬ
    ask.ask_time = time(NULL);
    ask.mtype = (PID)%3 + 1;
    ask.sender = PID;
    ask.answer_queue = local_queue;
    msgsnd(global_queue, &ask, sizeof(ask_msg), 0);

    ask.mtype = (PID + 1)%3 + 1;
    msgsnd(global_queue, &ask, sizeof(ask_msg), 0);
    
    // ФОРМИРУЕМ ОТВЕТЫ
    answer.mtype = 1;
    answer.sender = PID;
    
    // ПОКА НЕ ПОЛУЧИЛИ ДВА РАЗРЕШЕНИЯ МЫ ТУТ
    while(num_get_access < 2)
    {   
        // проверка запросов из общей очереди для данной программы
        // если получили, то заносим в буфер 

        // global_queue – идентификатор очереди сообщений;
        // buffer – адрес буфера сообщения;
        // sizeof(ask_msg) – максимальный размер сообщения;
        // PID – тип сообщения > 0 - первое указанного типа
        // 0 – флаги операции
        if(msgrcv(global_queue, &buffer[buf_i], sizeof(ask_msg), PID, 0) != -1)
        {
            std::cout << "Получен запрос доступа от " << buffer[buf_i].sender << std::endl;
            std::cout << "Время запроса: " << buffer[buf_i].ask_time << std::endl;
            
            // ВТОРОЕ УСЛОВИЕ ПРИ ПОЛУЧЕНИИ ЗАПРОСА:
            /* если файл не читался, то ответ передается только при условии, что
            время посылки запроса в сообщении меньше, чем время запроса на
            чтение у данной программы */

            if(buffer[buf_i].ask_time < ask.ask_time || (buffer[buf_i].ask_time == ask.ask_time && buffer[buf_i].sender < PID))
            {
                // выдача разрешения
                msgsnd(buffer[buf_i].answer_queue, &answer, sizeof(answer_msg), 0);
                // запоминаем сколько разрешений выдали
                ++num_send_access;
                std::cout<<"Выдача разрешения процессу " << buffer[buf_i].sender << std::endl;
            }
            else 
                // ЕСЛИ НЕ ПОДОШЛИ ПО УСЛОВИЮ ИНДЕКС В БУФЕРЕ СДВИГАЕМ
                ++buf_i;
        }

        // проверка разрешений из локальной очереди
        if(msgrcv(local_queue, &access, sizeof(answer_msg), 1, 0) != -1)
        {   
            ++num_get_access;
            std::cout << "Получено разрешение на доступ от " << access.sender << std::endl;
            std::cout << "Время получения: " << time(NULL) << std::endl;
        }
    }
    
    // ВЫВОДИМ ФАЙЛ
    std::cout << "Начать вывод файла" << std::endl;
    std::ifstream fin(FNAME);
    std::string str;
    while(std::getline(fin,str))
        std::cout << str << std::endl;
    fin.close();
    std::cout << "Конец файла. Файл закрыт." << std::endl;
    
    // обслуживаем те, на которые ответы не передали...

    // РАБОТАЕТ УСЛОВИЕ 1:
    /* если программа прочитала файл, то сразу передается ответ, он должен
    содержать номер отвечающей программы и время ответа */

    // те, которые не подошли по условию времени
    while(buf_i > 0)
    {
        --buf_i;
        msgsnd(buffer[buf_i].answer_queue, &answer, sizeof(answer_msg), 0);
        ++num_send_access;
        std::cout<<"Выдача разрешения процессу " << buffer[buf_i].sender << std::endl;
    }
    
    // если еще остались необслуженные
    while(num_send_access < 2)
    {
        // считываем из общей очереди
        if(msgrcv(global_queue, &buffer[buf_i], sizeof(ask_msg), PID, 0) != -1)
        {   
            // выдаем разрешение
            msgsnd(buffer[buf_i].answer_queue, &answer,sizeof(answer_msg), 0);
            ++num_send_access;
            std::cout<<"Выдача разрешения процессу " << buffer[buf_i].sender <<std::endl;
        }
    }
    
    // завершили
    ask.mtype = 0;
    msgsnd(global_queue, &ask, sizeof(ask_msg), 0);
    
    // ожидание остальных процессов для закрытия глобальной очереди
    if(is_creator)
    {
        while(proc_end < 3)
        {
            if(msgrcv(global_queue,&ask,sizeof(ask_msg),0,0)!=-1)
                ++proc_end;
        }
        msgctl(global_queue, IPC_RMID, 0);
    }
    
    // закрытые локальной очереди
    msgctl(local_queue, IPC_RMID, 0);
    
    return 0;
}