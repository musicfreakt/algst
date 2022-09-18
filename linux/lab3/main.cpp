#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>
#include <sstream>


void printAttributes(std::string process_name, FILE* file)
{
    std::stringstream out;
    pid_t pid = getpid();

    out << process_name << std::endl;
    out << "Process ID: " << pid << std::endl;
    out << "Parent Process ID: " << getppid() << std::endl;
    out << "Session ID: " << getsid(pid) << std::endl;
    out << "Process group ID: " << getpgid(pid) << std::endl;
    out << "User ID: " << getuid() << std::endl;
    out << "Effective user ID: " << geteuid() << std::endl;
    out << "Group ID: " << getgid() << std::endl;
    out << "Effective group ID: " << getegid() << std::endl;

    fprintf(file, out.str().c_str());
}


int main(int argc, char* argv[])
{
    if(argc == 5)
    {
        int parent_delay, fork_delay, vfork_delay;

        parent_delay = atoi(argv[1]);
        fork_delay = atoi(argv[2]);
        vfork_delay = atoi(argv[3]);

        FILE* file = fopen(argv[4], "w");

        fprintf(file, "parent delay: %d; fork delay: %d; vfork delay: %d\n", parent_delay, fork_delay, vfork_delay);
        fclose(file);

        if((file = fopen(argv[4], "a")))
        { 

            pid_t process_fork = fork();
            if(process_fork == 0)
            {
                sleep(fork_delay);
                printAttributes("fork", file);
                return 0;
            }
            if(process_fork < 0) 
                std::cout << "fork process was not created" << std::endl;

            pid_t process_vfork = vfork();
            if(process_vfork == 0)
                execl("vfork", "vfork", argv[3], argv[4], NULL);
            if(process_vfork < 0) 
                std::cout << "vfork process was not created" << std::endl;
            
            sleep(parent_delay);
            printAttributes("Parent", file);
            fclose(file);
        }
        else 
            std::cout << "Error opening file " <<  argv[4] << std::endl;
    }
    else
    {
        std::cout << "Incorrect number of arguments!\n" << std::endl;
        std::cout << "Example: program parent_delay fork_delay vfork_delay file_name" << std::endl;
    }
    
    return 0;
}
