#include <iostream>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>

const int PAGESIZE = 4096;
const int COUNT = 17;
const int N = COUNT - 1;
const int WRITERCOUNT = COUNT / 2;
const int READERCOUNT = COUNT / 2;

int main()
{
    srand(time(nullptr));
    int fileSize = N * PAGESIZE;
    HANDLE mapHandle;
    HANDLE freeSem = CreateSemaphore(NULL, N, N, "freeSem");
    HANDLE usedSem = CreateSemaphore(NULL, 0, N, "usedSem");
    std::vector<HANDLE> pageMutex;
    std::string mutexName = "mutex_a";

    for(int i = 0; i < COUNT; i++)
    {
        mutexName.back() = '0' + i;
        pageMutex.push_back(CreateMutexA(nullptr, false, &mutexName[0]));
    }

    HANDLE fHandle = CreateFileA("file.txt", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    SetFilePointer(fHandle, fileSize, 0, FILE_BEGIN);
    SetEndOfFile(fHandle);

    mapHandle = CreateFileMapping(fHandle, NULL, PAGE_READWRITE, 0, 0, (LPCTSTR)"filemap");

    LPVOID mapView = MapViewOfFile(mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);

    VirtualLock(mapView, fileSize);

    HANDLE processHandles[COUNT];

    for (int i = 0; i < WRITERCOUNT; ++i)
    {
        std::string logName = "writelog_" + std::to_string(i + 1) + ".txt";

        STARTUPINFO sysInfo;
        PROCESS_INFORMATION procInfo;
        SECURITY_ATTRIBUTES secureAttr = { sizeof(secureAttr), NULL, TRUE };

        ZeroMemory(&sysInfo, sizeof(sysInfo));

        HANDLE outHandle = CreateFile((LPCTSTR)logName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, &secureAttr,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

        // Sleep(1);
    }

    WaitForMultipleObjects(COUNT, processHandles, true, INFINITE);

    for (int i = 0; i < COUNT; ++i)
        CloseHandle(processHandles[i]);

    CloseHandle(mapHandle);
    UnmapViewOfFile(mapView);
    CloseHandle(fHandle);
    for(int i = 0; i < COUNT; i++)
        CloseHandle(pageMutex[i]);
    CloseHandle(freeSem);
    CloseHandle(usedSem);

    std::cout << std::endl << "The work is done." << std::endl;
    system("pause");
}
