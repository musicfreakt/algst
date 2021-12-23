#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
const std::string MUTEXNAME ="mutex_a";


int main()
{
    srand(time(nullptr) + GetCurrentProcessId()*197);
    HANDLE freeSem = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, (LPCTSTR)"freeSem");
    HANDLE usedSem = OpenSemaphore(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, (LPCTSTR)"usedSem");
    HANDLE mutex;
    DWORD waitResult;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    std::string outputString = "";

    HANDLE mapHandle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, "filemap");
    if (mapHandle)
    {
        for (int i = 0; i < 8; ++i)
        {
            LONG page = -1;

            WaitForSingleObject(usedSem, INFINITE);
            // ReleaseSemaphore(usedSem, -1, nullptr);
            // outputString = "time = " + std::to_string(GetTickCount()) + "; use semaphore\n";
            outputString = std::to_string(GetTickCount()) + "\n";
            WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);

            do {
                page += 1;
                std::string mutexName = MUTEXNAME + std::to_string(page);
                mutex = OpenMutexA(SYNCHRONIZE, false, mutexName.c_str());
                waitResult = WaitForSingleObject(mutex, 0);
            } while (waitResult == WAIT_TIMEOUT);

            // outputString = "time = " + std::to_string(GetTickCount()) + "; take mutex\n";
            // WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);

            Sleep(rand() % 1000 + 500); // fake read

            if (ReleaseMutex(mutex))
            {
                // outputString = "time = " + std::to_string(GetTickCount()) + "; free mutex\n";
                // WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);
            }
            else
            {
                std::string errorString = std::to_string(GetLastError()) + " code\n";
                WriteFile(hStdout, errorString.data(), errorString.length(), &written, NULL);
            }

            if (ReleaseSemaphore(freeSem, 1, nullptr))
            {
                // outputString = "time = " + std::to_string(GetTickCount()) + "; free semaphore\n";
                // WriteFile(hStdout, outputString.data(), outputString.length(), &written, NULL);
                // std::string str = "time = " + std::to_string(GetTickCount()) + "; page number = " + std::to_string(page + 1) + "\n\n";
                std::string str = std::to_string(GetTickCount()) + " " + std::to_string(page + 1) + "\n\n";

                WriteFile(hStdout, str.data(), str.length(), &written, NULL);
            }
            else
            {
                std::string errorString = std::to_string(GetLastError()) + " code\n";
                WriteFile(hStdout, errorString.data(), errorString.length(), &written, NULL);
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
