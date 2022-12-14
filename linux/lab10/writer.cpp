#include <iostream>
#include <fstream>
#include <sys/sem.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // 0 - индекс
    // 1 - операция (положительное + , отрицательное число - или нуль (получение))
    // 2 - флаги
    struct sembuf operation;

    int number = atoi(argv[1]);
    int sleept = atoi(argv[2]);

    // 5051 - key
    // 4 - количество семафоров (0 - занятость файла, 1 - программы, 2 - писатели, 3 - читатели)
    // IPC_CREAT | IPC_EXCL | 0666 - флаги
    int semaphore = semget(5051, 4, IPC_CREAT | IPC_EXCL | 0666);

    if (semaphore != -1)
    {   
        operation = {0, 1, 0}; // файл доступен
        
        // semaphore – идентификатор семафора;
        // operation – массив операций;
        // 1 – число операций
        semop(semaphore, &operation, 1);
    }
    else
        semaphore = semget(5051, 4, IPC_CREAT | 0666);

    std::ofstream file("out.txt", std::ios::app);

    // программы увеличиваем
    operation = {1, 1, 0}; 
    semop(semaphore, &operation, 1);

    
    int pid = getpid();
    std::cout << "PID:" << pid << std::endl;

    for (int i = 0; i < number; i++)
    {   
        // увеличиваем писателей
        operation = {2, 1, 0}; 
        semop(semaphore, &operation, 1);

        // проверка читателей
        operation = {3, 0, 0}; 
        semop(semaphore, &operation, 1);

        // занимаем файл
        operation = {0, -1, 0}; 
        semop(semaphore, &operation, 1);
        
        // пишем в файл
        file << "Writer " << pid << "; String num " << i << std::endl;
        std::cout << "Writer " << pid << "; String num " << i << std::endl;
        
        // открываем файл
        operation = {0, 1, 0}; 
        semop(semaphore, &operation, 1);

        // уменьшаем писателей
        operation = {2, -1, 0}; 
        semop(semaphore, &operation, 1);

        // ждем....
        sleep(sleept);
    }

    // уменьшаем программы
    operation = {1, -1, 0}; 
    semop(semaphore, &operation, 1);
    
    if (semctl(semaphore, 3, GETVAL, 0) == 0)
    {   
        semctl(semaphore, IPC_RMID, 0);
    }
        
    file.close();
    return 0;
}