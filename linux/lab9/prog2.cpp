#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

const int NUM_PROCESS = 3;
const int NUM = 2;

struct shared 
{
    int waiting_ticket[NUM_PROCESS]; // какой процесс ждет билет
    int values[NUM_PROCESS]; // какой номер билета получил процесс
};

shared* shared_var;

// Алгоритм Лампорта (булочной)
void lock(shared* shared_var, int process) 
{
    shared_var->waiting_ticket[process] = 1;

    shared_var->values[process] = 1 + std::max(shared_var->values[0], std::max(shared_var->values[1], shared_var->values[2]));
    
    shared_var->waiting_ticket[process] = 0;

    for (int i = 0; i < NUM_PROCESS; ++i) 
    {
        if (i != process)
        {
            // Если процесс еще ждет билет, то ждем: 
            while (shared_var->waiting_ticket[i]); 

            // Ждём, пока все потоки с меньшим номером или с таким же номером, но с более высоким приоритетом, закончат свою работу:
            while (shared_var->values[i] > 0 // Если значение было проинициализировано
                && (shared_var->values[i] < shared_var->values[process])  // И значение билетика было раньше
                || (shared_var->values[process] == shared_var->values[i] && i < process)); // Или если получили в одно время, то по номеру
        }
    }
    
}

void unlock(shared* shared_var, int process) 
{
    shared_var->values[process] = 0;
}

void signal_handler(int sig)
{
    if (sig == SIGALRM) 
    {
        std::cout << "Program "<< NUM << " waiting for shared variable\n";
        lock(shared_var, NUM);

        FILE *file = fopen("output.txt", "a");
        time_t curTime = time(NULL);
        fprintf(file, "Program %d PID: %d, Time: %s", NUM, getpid(), ctime(&curTime));
        fclose(file);

        unlock(shared_var, NUM);
        std::cout << "Program "<< NUM << " free shared variable\n";
    }
}

int main(int argc, char** argv)
{
    if(argc == 3)
    {
        int starts = atoi(argv[1]);
        int period = atoi(argv[2]);
        signal(SIGALRM, signal_handler);
        
        struct itimerval timer_value;
        timerclear(&timer_value.it_interval);
        timerclear(&timer_value.it_value);
        timer_value.it_interval.tv_sec = period;
        timer_value.it_value.tv_sec = period;
        setitimer(ITIMER_REAL, &timer_value, NULL);

        // создание разделяемого сегмента памяти
        // 123 - ключ
        // sizeof(shared) - размер сегмента
        // 0666 - чтение и запись вообще всем
        int shm_id = shmget(123, sizeof(shared), (0666 | IPC_CREAT));
        if(shm_id != -1)
             std::cout <<  "Program " << NUM << " get id = " << shm_id << " shared segment\n";
        else
            return -1;
        
        // получение виртуального адрес, по которому сегмент был привязан к процессу
        // shm_id - идентификатор сегмента
        // 0 или адрес, по которому присоединить
        // 0 - флаги
        void* shm_addr = shmat(shm_id, 0, 0);
        if(*(int*)shm_addr != -1)
            std::cout << "Program " << NUM << " connect shared segment\n\n";
        else
            return -1;

        // получение данных из разделяемой памяти
        shared_var = (shared*)shm_addr;

        for(int i = 0; i < starts; i++)
        {
            pause();
        }

        // Отсоединение сегмента
        if(shmdt(shm_addr) != -1)
            std::cout << "\nProgram " << NUM << " disconnect shared segment\n";
        
    }
    else
        std::cout << "Error: " << argv[0] << " starts period\n";

    return 0;
}