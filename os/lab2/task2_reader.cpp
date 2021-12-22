#include <iostream>
#include <string>
#include <windows.h>

int main()
{
    HANDLE MapHandle = nullptr;
    LPVOID addrMap = NULL;
    std::string mapName;

    std::cout << "Enter map name: ";
    std::cin >> mapName;

    MapHandle = OpenFileMappingA(FILE_MAP_READ, FALSE, mapName.c_str());
    if(MapHandle == NULL)
    {
        std::cout << "Error: " << GetLastError() << std::endl;
        return GetLastError();
    }

    addrMap = MapViewOfFile(MapHandle, FILE_MAP_READ, 0, 0, 0);
    if(addrMap == NULL)
    {
        std::cout << "Error: " << GetLastError() << std::endl;
        CloseHandle(MapHandle);
        return GetLastError();
    }

    std::cout << "Trying to read from " << addrMap << ": \n";
    std::cout << (char*)addrMap << std::endl << std::endl;

    CloseHandle(MapHandle);
    UnmapViewOfFile(addrMap);
    return 0;
}
