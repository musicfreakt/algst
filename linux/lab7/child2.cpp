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
    signal(SIGUSR2, signal_handler);
    signal(SIGQUIT, signal_handler);
    write_file_child(pipe, 2);

    return 0;
}

void signal_handler(int signal)
{
    if(signal == SIGQUIT)
        END = true;
    else
        READ = true;
}

void write_file_child(int pipe, int child)
{
    std::string sFileName = "child" + std::to_string(child) + ".txt";
    std::ofstream outFile(sFileName.c_str());
    //std::cout << child << ": ready. Pipe is " << pipe << "\n";
    int iMySignal = (child == 1 ? SIGUSR1 : SIGUSR2),
        iOtherSignal = (child == 1 ? SIGUSR2 : SIGUSR1);
    ssize_t iReadBytes;
    bool bEnd;
    char cBuffer;
    while(!bEnd)
    {
        //std::cout << child << ": wait for " << iMySignal << "\n";
        while(!READ)
            pause();
        READ = false;

        //std::cout << child << " got signal. Reading from pipe...\n";
        iReadBytes = read(pipe, &cBuffer, sizeof(cBuffer));
        if(iReadBytes > 0L)
        {
            //std::cout << child << " got (" << iReadBytes << "): " << cBuffer << std::endl;
            outFile << cBuffer;
        }
        else
        {
            //perror("Failed to read pipe");
            bEnd = END;
        }

        //std::cout << child << " finished reading from pipe, raise signal...\n";
        kill(0, iOtherSignal);
    }
    //std::cout << child << ": out\n";
    outFile.close();
}
