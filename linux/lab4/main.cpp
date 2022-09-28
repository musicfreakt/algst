#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>

#include <iostream>
#include <fstream>
#include <string>


typedef struct _thread_args
{
    std::fstream *file;
    const char *str;
} thread_args;


void *thread_routine(void *args)
{
    thread_args *arg = (thread_args*)args;

    std::fstream &file_out = *(arg->file);
    file_out << arg->str << std::endl;

    return NULL;
}


int main(int argc, char **argv)
{

    std::fstream input_file;
    std::fstream output1_file;
    std::fstream output2_file;
    
    if(argc == 2)
        input_file.open(argv[1], std::ios::in);
    else
        input_file.open("input", std::ios::in);

    output1_file.open("output1", std::ios::out | std::ios::trunc);
    output2_file.open("output2", std::ios::out | std::ios::trunc);

    thread_args thr_arg_1,
                thr_arg_2;
                
    thr_arg_1.file = &output1_file;
    thr_arg_2.file = &output2_file;

    bool end_of_file = false;
    std::string buffer1, buffer2;
    pthread_t thread_1 = 0, thread_2 = 0;

    if(input_file && output1_file && output2_file)
    {
        std::cout << "Start of input file\n";

        while(!end_of_file)
        {   

            if(getline(input_file, buffer1))
            {
                thr_arg_1.str = buffer1.c_str();
                if(pthread_create(&thread_1, NULL, &thread_routine, &thr_arg_1))
                {
                   std::cout << "Thread_1 ERROR\n";
                   return -1;
                }
            }
            else
            {
                end_of_file = true;
                continue;
            }

            if(getline(input_file, buffer2))
            {
                thr_arg_2.str = buffer2.c_str();
                if(pthread_create(&thread_2, NULL, &thread_routine, &thr_arg_2))
                {
                    std::cout << "Thread_2 ERROR\n";
                    return -1;
                }
            }
            else
                end_of_file = true;

            pthread_join(thread_1, NULL);
            pthread_join(thread_2, NULL);
        }

        std::cout << "End of input file. Bye\n";
        input_file.close();
        output1_file.close();
        output2_file.close();
    }
    else
        std::cout << "ERROR: cant open file\n";
    return 0;
}