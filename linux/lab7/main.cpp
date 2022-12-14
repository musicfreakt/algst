#include <iostream>
#include <fstream>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void file_to_pipe(std::ifstream& file, int pipe, int first_child);

int main(int argc, char* argv[])
{   
    // игнорируем сигналы синхронизации потомков
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    if(argc < 2)
    {
        std::cout << "You forgot param: ./lab7 {file to read}" << std::endl;
        return 0;
    }

    char* filename = argv[1];

    int pipes[2]; 
    // дескрипторы файлов канала
    // 0 - read, 1 - write

    // открытие каналов
    int pipe_status = pipe(pipes);
    if(pipe_status == -1)
    {
        perror("Cannot create pipe");
        return errno;
    }

    pid_t child1 = vfork();
    if(child1 == -1)
    {
        perror("Failed to create child1");
        return errno;
    }
    else if(child1 == 0)
    {
        std::string pipe_str = std::to_string(pipes[0]);
        execl("child1", "child1", pipe_str.c_str(), NULL);
        return 0;
    }


    pid_t child2 = vfork();
    if(child2 == -1)
    {
        perror("Failed to create child2");
        return errno;
    }
    else if(child2 == 0)
    {
        std::string pipe_str = std::to_string(pipes[0]);
        execl("child2", "child2", pipe_str.c_str(), NULL);
        return 0;
    }

    std::ifstream file(filename);

    // пишем в канал
    file_to_pipe(file, pipes[1], child1);

    // посылаем сигналы завершения
    kill(child1, SIGQUIT);
    kill(child2, SIGQUIT);

    // ожидаем завершения потомков
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    // закрываем каналы
    file.close();
    close(pipes[0]);
    close(pipes[1]);

    return 0;
}

void file_to_pipe(std::ifstream& file, int pipe, int first_child)
{
    sleep(1);
    bool first_signal;
    if(!file.is_open())
    {
        perror("Failed to read file");
        exit(errno);
    }

    std::string buff;
    while(getline(file, buff))
    {
        buff += "\n";
        // std::cout << "write to pipe: " << buff << std::endl;

        // записываем строку в канал
        write(pipe, buff.c_str(), buff.size());
        if(!first_signal)
        {   
            first_signal = true;
            // посылаем сигнал первому потомку на чтение канала
            kill(first_child, SIGUSR1); 
        }
    }
    if(!first_signal)
        // посылаем сигнал первому потомку на чтение канала
        kill(first_child, SIGUSR1);
}
