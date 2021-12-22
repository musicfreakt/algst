#include <iostream>
#include <chrono>
#include <windows.h>
#define TEST

LARGE_INTEGER shiftRead;
LARGE_INTEGER shiftWrite;

int callbackCounter = 0;
VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    // Отслеживаем количество вызовов
    callbackCounter++;
}

void ReadFileOverlapped(long long fileSize, DWORD blockSize, int operationsCount,
            LPOVERLAPPED overlap, char** buffer, HANDLE fileHandle)
{
    int operationsStarted = 0;
    for (int i = 0; i < operationsCount; i++)
    {
        if (fileSize > 0)
        {
            operationsStarted++;
            ReadFileEx(fileHandle, buffer[i], blockSize, &overlap[i], FileIOCompletionRoutine);
            fileSize -= blockSize;
        }
    }

    while (callbackCounter < operationsStarted)
        SleepEx(-1, true);

    for (int i = 0; i < operationsCount; i++)
    {
        overlap[i].Offset = shiftRead.LowPart;
        overlap[i].OffsetHigh = shiftRead.HighPart;
        shiftRead.QuadPart += blockSize;
    }
        // overlap[i].Offset += (DWORD) blockSize * operationsCount;
    callbackCounter = 0;
}

void WriteFileOverlapped(long long fileSize, DWORD blockSize, int operationsCount,
            LPOVERLAPPED overlap, char** buffer, HANDLE fileHandle)
{
    int operationsStarted = 0;
    for (int i = 0; i < operationsCount; i++)
    {
        if (fileSize > 0)
        {
            operationsStarted++;
            WriteFileEx(fileHandle, buffer[i], blockSize, &overlap[i], FileIOCompletionRoutine);
            fileSize -= blockSize;
        }
    }

    while (callbackCounter < operationsStarted)
        SleepEx(-1, true);

    for (int i = 0; i < operationsCount; i++)
    {
        overlap[i].Offset = shiftWrite.LowPart;
        overlap[i].OffsetHigh = shiftWrite.HighPart;
        shiftWrite.QuadPart += blockSize;
    }
        // overlap[i].Offset += (DWORD) blockSize * operationsCount;
    callbackCounter = 0;
}

void CopyFileOverlapped(HANDLE sourceHandle, HANDLE targetHandle, DWORD blockSize, int operationsCount)
{
    long long srcSize = 0;
    long long curSize = 0;
    LARGE_INTEGER fileSizeStruct;
    GetFileSizeEx(sourceHandle, &fileSizeStruct);
    srcSize = curSize = fileSizeStruct.QuadPart;

    char** buffer = new char*[operationsCount];
    for (int i = 0; i < operationsCount; ++i)
        buffer[i] = new char[blockSize];

    try
    {
        LPOVERLAPPED over_1 = new OVERLAPPED[operationsCount];
        LPOVERLAPPED over_2 = new OVERLAPPED[operationsCount];
        shiftRead.QuadPart = 0;
        shiftWrite.QuadPart = 0;
        for (int i = 0; i < operationsCount; ++i)
        {
            over_1[i].Offset = over_2[i].Offset = shiftRead.LowPart;
            over_1[i].OffsetHigh = over_2[i].OffsetHigh = shiftRead.HighPart;
            over_1[i].hEvent = over_2[i].hEvent = NULL;
            shiftRead.QuadPart += blockSize;
            shiftWrite.QuadPart += blockSize;
        }

        do
        {
            ReadFileOverlapped(srcSize, blockSize, operationsCount, over_1, buffer, sourceHandle);
            WriteFileOverlapped(srcSize, blockSize, operationsCount, over_2, buffer, targetHandle);
            curSize -= (long long) (blockSize * operationsCount);
        }
        while (curSize > 0);

        SetFilePointerEx(targetHandle, fileSizeStruct, NULL, FILE_BEGIN);
        SetEndOfFile(targetHandle);
    }
    catch (...)
    {
        std::cout << "Error code: " << GetLastError() << " milliseconds\n";
    }
}

void printArray(unsigned* a, size_t n)
{
    std::cout << "[";
    for(size_t i = 0; i < n; ++i)
    {
        std::cout << std::to_string(a[i]);
        if(i != n-1)
            std::cout << ", ";
    }
    std::cout << "];\n";
}

#ifndef TEST
    int main()
    {
        DWORD blockSize;
        DWORD sectorsPerCluster;
        DWORD bytesPerSector;
        GetDiskFreeSpaceA(NULL, &sectorsPerCluster, &bytesPerSector, NULL, NULL);
        int operations;
        std::string sourcePath, targetPath;

        std::cout << "Enter size of the block:\n>" << bytesPerSector*sectorsPerCluster << "*";
        std::cin >> blockSize;
        blockSize *= bytesPerSector*sectorsPerCluster;

        std::cout << "Enter number of the operations:\n> ";
        std::cin >> operations;

        std::cout << "Enter the directory of first file:\n> ";
        std::cin >> sourcePath;

        std::cout << "Enter the directory of second file:\n> ";
        std::cin >> targetPath;

        HANDLE sourceHandle = CreateFile(sourcePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

        if (sourceHandle == INVALID_HANDLE_VALUE)
        {
            std::cout << "Error reading file. Error code:" << GetLastError() << "\n";
            return -1;
        }

        HANDLE targetHandle = CreateFile(targetPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

        if (targetHandle == INVALID_HANDLE_VALUE)
        {
            std::cout << "Error creating target file. Error code:" << GetLastError() << "\n";
            CloseHandle(sourceHandle);
            return -1;
        }

        try
        {
            auto begin = std::chrono::high_resolution_clock::now(); // ПАРОВОЗИК ЧУХ-ЧУХ!!!
            CopyFileOverlapped(sourceHandle, targetHandle, blockSize, operations);
            auto end = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);
            std::cout << "File copied successfully. Copy time: " << delta.count() << " milliseconds\n";
        }
        catch (...)
        {
            std::cout << "Error file copy. Error code: " << GetLastError() << " milliseconds\n";
        }

        CloseHandle(sourceHandle);
        CloseHandle(targetHandle);

        return 0;
    }
#else
    // int main()
    // {
    //     const unsigned attempts = 5;
    //     DWORD blockSize;
    //     DWORD sectorsPerCluster;
    //     DWORD bytesPerSector;
    //     GetDiskFreeSpaceA(NULL, &sectorsPerCluster, &bytesPerSector, NULL, NULL);
    //     int operations = 1;
    //
    //     unsigned *results = new unsigned[20];
    //     unsigned *x = new unsigned[20];
    //     std::string sourcePath, targetPath;
    //
    //     std::cout << "Enter the directory of first file:\n> ";
    //     std::cin >> sourcePath;
    //
    //     std::cout << "Enter the directory of second file:\n> ";
    //     std::cin >> targetPath;
    //
    //     blockSize = sectorsPerCluster*bytesPerSector;
    //     for (int i = 1; i <= 20; ++i)
    //     {
    //         DWORD sumMillis = 0;
    //         for (int j = 0; j < attempts; ++j)
    //         {
    //             HANDLE sourceHandle = CreateFile(sourcePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
    //                 FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
    //
    //             HANDLE targetHandle = CreateFile(targetPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
    //                 FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
    //
    //             auto begin = std::chrono::high_resolution_clock::now();
    //             CopyFileOverlapped(sourceHandle, targetHandle, i*blockSize, operations);
    //             auto end = std::chrono::high_resolution_clock::now();
    //             auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);
    //             auto count = delta.count();
    //             sumMillis += count;
    //             std::cout << "Attempt " << j << "; BLOCKSIZE:" << i*blockSize << " ;Copy time: " << count << " milliseconds\n";
    //             CloseHandle(sourceHandle);
    //             CloseHandle(targetHandle);
    //         }
    //         results[i-1] = sumMillis/attempts;
    //         x[i-1] = i;
    //     }
    //     printArray(x, 20);
    //     printArray(results, 20);
    //     delete results;
    //     delete x;
    //     return 0;
    // }
    int main()
    {
        const unsigned attempts = 5;
        DWORD blockSize;
        DWORD sectorsPerCluster;
        DWORD bytesPerSector;
        GetDiskFreeSpaceA(NULL, &sectorsPerCluster, &bytesPerSector, NULL, NULL);
        // int operations = 1;

        unsigned *results = new unsigned[16];
        unsigned *x = new unsigned[16];
        std::string sourcePath, targetPath;

        std::cout << "Enter the directory of first file:\n> ";
        std::cin >> sourcePath;

        std::cout << "Enter the directory of second file:\n> ";
        std::cin >> targetPath;

        blockSize = 8*sectorsPerCluster*bytesPerSector;
        for (int i = 1; i <= 16; ++i)
        {
            DWORD sumMillis = 0;
            for (int j = 0; j < attempts; ++j)
            {
                HANDLE sourceHandle = CreateFile(sourcePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
                    FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

                HANDLE targetHandle = CreateFile(targetPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                    FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

                auto begin = std::chrono::high_resolution_clock::now();
                CopyFileOverlapped(sourceHandle, targetHandle, blockSize, i);
                auto end = std::chrono::high_resolution_clock::now();
                auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);
                auto count = delta.count();
                sumMillis += count;
                std::cout << "Attempt " << j << "; Operations:" << i << " ;Copy time: " << count << " milliseconds\n";
                CloseHandle(sourceHandle);
                CloseHandle(targetHandle);
            }
            results[i-1] = sumMillis/attempts;
            x[i-1] = i;
        }
        printArray(x, 16);
        printArray(results, 16);
        delete results;
        delete x;
        return 0;
    }
#endif
