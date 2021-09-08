#include <iostream>
#include "files.cpp"

using namespace std;

int menu()
{
	system("cls");
	int menu_item;
	do
	{
		cin.clear();
		cin.sync();

		cout << "Menu:" << endl;
		cout << "1. Displaying a list of disks" << endl;
		cout << "2. Output information about the disk" << endl;
		cout << "3. Create a folder" << endl;
		cout << "4. Delete a folder" << endl;
		cout << "5. Create a file" << endl;
		cout << "6. Copy a file" << endl;
		cout << "7. Move a file" << endl;
		cout << "8. Information about the file" << endl;
		cout << "9. Change file attributes" << endl;
		cout << "10. Change the file creation time" << endl;
		cout << "11. Asynchronous file copying" << endl;
		cout << "0. Exit" << endl;
		cout << "> ";
		cin >> menu_item;
		if (cin.fail())
			cout << "Something went wrong, select the menu item again" << endl;
	}
	while (cin.fail());
	system("cls");
	return menu_item;
}

int main()
{
	int notExit;
	do
	{
		switch (notExit = menu())
		{
		case 1:
			showDrivesList();
			break;
		case 2:
			showDriveInfo();
			break;
		case 3:
			createFolder();
			break;
		case 4:
			deleteFolder();
			break;
		case 5:
			createFile();
			break;
		case 6:
			copyFile();
			break;
		case 7:
			moveFile();
			break;
		case 8:
			// fileInfo();
			break;
		case 9:
			// changeFileAttributes();
			break;
		case 10:
			// changeCreationTime();
			break;
		case 11:
			// asyncCopyOfFile();
			break;
		case 0:
			break;
		default:
			if(notExit)
				cout << "There is no such option, repeat the input" << endl;
		}
		if(notExit)
			system("pause");
	}
	while (notExit);
	cin.get();
	return 0;
}
