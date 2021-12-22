#include <windows.h>
#include <iostream>

//Функция для асинхронного чтения данных
void WINAPI Callback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	std::cout << "Information received." << std::endl;
}

int main()
{
	system("cls");

	CHAR buffer[512]; //Буфер для передачи сообщений
	BOOL isConnected = FALSE; //Проверка подключения
	LPCTSTR Pipename = TEXT("\\\\.\\pipe\\lab4"); //Название именованного канала

	HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL); //Создание события для организации асинхронного чтения
	HANDLE Pipe = CreateFile(Pipename, //Создание именованного канала
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL
	);

	int choose;

	OVERLAPPED overlapped = OVERLAPPED(); //Содержит информацию, используемую в асинхронном (или перекрывающем) вводе/выводе данных

	//При успешном создании объектов HANDLE программа начинает свою основную работу

	if (Event != INVALID_HANDLE_VALUE && Pipe != INVALID_HANDLE_VALUE)
	{
		do
		{
			system("cls");

			std::cout << "1. Receive message" << std::endl;
			std::cout << "2. Disconnect from a named pipe" << std::endl;
			std::cout << "0. Exit" << std::endl;

			std::cin >> choose;

			switch (choose)
			{
			case 1:
				overlapped.hEvent = Event;
				isConnected = ReadFileEx(Pipe, (LPVOID)buffer, 512, &overlapped, Callback);	//Запуск асинхронного чтения

				if (isConnected)
				{
					std::cout << buffer << std::endl;
					system("pause");
				}
				else
				{
					std::cout << "Error" << std::endl;
					system("pause");
				}
				break;
			case 2:
				isConnected = CloseHandle(Pipe); //Закрытие именованного канала

				if (isConnected)
					std::cout << "You have been disconnected from the named pipe" << std::endl;
				else
					std::cout << "Error" << std::endl;

				isConnected = FALSE;
				system("pause");
				break;
			default:
				break;
			}
		}
		while (choose);
	}
	else
		std::cout << "Error" << std::endl;

	return 0;
}
