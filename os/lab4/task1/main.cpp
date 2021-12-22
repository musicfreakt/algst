#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main()
{
    const int pageSize = 4096; //Размер страницы
    const int Count = 17; //Число страниц буферной памяти
    const int N = Count - 1;  //Количестов семафоров (с учётом нулевого)
    const int writerCount = Count / 2; //Счётчик писателей
    const int readerCount = Count / 2; //Счётчик читателей

    int fileSize = N * pageSize;

    HANDLE freeSem = CreateSemaphore(NULL, N, N, "freeSem"); //Создание семафора для неотработанных страниц
    HANDLE usedSem = CreateSemaphore(NULL, 0, N, "usedSem"); //Создание семафора для отработанных страниц
    HANDLE fileMutex = CreateMutex(NULL, false, "fileMutex"); //Мьютекс

    HANDLE mapHandle;

    cout << "This program will copy data from file named 'file.txt' by using semaphores and mutex.\n";
    cout << "Wait for a few seconds to see a result\n";

    //Создаём файл, с которым будет производиться работа
    HANDLE fHandle = CreateFileA("file.txt", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    SetFilePointer(fHandle, fileSize, 0, FILE_BEGIN);
    SetEndOfFile(fHandle);

    //Создаём проекцию файла
    mapHandle = CreateFileMapping(fHandle, NULL, PAGE_READWRITE, 0, 0, "filemap");

    //Отображаем проекцию
    LPVOID mapView = MapViewOfFile(mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);

    //Блокируем страницы
    VirtualLock(mapView, fileSize);

    //Массив HANDLE для процессов
    HANDLE processHandles[Count];
    HANDLE logHandles[Count];

    //Цикл для писателей
    for (int i = 0; i < writerCount; ++i)
    {
        //Называем очередной файл

        string logName = "writelog_" + to_string(i + 1) + ".txt";

        STARTUPINFO sysInfo; //Информация для создания процесса
        PROCESS_INFORMATION procInfo;  //Объект для записи информации о процессе
        SECURITY_ATTRIBUTES secureAttr = { sizeof(secureAttr), NULL, TRUE }; //Атрибуты безопасности

        //Обнуляем нашу информацию о процессе
        ZeroMemory(&sysInfo, sizeof(sysInfo));

        //Создаём файл для логгирования
        HANDLE outHandle = CreateFile(logName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, &secureAttr,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        sysInfo.cb = sizeof(sysInfo); //Устанавливает размер структуры, в байтах
        sysInfo.hStdOutput = outHandle; //Определяет дескриптор, который будет использован как стандартный дескриптор вывода для процесса
        sysInfo.hStdError = NULL;  //Определяет дескриптор, который будет использован как стандартный дескриптор ошибки для процесса
        sysInfo.hStdInput = NULL; //Определяет дескриптор, который будет использован как стандартный дескриптор ввода для процесса
        sysInfo.dwFlags |= STARTF_USESTDHANDLES; //Устанавливается стандартный ввод данных, стандартный вывод и стандартная обработка ошибок дескрипторов процесса

        ZeroMemory(&procInfo, sizeof(procInfo));

        int mainProcess = CreateProcess("writer.exe",
            NULL, // Командная строка
            NULL, // Дескриптор процесса не наследуется
            NULL, // Дескриптор потока не наследуется
            TRUE, // Установить наследование дескрипторов в TRUE
            0, // Нет флагов создания
            NULL, // Использовать родительский блок среды
            NULL, // Использовать начальный каталог родителя
            &sysInfo, // Указатель на структуру STARTUPINFO
            &procInfo); // Указатель на структуру PROCESS_INFORMATION

        if (mainProcess)
        {
            processHandles[i] = procInfo.hProcess;
            logHandles[i] = sysInfo.hStdOutput;
        }
    }

    //Аналогично работаем и с читателями
    for (int i = 0; i < readerCount; ++i)
    {
        string fname = "readlog_" + to_string(i + 1) + ".txt";

        STARTUPINFO sysInfo;
        PROCESS_INFORMATION procInfo;
        SECURITY_ATTRIBUTES secureAttr = { sizeof(secureAttr), NULL, TRUE };

        ZeroMemory(&sysInfo, sizeof(sysInfo));

        sysInfo.hStdOutput = CreateFile(fname.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, &secureAttr,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        sysInfo.cb = sizeof(sysInfo);
        sysInfo.hStdError = NULL;
        sysInfo.hStdInput = NULL;
        sysInfo.dwFlags |= STARTF_USESTDHANDLES;
        ZeroMemory(&procInfo, sizeof(procInfo));

        int createProcess = CreateProcess("reader.exe",
            NULL, // Командная строка
            NULL, // Дескриптор процесса не наследуется
            NULL, // Дескриптор потока не наследуется
            TRUE, // Установить наследование дескрипторов в TRUE
            0, // Нет флагов создания
            NULL, // Использовать родительский блок среды
            NULL, // Использовать начальный каталог родителя
            &sysInfo, // Указатель на структуру STARTUPINFO
            &procInfo); // Указатель на структуру PROCESS_INFORMATION


        if (createProcess != 0)
        {
            processHandles[writerCount + i] = procInfo.hProcess;
            logHandles[writerCount + i] = sysInfo.hStdOutput;
        }
    }

    WaitForMultipleObjects(Count, processHandles, true, INFINITE);

    for (int i = 0; i < Count; ++i)
    {
        CloseHandle(logHandles[i]);
        CloseHandle(processHandles[i]);
    }

    CloseHandle(mapHandle);
    UnmapViewOfFile(mapView);
    CloseHandle(fHandle);
    CloseHandle(fileMutex);
    CloseHandle(freeSem);
    CloseHandle(usedSem);

    cout << endl << "The work is done. Press ENTER to close the program." << endl;
    system("pause");
}
