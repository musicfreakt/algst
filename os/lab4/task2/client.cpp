#include <windows.h>
#include <iostream>

const size_t BUFFER_SIZE = 1024;
const std::string PIPE_NAME("\\\\.\\pipe\\lab4");
const char* EXIT_STR = ":q";

size_t callback;
void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    ++callback;
}

int main()
{
    WaitNamedPipeA(PIPE_NAME.c_str(), NMPWAIT_WAIT_FOREVER);

    HANDLE hPipe = CreateFileA(PIPE_NAME.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

    if(hPipe != INVALID_HANDLE_VALUE)
    {
        OVERLAPPED over;
        size_t offset_i = 0;
		over.Offset = offset_i;
		over.OffsetHigh = offset_i >> 31;

        char buffer[BUFFER_SIZE];
		buffer[0] = '\0';

        while(strcmp(buffer, EXIT_STR) != 0)
        {
            callback = 0;

            ZeroMemory(buffer, BUFFER_SIZE);

            ReadFileEx(hPipe, buffer, BUFFER_SIZE, &over, FileIOCompletionRoutine);
            SleepEx(-1, TRUE);

            std::cout << "Message: \"" << buffer << "\". " <<  std::endl;
        }
        CloseHandle(hPipe);
    }
    else
    {
        std::cout << "Error: Can't connect to the pipe." << std::endl;
        return GetLastError();
    }

    return 0;
}
