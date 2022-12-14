#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define SEQUENCE_LEN 10
#define MSG_LEN 1024

int *arr;
int timeout;

void generate_sequence();
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    int server_reply[MSG_LEN];
    int rv;
    struct timeval tv;

    if(argc < 2)
    {
        std::cout << "Incorrect number of arguments!\n" << std::endl;
        std::cout << "Example: " << argv[0] << " delay \n";
        return 1;
    }

    timeout = atoi(argv[1]);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    srand(time(NULL));
    
    // создание сокета
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        std::cout << "Error creating socket\n";
        return -1;
    }
        
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    rv = select(sock+1, &readfds, NULL, NULL, &tv);
    
    if(rv > 0)
    {   
        int start_time = time(NULL);
        int duration = 0;
        do
        {
            if(connect(sock, (struct sockaddr *)&server, sizeof(server)) >= 0)
            {
                std::cout << "Joined the server\n";
                generate_sequence();   

                // посылка данных
                if(send(sock, arr, SEQUENCE_LEN*4 , 0) < 0)
                {
                    std::cout << "Failed\n";
                    exit(EXIT_FAILURE);
                }
                std::cout << "Sent data\n"; 
                
                // получение ответа от сервера
                if(recv(sock, server_reply , MSG_LEN , 0) < 0)
                {
                    std::cout << "Failed\n";
                    exit(EXIT_FAILURE);
                }
                std::cout << "Server-sorted sequence: ";

                for(int i = 0; i < SEQUENCE_LEN; i++)
                std::cout << server_reply[i] << " ";
        
                std::cout << std::endl;
                break;
            }
            else
                duration = time(NULL) - start_time;
        }
        while(duration < timeout);

        if(duration >= timeout)
            std::cout << "Timeout " << timeout << " seconds" << std::endl;
    }

    return 0;
}

void generate_sequence()
{
    arr = (int*)malloc(SEQUENCE_LEN);

    std::cout << "Generated sequence: ";
    for(int i = 0; i < SEQUENCE_LEN; i++)
    {
        arr[i] = rand() % 21;
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;
}