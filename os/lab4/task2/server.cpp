#include <windows.h>
#include <iostream>


int main()
{
	system("cls");

	CHAR buffer[512]; // Буфер для передачи сообщений
	BOOL isConnected = FALSE; // Проверка подключения
	OVERLAPPED overlapped = OVERLAPPED(); // Содержит информацию, используемую в асинхронном (или перекрывающем) вводе/выводе данных
	OVERLAPPED syncPipe = OVERLAPPED();	// Перекрывающаяся структура

	HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE Pipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\lab4"),							// Имя канала
								  PIPE_ACCESS_DUPLEX,										// Канал двунаправленный
								  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
								  PIPE_UNLIMITED_INSTANCES,									// Макс. число запросов
								  512,														// Выводимый размер буфера
								  512,														// Вводимый размер буфера
								  0,														// time-out время, мс
								  NULL														// Указатель на атрибуты безопасности
								  );

	int choose;

	if (Event != INVALID_HANDLE_VALUE && Pipe != INVALID_HANDLE_VALUE)
	{
		do
		{
			system("cls");
			std::cout << "1. Connect to named pipe" << std::endl;
			std::cout << "2. Send a message" << std::endl;
			std::cout << "3. Disconnect from a named pipe" << std::endl;
			std::cout << "0. Exit" << std::endl;

			std::cin >> choose;

			switch (choose)
			{
			case 1:
				syncPipe.hEvent = Event;
				isConnected = ConnectNamedPipe(Pipe, &syncPipe);
				WaitForSingleObject(Event, INFINITE);

				if (isConnected)
					std::cout << "The connection was successful" << std::endl;
				else
					std::cout << "Error" << std::endl;

				system("pause");
				break;
			case 2:
				if (isConnected == FALSE) std::cout << "No connection!" << std::endl;
				else
				{
					std::cout << "Enter a message: ";
					std::cin >> buffer;

					overlapped.hEvent = Event;
					isConnected = WriteFile(Pipe, (LPCVOID)buffer, 512, NULL, &overlapped);

					if (WaitForSingleObject(Event, 20000) == WAIT_OBJECT_0 && isConnected)
						std::cout << "The recording was successful" << std::endl;
					else
						std::cout << "Error" << std::endl;
				}

				std::cout << std::endl;
				system("pause");
				break;
			case 3:
				isConnected = DisconnectNamedPipe(Pipe);

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
		} while (choose);
	}
	else
		std::cout << "Error" << std::endl;

	if (Pipe != INVALID_HANDLE_VALUE)
		CloseHandle(Pipe);
	if (Event != INVALID_HANDLE_VALUE)
		CloseHandle(Event);

	return 0;
}
