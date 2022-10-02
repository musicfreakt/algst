#include <iostream>
#include <signal.h>

void signal_handler(int sig)
{
    switch (sig)
    {
        case SIGFPE:
            std::cout << "SIGFPE signal; return 1" << std::endl;
            exit(1);
        break;
        case SIGSEGV:
            std::cout << "SIGSEGV signal; return 2" << std::endl;
            exit(2);
        break;
    }
}

int main(int argc, char* argv[])
{
    int error_type,
        num = 1917;
    char *str = "test";

	// установка обработчиков сигналов
    signal(SIGFPE, signal_handler);
    signal(SIGSEGV, signal_handler);

    if(argc == 2)
    {
        error_type = atoi(argv[1]);

        if(error_type == 1)
            num /= 0; // деление на ноль )
        else if(error_type == 2)
            str[num] = 'y'; // запись в раздел памяти только для чтения
		else
			std::cout << "Illigal agrument! error_type should be 1 or 2" << std::endl;
    }
    else
	{
	    std::cout << "Incorrect number of arguments!" << std::endl;
        std::cout << "Example: " << argv[0] << " error_type" << std::endl;
	}

    return 0;
}
