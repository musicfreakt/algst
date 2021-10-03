#include <iostream>
#include <windows.h>
#include <io.h>
#include <string>

using namespace std;

void showDrivesList()
{
    int n;
    char drive_letter;
    DWORD drives = GetLogicalDrives();

    cout << "List of logical disks:\n";

    for (int i = 0; i < 26; i++)
        if (((drives >> i) & 0x1))
            cout << char('A' + i) << endl;
}


void showDriveInfo()
{
    string drive;
    cout << "Enter the volume label (for example, C): ";
    cin >> drive;
    drive += ":\\";
    unsigned int driveType = GetDriveType(drive.c_str());
    cout << "Disk type: ";
    switch (driveType)
    {
        case DRIVE_UNKNOWN:
            cout << "Unknown type\n";
            return;
        case DRIVE_NO_ROOT_DIR:
            cout << "There is no disk with this label\n";
            return;
        case DRIVE_REMOVABLE:
            cout << "Removable drive";
            break;
        case DRIVE_FIXED:
            cout << "Fixed drive";
            break;
        case DRIVE_REMOTE:
            cout << "Remote drive";
            break;
        case DRIVE_CDROM:
            cout << "CDROM";
            break;
        case DRIVE_RAMDISK:
            cout << "RAM disk";
            break;
    }
    cout << endl;

    char volume_name[255];
    char filesystem_name[100];
    DWORD max_component_length = 0;
    DWORD system_flags = 0;
    unsigned long drive_serial_number = 0;

    GetVolumeInformation(drive.c_str(), volume_name, 255,
		&drive_serial_number, &max_component_length, &system_flags,
		filesystem_name, 100);

    cout << "Disk name: " << volume_name << endl <<
        "Serial number: " << drive_serial_number << endl <<
        "File system type: " << filesystem_name << endl <<
	    "System flags: " << endl;

    const string temp = "The specified volume";
    const string temp_ = temp + " supports";
    if (system_flags & FILE_CASE_PRESERVED_NAMES)
        cout << temp_ +" preserved case of file names when it places a name on disk.\n";
    if (system_flags & FILE_CASE_SENSITIVE_SEARCH)
        cout << temp_ + " case-sensitive file names.\n";
    if (system_flags & FILE_FILE_COMPRESSION)
        cout << temp_ + " file-based compression.\n";
    if (system_flags & FILE_NAMED_STREAMS)
        cout << temp_ + " named streams.\n";
    if (system_flags & FILE_PERSISTENT_ACLS)
        cout << temp + " preserves and enforces access control lists (ACL). \n";
    if (system_flags & FILE_READ_ONLY_VOLUME)
        cout << temp + " is read-only.\n";
    if (system_flags & FILE_SEQUENTIAL_WRITE_ONCE)
        cout << temp_ + " a single sequential write.\n";
    if (system_flags & FILE_SUPPORTS_ENCRYPTION)
        cout << temp_ + " the Encrypted File System (EFS).\n";
    if (system_flags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
        cout << temp_ + " extended attributes.\n";
    if (system_flags & FILE_SUPPORTS_HARD_LINKS)
        cout << temp_ + " hard links. \n";
    if (system_flags & FILE_SUPPORTS_OBJECT_IDS)
        cout << temp_ + " object identifiers.\n";
    if (system_flags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
        cout << temp_ + " open by FileID.\n";
    if (system_flags & FILE_SUPPORTS_REPARSE_POINTS)
        cout << temp_ + " reparse points.\n";
    if (system_flags & FILE_SUPPORTS_SPARSE_FILES)
        cout << temp_ + " sparse files.\n";
    if (system_flags & FILE_SUPPORTS_TRANSACTIONS)
        cout << temp_ + " transactions.\n";
    if (system_flags & FILE_SUPPORTS_USN_JOURNAL)
        cout << temp_ + " update sequence number (USN) journals.\n";
    if (system_flags & FILE_UNICODE_ON_DISK)
        cout << temp_ + " Unicode in file names as they appear on disk.\n";
    if (system_flags & FILE_VOLUME_IS_COMPRESSED)
        cout << temp + " is a compressed volume, for example, a DoubleSpace volume.\n";
    if (system_flags & FILE_VOLUME_QUOTAS)
        cout << temp_ + " disk quotas.\n";

    DWORD sectors_per_cluster;
    DWORD bytes_per_sector;
    DWORD free_clusters;
    DWORD total_clusters;
    GetDiskFreeSpace(drive.c_str(), &sectors_per_cluster, &bytes_per_sector, &free_clusters, &total_clusters);
    unsigned long long free = free_clusters * sectors_per_cluster / 1024 * bytes_per_sector / 1024;
    unsigned long long total = total_clusters* sectors_per_cluster / 1024 * bytes_per_sector / 1024;

    cout << "Number of sectors in a cluster: " << sectors_per_cluster << '\n';
    cout << "The number of bytes in the sector: "  << bytes_per_sector << '\n';
    cout << "Number of free clusters: " << free_clusters << '\n';
    cout << "Total number of clusters: " << total_clusters << '\n';
    cout << "Disk space (free/total): " << free << " / " << total << " MiB\n";
}

bool isDirectoryExists(const char* filename)
{
    DWORD dwFileAttributes = GetFileAttributes(filename);
    if (dwFileAttributes == 0xFFFFFFFF)
        return false;
    return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

void createFolder()
{
    string directory_name;
    cout << "Enter the folder name: ";
    cin >> directory_name;
    if (CreateDirectory(directory_name.c_str(), NULL) != 0)
        cout << "The folder has been created\n";
    else
        cout << "Error, the folder was not created!\n";
}

void deleteFolder()
{
    string directory_name;
    cout << "Enter the folder name: ";
    cin >> directory_name;
    if (isDirectoryExists(directory_name.c_str()))
	{
        if (RemoveDirectory(directory_name.c_str()) != 0)
            cout << "The folder has been deleted\n";
        else
            cout << "Error, the folder was not deleted!\n";
    }
    else
        cout << "There is no such folder!\n";
}

void createFile()
{
    string file_name;
    cout << "Enter the file name: ";
    cin >> file_name;
    HANDLE hfile = CreateFile(file_name.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile != 0)
        cout << "The file has been created\n";
    else
        cout << "Error, the file was not created!\n";
    CloseHandle(hfile);
}

void copyFile()
{
	string source;
	string destination;

    cout << "Enter the path to the file: ";
    cin >> source;
    cout << "Enter the path where to copy the file: ";
    cin >> destination;

    if (CopyFile(source.c_str(), destination.c_str(), false) != 0)
        cout << "The file is copied!\n";
    else
        cout << "An error occurred, the file was not copied!\n";
}

void moveFile() {
    string source;
    string destination;

    cout << "Enter the path to the file: ";
    cin >> source;
    cout << "Enter the path where to move the file: ";
    cin >> destination;

    if (MoveFile(source.c_str(), destination.c_str()) != 0)
        cout << "The file has been moved!\n";
    else
        cout << "An error occurred, the file was not moved!\n";
}

void fileInfo() {
    string filename;
    cout << "Enter the file name: ";
    cin >> filename;

    HANDLE hfile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hfile != NULL)
    {
        BY_HANDLE_FILE_INFORMATION info;
        if(GetFileInformationByHandle(hfile, &info))
        {
            DWORD file_attributes = info.dwFileAttributes;

            cout << "Attributes: \n";
            if (file_attributes & FILE_ATTRIBUTE_ARCHIVE)
                cout<<"FILE_ATTRIBUTE_ARCHIVE:\nA file or directory that is an archive file or directory. Applications typically use this attribute to mark files for backup or removal.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_COMPRESSED)
                cout<<"FILE_ATTRIBUTE_COMPRESSED:\nA file or directory that is compressed. For a file, all of the data in the file is compressed. For a directory, compression is the default for newly created files and subdirectories.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_DEVICE)
                cout<<"FILE_ATTRIBUTE_DEVICE:\nThis value is reserved for system use.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_DIRECTORY)
                cout<<"FILE_ATTRIBUTE_DIRECTORY:\nThe handle that identifies a directory.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_ENCRYPTED)
                cout<<"FILE_ATTRIBUTE_ENCRYPTED:\nA file or directory that is encrypted. For a file, all data streams in the file are encrypted. For a directory, encryption is the default for newly created files and subdirectories.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_HIDDEN)
                cout<<"FILE_ATTRIBUTE_HIDDEN:\nThe file or directory is hidden. It is not included in an ordinary directory listing.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_NORMAL)
                cout<<"FILE_ATTRIBUTE_NORMAL:\nA file that does not have other attributes set. This attribute is valid only when used alone.\n";
            if (file_attributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
                cout<<"FILE_ATTRIBUTE_NOT_CONTENT_INDEXED:\nThe file or directory is not to be indexed by the content indexing service.\n";
            if (file_attributes & FILE_ATTRIBUTE_OFFLINE)
                cout<<"FILE_ATTRIBUTE_OFFLINE:\nThe data of a file is not available immediately. This attribute indicates that the file data is physically moved to offline storage. This attribute is used by Remote Storage,\
                         which is the hierarchical storage management software. Applications should not arbitrarily change this attribute.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_READONLY)
                cout<<"FILE_ATTRIBUTE_READONLY:\nA file that is read-only. Applications can read the file, but cannot write to it or delete it. This attribute is not honored on directories.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_REPARSE_POINT)
                cout<<"FILE_ATTRIBUTE_REPARSE_POINT:\nA file or directory that has an associated reparse point, or a file that is a symbolic link.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_SPARSE_FILE)
                cout<<"FILE_ATTRIBUTE_SPARSE_FILE:\nA file that is a sparse file.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_SYSTEM)
                cout<<"FILE_ATTRIBUTE_SYSTEM:\nA file or directory that the operating system uses a part of, or uses exclusively.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_TEMPORARY)
                cout<<"FILE_ATTRIBUTE_TEMPORARY:\nA file that is being used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application\
                         deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed.\n--\n";
            if (file_attributes & FILE_ATTRIBUTE_VIRTUAL)
                cout<<"FILE_ATTRIBUTE_VIRTUAL:\nThis value is reserved for system use.\n--\n";
        }

        FILETIME file_created_time;
        SYSTEMTIME file_created_system_time;
        char created_local_date[255];
        char created_local_time[255];
        FILETIME file_accessed_time;
        SYSTEMTIME file_accessed_system_time;
        char accessed_local_date[255];
        char accessed_local_time[255];
        FILETIME file_writed_time;
        SYSTEMTIME file_writed_system_time;
        char writed_local_date[255];
        char writed_local_time[255];

        if (GetFileTime(hfile, &file_created_time, &file_accessed_time, &file_writed_time) != 0)
        {
            FileTimeToLocalFileTime(&file_created_time, &file_created_time);
            FileTimeToLocalFileTime(&file_accessed_time, &file_accessed_time);
            FileTimeToLocalFileTime(&file_writed_time, &file_writed_time);

            FileTimeToSystemTime(&file_created_time, &file_created_system_time);
            FileTimeToSystemTime(&file_accessed_time, &file_accessed_system_time);
            FileTimeToSystemTime(&file_writed_time, &file_writed_system_time);

            GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &file_created_system_time, NULL, created_local_date, 255);
            GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &file_accessed_system_time, NULL, accessed_local_date, 255);
            GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &file_writed_system_time, NULL, writed_local_date, 255);

            GetTimeFormat(LOCALE_USER_DEFAULT, 0, &file_created_system_time, NULL, created_local_time, 255);
            GetTimeFormat(LOCALE_USER_DEFAULT, 0, &file_accessed_system_time, NULL, accessed_local_time, 255);
            GetTimeFormat(LOCALE_USER_DEFAULT, 0, &file_writed_system_time, NULL, writed_local_time, 255);

            cout << "Creation time: " << file_created_system_time.wDay << "." << file_created_system_time.wMonth << "."
                << file_created_system_time.wYear << " " << file_created_system_time.wHour << ":"
                << file_created_system_time.wMinute << "\n";
            cout << "Last request: " << file_accessed_system_time.wDay << "." << file_accessed_system_time.wMonth << "."
                << file_accessed_system_time.wYear << " " << file_accessed_system_time.wHour << ":"
                << file_accessed_system_time.wMinute << "\n";
            cout << "Last change: " << file_writed_system_time.wDay << "." << file_writed_system_time.wMonth << "."
                << file_writed_system_time.wYear << " " << file_writed_system_time.wHour << ":"
                << file_writed_system_time.wMinute << "\n";
        }

        cout << "\nVolume serial number: " << info.dwVolumeSerialNumber << endl
            << "Number of links: " << info.nNumberOfLinks << endl;
    }
    else
        cout << "Unable to get a file handler!\n";
}


void changeFileAttributes()
{
    string filename;
    cout << "Enter the file name: ";
    cin >> filename;
    DWORD attrs = GetFileAttributesA(filename.c_str());

    char answer;
    cout<<"Make it archived? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_ARCHIVE;
    else
        attrs &= ~FILE_ATTRIBUTE_ARCHIVE;

    cout<<"Make it invisible? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_HIDDEN;
    else
        attrs &= ~FILE_ATTRIBUTE_HIDDEN;

    cout<<"Make it ordinary? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_NORMAL;
    else
        attrs &= ~FILE_ATTRIBUTE_NORMAL;

    cout<<"Index the content? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    else
        attrs &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;

    cout<<"Is it available without a network? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_OFFLINE;
    else
        attrs &= ~FILE_ATTRIBUTE_OFFLINE;

    cout<<"Make it read-only? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_READONLY;
    else
        attrs &= ~FILE_ATTRIBUTE_READONLY;

    cout<<"Make it system-based? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_SYSTEM;
    else
        attrs &= ~FILE_ATTRIBUTE_SYSTEM;

    cout<<"Make it temporary? (y/n):";
    cin >> answer;
    if (answer == 'y')
        attrs |= FILE_ATTRIBUTE_TEMPORARY;
    else
        attrs &= ~FILE_ATTRIBUTE_TEMPORARY;

    if (SetFileAttributesA(filename.c_str(), attrs))
        cout<<"The attributes have been successfully installed! \n";
    else
        cout<<"An error occurred, the attributes were not set! \n";
}


void changeCreationTime()
{
    string filename;
    cout << "Enter the file name: ";
    cin >> filename;
    HANDLE hfile = CreateFile(filename.c_str(), FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);

    FILETIME file_time;
    SYSTEMTIME system_time_now;
    GetSystemTime(&system_time_now);
    SystemTimeToFileTime(&system_time_now, &file_time);

    if (SetFileTime(hfile, &file_time, NULL, NULL))
        cout << "The time was successfully set\n"<< system_time_now.wDay<<"." << system_time_now.wMonth << "."
        << system_time_now.wYear << " " << system_time_now.wHour+3 << ":" << system_time_now.wMinute << "\n";
    else
        cout << "An error occurred, the time could not be set\n";

    CloseHandle(hfile);
}


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
			fileInfo();
			break;
		case 9:
			changeFileAttributes();
			break;
		case 10:
			changeCreationTime();
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
