int streamCount;
int numbersOfByteToWrite;
int bufferMultiplier;
char **buffersArray; 

using namespace std;

OVERLAPPED *overlapIn, *overlapOut;
HANDLE original, copyFile; 
LARGE_INTEGER fileSize, endOfFile;
LONGLONG  doneCount, recordCount; 

void asyncCopyOfFile() 
{
	int overlapOperationsCount = 0;
	numbersOfByteToWrite = 4096;
	bufferMultiplier = 1;

	char sourceCharFile[250], destinationCharFile[250];

	cout << "Enter the path to the file: ";
	cin >> sourceCharFile;
	cout << "Enter the path where to copy the file: ";
	cin >> destinationCharFile;
	cout << "Number of overlapping operations: ";
	cin >> overlapOperationsCount;
	cout << "Buffer Multiplier: ";
	cin >> bufferMultiplier;
	numbersOfByteToWrite *= bufferMultiplier;
	cout << "Numbers of byte to write = " << numbersOfByteToWrite << "\n";

	buffersArray = new char*[overlapOperationsCount];
	for (int i = 0; i<overlapOperationsCount; i++)
		buffersArray[i] = new char[numbersOfByteToWrite];

	overlapIn = new OVERLAPPED[overlapOperationsCount];
	overlapOut = new OVERLAPPED[overlapOperationsCount];

	LARGE_INTEGER curPosIn;
	wchar_t source[250], destination[250];
	mbstowcs(source, sourceCharFile, 250);
	mbstowcs(destination, destinationCharFile, 250);
	original = CreateFile(source, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
	copyFile = CreateFile(destination, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);

	GetFileSizeEx(original, &fileSize);

	cout << "File size = " << fileSize.QuadPart << "\n";

	recordCount = fileSize.QuadPart / numbersOfByteToWrite;
	if ((fileSize.QuadPart % numbersOfByteToWrite) != 0) 
		++recordCount;
	cout << "Record = " << recordCount << "\n";

	DWORD startCopyTime, endCopyTime;
	startCopyTime = GetTickCount();
	curPosIn.QuadPart = 0;

	for (int i = 0; i < overlapOperationsCount; ++i) {
		overlapIn[i].hEvent = (HANDLE)i;
		overlapOut[i].hEvent = (HANDLE)i;
		overlapIn[i].Offset = curPosIn.LowPart;
		overlapIn[i].OffsetHigh = curPosIn.HighPart;
		if (curPosIn.QuadPart < fileSize.QuadPart)
			ReadFileEx(original, buffersArray[i], numbersOfByteToWrite, &overlapIn[i], asyncRead);
		curPosIn.QuadPart += (LONGLONG)numbersOfByteToWrite;
	}

	doneCount = 0;
	while (doneCount < 2 * recordCount)
		SleepEx(INFINITE, true); 

	cout << "Copying completed successfully" << "\n";

	delete[] overlapIn;
	delete[] overlapOut;
	for (int i = 0; i < overlapOperationsCount; ++i)
		delete[]buffersArray[i];
	delete[] buffersArray;

	endOfFile.QuadPart = fileSize.QuadPart;
	endOfFile.HighPart = fileSize.HighPart;
	SetFilePointerEx(copyFile, endOfFile, 0, FILE_BEGIN);
	SetEndOfFile(copyFile);

	CloseHandle(original);
	CloseHandle(copyFile);
	endCopyTime = GetTickCount();
	cout << "Time: " << endCopyTime - startCopyTime << " ms\n";
}

void WINAPI asyncRead(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv) 
{

	++doneCount;

	LARGE_INTEGER curPosIn, curPosOut;
	DWORD i = (DWORD)(lpOv->hEvent);

	curPosIn.LowPart = overlapIn[i].Offset;
	curPosIn.HighPart = overlapIn[i].OffsetHigh;
	curPosOut.QuadPart = curPosIn.QuadPart;
	overlapOut[i].Offset = curPosOut.LowPart;
	overlapOut[i].OffsetHigh = curPosOut.HighPart;
	WriteFileEx(copyFile, buffersArray[i], numbersOfByteToWrite, &overlapOut[i], asyncWrite);
	curPosIn.QuadPart += numbersOfByteToWrite * (LONGLONG)(streamCount);
	overlapIn[i].Offset = curPosIn.LowPart;
	overlapIn[i].OffsetHigh = curPosIn.HighPart;
}

void WINAPI asyncWrite(DWORD Code, DWORD nBytes, LPOVERLAPPED lpOv)
{

	++doneCount;

	LARGE_INTEGER curPosIn;
	DWORD i = (DWORD)(lpOv->hEvent);
	
	curPosIn.LowPart = overlapIn[i].Offset;
	curPosIn.HighPart = overlapIn[i].OffsetHigh;
	if (curPosIn.QuadPart < fileSize.QuadPart)
		ReadFileEx(original, buffersArray[i], numbersOfByteToWrite, &overlapIn[i], asyncRead);
}