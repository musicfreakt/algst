#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>

int main()
{
    HANDLE freeSem = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, "freeSem");
    HANDLE usedSem = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, "usedSem");
    HANDLE fileMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "fileMutex");

    std::srand(std::time(nullptr));

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    LONG page = -1;
    DWORD written = 0;
    std::string outputString = "";

    HANDLE mappingHandle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, "filemap");
    if (mappingHandle)
    {
        for (int i = 0; i < 3; ++i)
        {
            WaitForSingleObject(freeSem, INFINITE);
            outputString = std::to_string(GetTickCount()) + " | TAKE | FREE SEMAPHORE\n";
            WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);

            WaitForSingleObject(fileMutex, INFINITE);
            outputString = std::to_string(GetTickCount()) + " | TAKE | MUTEX\n";
            WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);

            Sleep(std::rand() % 1000 + 500);

            if (ReleaseMutex(fileMutex))
            {
                outputString = std::to_string(GetTickCount()) + " | FREE | MUTEX\n";
                WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);
            }
            else
            {
                std::string str = std::to_string(GetLastError()) + " CODE mutex\n";
                WriteFile(hStdout, str.data(), str.length(), &written, NULL);
            }

            if (ReleaseSemaphore(usedSem, 1, &page))
            {
                outputString = std::to_string(GetTickCount()) + " | FREE | USED SEMAPHORE\n";
                WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);
                std::string str = std::to_string(GetTickCount()) + " | PAGE | NUMBER = " + std::to_string(page + 1) + "\n\n";
                WriteFile(hStdout, str.data(), str.length(), &written, NULL);
            }
            else
            {
                std::string str = std::to_string(GetLastError()) + " CODE semaphore\n";
                WriteFile(hStdout, str.data(), str.length(), &written, NULL);
            }
        }
    }
    else
    {
        WriteFile(hStdout, "Mapping creation failed\n", strlen("Mapping creation failed\n"), &written, NULL);
    }

    CloseHandle(hStdout);

    return 0;
}
