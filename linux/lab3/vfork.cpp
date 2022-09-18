#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>
#include <sstream>


void printAttributes(char* process_name, FILE* file)
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

    if (file = fopen(argv[1], "a"))
        printAttributes("vfork", file);

    return 0;
}