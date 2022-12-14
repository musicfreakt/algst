#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define SEQUENCE_LEN 10
#define MSG_LEN 1024

int sock, attached_socket;
int reply[MSG_LEN];
int timeout;

int cmp(const void *a, const void *b);
void service_process();

int main(int argc , char *argv[])
{
    struct sockaddr_in server, client;
    int rv;
    struct timeval tv;

    if(argc < 2)
    {
        std::cout << "Incorrect number of arguments!\n" << std::endl;
        std::cout << "Example: " << argv[0] << " delay \n";
        return 1;
    }

    timeout = atoi(argv[1]);

    tv.tv_sec = timeout; // установка timeout
    tv.tv_usec = 0;

    // создание сокета
    // AF_INET - домен для взаимодействия через сеть по протоколу TCP/IP
    // SOCK_STREAM - тип сокета потоковый
    // 0 - автоматический выбор протокола (для потокового IPPROTO_TCP)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "Error socket creation\n";
    }
    std::cout << "PID " << getpid() << " create socket\n";


    server.sin_family = AF_INET; // тип домена
    server.sin_addr.s_addr = INADDR_ANY; // адрес (0.0.0.0)
    server.sin_port = htons(8888); // порт
 
    // привязка сокета к адресу и номеру порта
    // sock - дескриптор сокета
    // server - cтруктура
    if(bind(sock,(struct sockaddr *) &server , sizeof(server)) < 0)
    {
        std::cout << "can't bind\n";
        return -1;
    }

    std::cout << "A binding to a network address has occurred\n";
    
    // перевод сокета в пассивное ожидание (создаем очередь)
    // sock - дескриптор сокета сервера
    // 5 - длина очереди
    listen(sock, 5);
    
    std::cout << "The server is waiting for clients to connect...\n";

    fd_set readfds; // набор дескрипторов на чтение
    FD_ZERO(&readfds); // очистка набора на чтение
    FD_SET(sock, &readfds); // добавление дескриптора в набор

    while(1)
    {
        // определение состояния сокета
        // sock+1 - кол-во опрашиваемых дескрипторов сокетов
        // readfds набор дескрипторов, которые следует проверять на готовность к чтению
        // tv - timeout
        rv = select(sock+1, &readfds, NULL, NULL, &tv);

        if(rv > 0)
        {   
            // создание нового присоединенного сокета
            // sock - дескриптор сокета сервера
            // client - содержит указатель на структуру с адресом сервера
            attached_socket = accept(sock, (struct sockaddr *)&client, (socklen_t*)&client);
            if(attached_socket > 0){
                std::cout << "Client successfully connected\n";
                service_process();    
            }
        }
        else
        {
            std::cout << "Timeout " << timeout << " seconds\n";
            break;
        }   
        tv.tv_sec = timeout;
    }
    
    // закрытие принимающего сокета
    close(sock);
    return 0;
}

void service_process()
{   
    // порождение обслуживающего потока
    if (fork() == 0)
    {
        printf("Maintenance process PID = %d\n", getpid());

        // закрытие принимающего сокета
        close(sock);
        
        // ожидание получения информации. чтение информации
        if(recv(attached_socket, reply, MSG_LEN, 0) > 0)
        {
            // обработка информации
            printf("Server accepted data\n");
            qsort(reply, SEQUENCE_LEN, sizeof(int), cmp);
            printf("The server sorted and sent the data\n");
        }

        // отправка ответа
        send(attached_socket, reply, SEQUENCE_LEN*4, 0);

        // завершение соединения
        close(attached_socket);

        // окончание работы
        exit(EXIT_SUCCESS);
    }
}

int cmp(const void *a, const void *b) 
{
     return *(int*)a - *(int*)b;
}
