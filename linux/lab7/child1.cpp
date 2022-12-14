#include <iostream>
#include <fstream>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void write_file_child(int pipe, int child);
void signal_handler(int signal);

bool END, READ;

int main(int argc, char* argv[])
{
    if(argc < 2)
        return 1;

    int pipe = atoi(argv[1]);

    // устанавливаем соответствие сигналов и обработчиков
    signal(SIGUSR1, signal_handler);
    signal(SIGQUIT, signal_handler);

    // пишем в файл
    write_file_child(pipe, 1);

    return 0;
}

void signal_handler(int sig)
{
    if(sig == SIGQUIT)
        END = true;
    else
        READ = true;
}

void write_file_child(int pipe, int child)
{
    std::string file_name = "child" + std::to_string(child) + ".txt";
    std::ofstream out(file_name.c_str());
    
    int my_signal = (child == 1 ? SIGUSR1 : SIGUSR2),
        other_signal = (child == 1 ? SIGUSR2 : SIGUSR1);

    ssize_t bytes;
    bool bEnd;
    char buffer;
    
    while(!bEnd)
    {
        // потомок ждет начала чтения по сигналу
        while(!READ)
            pause();
        READ = false;

        // получили сигнал и читаем из канала
        bytes = read(pipe, &buffer, sizeof(buffer));

        // если биты еще есть, то выводим
        if(bytes > 0L)
            out << buffer;
        else
            // завершаем, если флаг активен
            bEnd = END;

        // прочитали символ, посылаем сингал второму потомку на чтение канала
        kill(0, other_signal);
    }
    
    out.close();
}
