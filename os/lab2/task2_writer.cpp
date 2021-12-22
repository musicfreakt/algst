#include <iostream>
#include <string>
#include <windows.h>

int main()
{
    HANDLE hFile = NULL;
    std::string fileName;
    std::string mapName;
    LPVOID addrMap = NULL;
    HANDLE hMap = NULL;

    std::cout << "Enter file name: ";
    std::cin >> fileName;

    hFile = CreateFileA(fileName.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if(hFile == INVALID_HANDLE_VALUE || hFile == NULL)
    {
        std::cout << "Error: " << GetLastError() << std::endl;
        return GetLastError();
    }

    std::cout << "Enter map name: ";
    std::cin >> mapName;

    hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 128, mapName.c_str());
    if(hMap == NULL)
    {
        std::cout << "Error: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return GetLastError();
    }

    addrMap = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0);
    if(addrMap == NULL)
    {
        std::cout << "Error: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return GetLastError();
    }

    std::cout << "Trying to write to " << addrMap;
    std::cout << ": \n";
    getchar();
    std::string toWrite;
    getline(std::cin, toWrite);

    CopyMemory(addrMap, toWrite.c_str(), toWrite.length() * sizeof(char));

    std::cout << "Run reader program to continue... \n";

    unsigned exit;
    do
    {
        std::cout << "To exit from writer program enter 1. \n>";
        std::cin >> exit;
    }
    while(exit != 1);

    CloseHandle(hFile);
    UnmapViewOfFile(addrMap);
    return 0;
}
