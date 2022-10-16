#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <iostream>

void signal_handler(int sig)
{
    if (sig == SIGALRM)
        if (fork() == 0) 
        {
            time_t start_time = time(NULL);
            std::cout << "\nPID: " << getpid() << std::endl;
            std::cout << "Start datetime: " << ctime(&start_time);
            exit(EXIT_SUCCESS);
        }
}

int main(int argc, char** argv)
{
    if(argc == 3)
    {
        int starts = atoi(argv[1]);
        int period = atoi(argv[2]);

        signal(SIGTSTP, SIG_IGN);
        signal(SIGALRM, signal_handler);
        
        struct itimerval timer_value;
        timerclear(&timer_value.it_interval);
        timerclear(&timer_value.it_value);
        timer_value.it_interval.tv_sec = period;
        timer_value.it_value.tv_sec = period;
        setitimer(ITIMER_REAL, &timer_value, NULL);

        time_t pt1, pt2; 
        clock_t pt3, pt4; 

        pt1 = time(NULL);
        for(int i = 0; i < starts; i++)
        {
            pause();

            pt3 = clock();
            wait(nullptr);
            pt4 = clock();

            std::cout << "Descendant: " << i;
            std::cout << "; Time: " << pt4-pt3 << " ticks (" << (pt4-pt3)/CLOCKS_PER_SEC << " seconds)" << std::endl;
        }
        pt2 = time(NULL);

        std::cout << "\nParent PID: " << getpid() << std::endl;
        std::cout << "Time: " << pt2-pt1 << " seconds" << std::endl;
    }
    else
    {
	    std::cout << "Incorrect number of arguments!" << std::endl;
        std::cout << "Example: " << argv[0] << " starts period" << std::endl;
    }
    
    return 0;
}
