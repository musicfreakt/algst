#include <iostream>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>

const int PAGESIZE = 4096;
const int PAGECOUNT = 17;
const int N = PAGECOUNT - 1;
const int WRITERCOUNT = 10;
const int READERCOUNT = 10;
const std::string MUTEXNAME ="mutex_a";

int main()
{
    srand(time(nullptr));
    int fileSize = PAGECOUNT * PAGESIZE;
    HANDLE freeSem = CreateSemaphore(NULL, N, N, "freeSem");
    HANDLE usedSem = CreateSemaphore(NULL, 0, N, "usedSem");
    std::vector<HANDLE> pageMutex;

    for(int i = 0; i < PAGECOUNT; i++)
    {
        std::string mutexName = MUTEXNAME + std::to_string(i);
        pageMutex.push_back(CreateMutexA(nullptr, false, mutexName.c_str()));
    }

    HANDLE fHandle = CreateFileA("file.txt", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    SetFilePointer(fHandle, fileSize, 0, FILE_BEGIN);
    SetEndOfFile(fHandle);

    HANDLE mapHandle = CreateFileMapping(fHandle, NULL, PAGE_READWRITE, 0, 0, (LPCTSTR)"filemap");

    LPVOID mapView = MapViewOfFile(mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);

    VirtualLock(mapView, fileSize);

    HANDLE processHandles[WRITERCOUNT+READERCOUNT];

    for (int i = 0; i < WRITERCOUNT; ++i)
    {
        std::string logName = "writelog_" + std::to_string(i + 1) + ".txt";

        STARTUPINFO sysInfo;
        PROCESS_INFORMATION procInfo;
        SECURITY_ATTRIBUTES secureAttr = { sizeof(secureAttr), NULL, TRUE };

        ZeroMemory(&sysInfo, sizeof(sysInfo));

        HANDLE outHandle = CreateFile((LPCTSTR)logName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, &secureAttr,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        sysInfo.cb = sizeof(sysInfo);
        sysInfo.hStdOutput = outHandle;
        sysInfo.hStdError = NULL;
        sysInfo.hStdInput = NULL;
        sysInfo.dwFlags |= STARTF_USESTDHANDLES;

        ZeroMemory(&procInfo, sizeof(procInfo));

        int mainProcess = CreateProcess((LPCTSTR)"writer.exe",
            NULL,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &sysInfo,
            &procInfo);

        if (mainProcess)
            processHandles[i] = procInfo.hProcess;
        // Sleep(1);
    }

    for (int i = 0; i < READERCOUNT; ++i)
    {
        std::string fname = "readlog_" + std::to_string(i + 1) + ".txt";

        STARTUPINFO sysInfo;
        PROCESS_INFORMATION procInfo;
        SECURITY_ATTRIBUTES secureAttr = { sizeof(secureAttr), NULL, TRUE };

        ZeroMemory(&sysInfo, sizeof(sysInfo));

        sysInfo.hStdOutput = CreateFile((LPCTSTR)fname.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, &secureAttr,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        sysInfo.cb = sizeof(sysInfo);
        sysInfo.hStdError = NULL;
        sysInfo.hStdInput = NULL;
        sysInfo.dwFlags |= STARTF_USESTDHANDLES;
        ZeroMemory(&procInfo, sizeof(procInfo));

        int createProcess = CreateProcess((LPCTSTR)"reader.exe",
            NULL,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &sysInfo,
            &procInfo);

        if (createProcess != 0)
            processHandles[WRITERCOUNT + i] = procInfo.hProcess;

    }

    WaitForMultipleObjects(WRITERCOUNT+READERCOUNT, processHandles, true, INFINITE);

    for (int i = 0; i < WRITERCOUNT+READERCOUNT; ++i)
        CloseHandle(processHandles[i]);

    CloseHandle(mapHandle);
    UnmapViewOfFile(mapView);
    CloseHandle(fHandle);
    for(int i = 0; i < PAGECOUNT; i++)
        CloseHandle(pageMutex[i]);
    CloseHandle(freeSem);
    CloseHandle(usedSem);

    // std::cout << GetTickCount()<<e
    std::cout << std::endl << "The work is done." << std::endl;
    system("pause");
}
