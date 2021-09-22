#include <iostream>
#include <windows.h>
using namespace std;

int callback = 0;
int operations;
int sizeToCopy;
HANDLE firstHandle;
HANDLE secondHandle;
OVERLAPPED* over_1;
OVERLAPPED* over_2;

void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    callback++;
}

void copyFile(HANDLE firstHandle, HANDLE secondHandle, DWORD blockSize, int operationsCount)
{
    int fileSize = GetFileSize(firstHandle, NULL);
    int firstSize = fileSize;
    int countOperations = 0;
    CHAR* buffer =  new CHAR[fileSize + (fileSize % blockSize > 0 ? 1 : 0) * blockSize];
    CHAR* buffer2 = new CHAR[fileSize + (fileSize % blockSize > 0 ? 1 : 0) * blockSize];

    OVERLAPPED* over_1 = new OVERLAPPED[operationsCount];
    OVERLAPPED* over_2 = new OVERLAPPED[operationsCount];
    for (int i = 0; i < operationsCount; i++)
    {
        over_1[i].Offset = 0;
        over_1[i].OffsetHigh = 0;
        over_1[i].hEvent = NULL;
        over_2[i].Offset = 0;
        over_2[i].OffsetHigh = 0;
        over_2[i].hEvent = NULL;
    }
    buffer2 = buffer;
    bool inFlag = false;
    bool outFlag = false;
    do
    {
        callback = 0;
        countOperations = 0;
        for (int i = 0; i < operationsCount; i++)
        {
            if (fileSize > 0)
            {
                countOperations++;
                if (i || inFlag)
                {
                    for (int j = 0; j < operationsCount; j++)
                    over_1[j].Offset += blockSize;
                    buffer += blockSize;
                }
                inFlag = true;
                ReadFileEx(firstHandle, buffer, blockSize, &over_1[i], FileIOCompletionRoutine);
                fileSize -= blockSize;
            }
        }
        while (callback < countOperations)
            SleepEx(-1, TRUE);
        callback = 0;
        for (int i = 0; i < countOperations; i++)
        {
            if (i || outFlag)
            {
                for (int j = 0; j < operationsCount; j++)
                over_2[j].Offset += blockSize;
                buffer2 += blockSize;
            }
            outFlag = true;
            WriteFileEx(secondHandle, buffer2, blockSize, &over_2[i], FileIOCompletionRoutine);
        }
        while (callback < countOperations)
            SleepEx(-1, TRUE);
        }
        while (fileSize > 0);
        SetFilePointer(secondHandle, firstSize, NULL, FILE_BEGIN);
        SetEndOfFile(secondHandle);
}

int main()
{
    int number = 0;
    int part = 0;
    int operations = 0;
    const DWORD directoryBufferSize = MAX_PATH;
    char directoryBuffer[directoryBufferSize];
    const DWORD directoryOut = GetCurrentDirectory(directoryBufferSize, directoryBuffer);
    if (directoryOut > 0 && directoryOut <= directoryBufferSize)
    {
        cout << "Enter size of the block:\n";
        cout << "> 4096*";
        do {
            cin >> part;
        } while (part == 0);
        cout << "Enter number of the operations:\n1     2     4     8     12     16     \n";
        cout << "> ";
        do {
            cin >> operations;
        } while (operations == 0);
        cout << "Enter the directory of first file:" << endl;
        string firstFileName;
        cin >> firstFileName;
        cout << "Enter the directory of second file:" << endl;
        string secondFileName;
        cin >> secondFileName;
        if (firstFileName.length() < MAX_PATH)
        {
            firstHandle = CreateFile(firstFileName.data(), GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
            secondHandle = CreateFile(secondFileName.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
            if (firstHandle != INVALID_HANDLE_VALUE)
            {
                sizeToCopy = 4096 * part;
                DWORD startTime = GetTickCount();
                copyFile(firstHandle, secondHandle, sizeToCopy, operations);
                DWORD endTime = GetTickCount();
                cout << endl << "Time to copy: " << endTime - startTime << endl;
                delete[] over_1, over_2;
            }
            else
            {
                if (GetLastError())
                cout << endl << "ERROR: " << GetLastError() << endl;
            }
        }
        if (!CloseHandle(firstHandle))
            cout << "ERROR: Could not close the handle!" << endl;
        if (!CloseHandle(secondHandle))
        cout << "ERROR: Could not close the handle!" << endl;
    }
    else
    {
        cout << "Too long name!";
    }
}
