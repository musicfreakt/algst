#include <iostream>
#include <fstream>
#include <sys/sem.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    struct sembuf operation;
    std::string line;

    int semaphore = semget(5051, 4, IPC_CREAT | IPC_EXCL | 0666);

    if (semaphore != -1) 
    {
        operation = {0, 1, 0};
        semop(semaphore, &operation, 1);
    } 
    else
        semaphore = semget(5051, 4, IPC_CREAT | 0666);

    std::ifstream file("out.txt");

    // увеличение количества запущенных программ
    operation = {1, 1, 0};
    semop(semaphore, &operation, 1);

    // проверка занятости писателей
    operation = {2, 0, 0}; 
    semop(semaphore, &operation, 1);

    // увеличение читателей
    operation = {3, 1, 0}; 
    semop(semaphore, &operation, 1);

    while (!file.eof()) 
    {
        getline(file, line);
        std::cout << line << std::endl;
    }

    // уменьшаем читателей
    operation = {3, -1, 0}; 
    semop(semaphore, &operation, 1);

    // уменьшаем программы
    operation = {1, -1, 0}; 
    semop(semaphore, &operation, 1);

    if (semctl(semaphore, 1, GETVAL, 0) == 0)
    {   
        semctl(semaphore, IPC_RMID, 0);
    }
    file.close();
    return 0;
}