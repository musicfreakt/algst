#include <iostream>
#include <chrono>
#include <windows.h>

// int callbackCounter = 0;
// private unsafe void OperationEndRoutine(uint errorCode,
//    uint numberOfBytesTransfered, NativeOverlapped* overlapped)
// {
//    callbackCounter++;
// }
//
// private readonly IOCompletionCallback callback;

int callbackCounter = 0;
VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    // Отслеживаем количество вызовов
    callbackCounter++;
}

void ReadFileOverlapped(int fileSize, int blockSize, int operationsCount,
            OVERLAPPED* overlap, char** buffer, HANDLE fileHandle)
{
    //read via multiple streams
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
    //wait for all operations to end
    while (callbackCounter < operationsStarted)
        SleepEx(-1, true);

    //move to next memory block
    for (int i = 0; i < operationsCount; i++)
        overlap[i].Offset += (int)blockSize * operationsCount;
    callbackCounter = 0;
}

void WriteFileOverlapped(int fileSize, int blockSize, int operationsCount,
            OVERLAPPED* overlap, char** buffer, HANDLE fileHandle)
{
    //write via multiple streams
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
    //wait for all operations to end
    while (callbackCounter < operationsStarted)
        SleepEx(-1, true);

    //move to next memory block
    for (int i = 0; i < operationsCount; i++)
        overlap[i].Offset += (int)blockSize * operationsCount;
    callbackCounter = 0;
}

void CopyFileOverlapped(HANDLE sourceHandle, HANDLE targetHandle, unsigned int blockSize, int operationsCount)
{
    int srcSize = 0;
    int curSize = 0;
    unsigned int high = 0;
    // srcSize = curSize = GetFileSize(sourceHandle, ref high);

    char** buffer = new char*[operationsCount];
    for (int i = 0; i < operationsCount; ++i)
        buffer[i] = new char[blockSize];

    try
    {
        OVERLAPPED* over_1 = new OVERLAPPED[operationsCount];
        OVERLAPPED* over_2 = new OVERLAPPED[operationsCount];

        for (int i = 0; i < operationsCount; ++i)
        {
            over_1[i].Offset = over_2[i].Offset = i * (int)blockSize;
            over_1[i].OffsetHigh = over_2[i].OffsetHigh = i * (int)high;
        }

        do
        {
            ReadFileOverlapped(srcSize, blockSize, operationsCount, over_1, buffer, sourceHandle);
            WriteFileOverlapped(srcSize, blockSize, operationsCount, over_2, buffer, targetHandle);
            curSize -= blockSize * operationsCount;
        }
        while (curSize > 0);

        SetFilePointer(targetHandle, srcSize, NULL, 0);
        SetEndOfFile(targetHandle);
    }
    catch (...)
    {
        std::cout << "Error code: " << GetLastError() << " milliseconds\n";
    }
}


int main()
{
    unsigned long long blockSize;
    int operations;
    std::string sourcePath, targetPath;

    std::cout << "Enter size of the block:\n";
    std::cout << "> 4096*";
    std::cin >> blockSize;
    blockSize *= 4096;
    //Ввод количества операций
    std::cout << "Enter number of the operations:\n";
    std::cout << "> ";
    std::cin >> operations;

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
        auto delta = std::chrono::duration_cast<std::chrono::duration<double>>(begin-end);
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
