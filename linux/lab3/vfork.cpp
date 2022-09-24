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
    FILE *file;

    if (file = fopen(argv[2], "a"))
    {
        sleep(atoi(argv[1]));
        printAttributes("vfork", file);
    }

    fclose(file);

    return 0;
}
