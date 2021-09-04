#include "files.h"

using namespace std;

void showDrivesList()
{
    int n;
    char drive_letter;
    DWORD drives = GetLogicalDrives();

    cout << "List of logical disks:\n";

    for (int i = 0; i < 26; i++) // подумай, надо ли менять
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
            cout << "]Remote drive";
            break;
        case DRIVE_CDROM:
            cout << "CDROM";
            break;
        case DRIVE_RAMDISK:
            cout << "RAM disk";
            break;
    }
    cout << endl;

    char volume_name_buffer[255];
    volume_name_buffer[0] = 0;
    char filesystem_name_buffer[100];
    filesystem_name_buffer[0] = 0;
    DWORD max_component_length = 0;
		DWORD system_flags = 0;
    unsigned long drive_serial_number = 0;

    GetVolumeInformation(drive.c_str(), volume_name_buffer, 255,
		&drive_serial_number, &max_component_length, &system_flags,
		filesystem_name_buffer, 100);

    cout << "Disk name: " << volume_name_buffer << endl <<
        "Serial number: " << drive_serial_number << endl <<
        "File system type: " << filesystem_name_buffer << endl <<
	      "System flags: " << endl;

    string TSV = "The specified volume";
    string TSVS = TSV +" supports";

    if (system_flags & FILE_CASE_PRESERVED_NAMES)
        cout<<TSVS+" preserved case of file names when it places a name on disk.\n";
    if (system_flags & FILE_CASE_SENSITIVE_SEARCH)
        cout<< TSVS + " case-sensitive file names.\n";
    if (system_flags & FILE_FILE_COMPRESSION)
        cout << TSVS + " file-based compression.\n";
    if (system_flags & FILE_NAMED_STREAMS)
        cout << TSVS + " named streams.\n";
    if (system_flags & FILE_PERSISTENT_ACLS)
        cout << TSV + " preserves and enforces access control lists (ACL). For example, the NTFS file system preserves and enforces ACLs, and the FAT file system does not.\n";
    if (system_flags & FILE_READ_ONLY_VOLUME)
        cout << TSV + " is read-only.\n";
    if (system_flags & FILE_SEQUENTIAL_WRITE_ONCE)
        cout << TSVS + " a single sequential write.\n";
    if (system_flags & FILE_SUPPORTS_ENCRYPTION)
        cout << TSVS + " the Encrypted File System (EFS).\n";
    if (system_flags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
        cout << TSVS + " extended attributes.\n";
    if (system_flags & FILE_SUPPORTS_HARD_LINKS)
        cout << TSVS + " hard links. \n";
    if (system_flags & FILE_SUPPORTS_OBJECT_IDS)
        cout << TSVS + " object identifiers.\n";
    if (system_flags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
        cout << TSVS + " open by FileID.\n";
    if (system_flags & FILE_SUPPORTS_REPARSE_POINTS)
        cout << TSVS + " reparse points.\n";
    if (system_flags & FILE_SUPPORTS_SPARSE_FILES)
        cout << TSVS + " sparse files.\n";
    if (system_flags & FILE_SUPPORTS_TRANSACTIONS)
        cout << TSVS + " transactions.\n";
    if (system_flags & FILE_SUPPORTS_USN_JOURNAL)
        cout << TSVS + " update sequence number (USN) journals.\n";
    if (system_flags & FILE_UNICODE_ON_DISK)
        cout << TSVS + " Unicode in file names as they appear on disk.\n";
    if (system_flags & FILE_VOLUME_IS_COMPRESSED)
        cout << TSV + " is a compressed volume, for example, a DoubleSpace volume.\n";
    if (system_flags & FILE_VOLUME_QUOTAS)
        cout << TSVS + " disk quotas.\n";

    // DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
    // GetDiskFreeSpaceA(drive_letter, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters);
    // unsigned __int64 free = freeClusters * sectorsPerCluster / 1024 * bytesPerSector / 1024;
    // unsigned __int64 total = totalClusters* sectorsPerCluster / 1024 * bytesPerSector / 1024;
    // cout << "Дисковое пространство (свободное/всего): " << free << " / " << total << " MiB\n";
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
    HANDLE hFile = CreateFile(file_name.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != 0)
        cout << "The file has been created\n";
    else
        cout << "Error, the file was not created!\n";
    CloseHandle(hFile);
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

// void fileInfo() {
//     wchar_t fileName[250];
//     char fileCharName[250];
//     cout << "Введите имя файла (латинскими буквами, без пробелов): ";
//     cin >> fileCharName;
//     mbstowcs(fileName, fileCharName, 250);
//     ostringstream tempStringStream;
//
//     DWORD fileAttributes;
//     fileAttributes = GetFileAttributes(fileName);
//     tempStringStream << "0x";
//     tempStringStream << hex << fileAttributes << "\n";
//     cout << "Аттрибуты: " << tempStringStream.str() << endl;
//     if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE)
//         cout<<"FILE_ATTRIBUTE_ARCHIVE:\nA file or directory that is an archive file or directory. Applications typically use this attribute to mark files for backup or removal.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_COMPRESSED)
//         cout<<"FILE_ATTRIBUTE_COMPRESSED:\nA file or directory that is compressed. For a file, all of the data in the file is compressed. For a directory, compression is the default for newly created files and subdirectories.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_DEVICE)
//         cout<<"FILE_ATTRIBUTE_DEVICE:\nThis value is reserved for system use.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//         cout<<"FILE_ATTRIBUTE_DIRECTORY:\nThe handle that identifies a directory.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
//         cout<<"FILE_ATTRIBUTE_ENCRYPTED:\nA file or directory that is encrypted. For a file, all data streams in the file are encrypted. For a directory, encryption is the default for newly created files and subdirectories.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_HIDDEN)
//         cout<<"FILE_ATTRIBUTE_HIDDEN:\nThe file or directory is hidden. It is not included in an ordinary directory listing.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
//         cout<<"FILE_ATTRIBUTE_INTEGRITY_STREAM:\nThe directory or user data stream is configured with integrity (only supported on ReFS volumes). It is not included in an ordinary directory listing. The integrity setting persists \
//              with the file if it's renamed. If a file is copied the destination file will have integrity set if either the source file or destination directory have integrity set.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_NORMAL)
//         cout<<"FILE_ATTRIBUTE_NORMAL:\nA file that does not have other attributes set. This attribute is valid only when used alone.\n";
//     if (fileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
//         cout<<"FILE_ATTRIBUTE_NOT_CONTENT_INDEXED:\nThe file or directory is not to be indexed by the content indexing service.\n";
//     if (fileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
//         cout<<"FILE_ATTRIBUTE_NO_SCRUB_DATA:\nThe user data stream not to be read by the background data integrity scanner (AKA scrubber). When set on a directory it only provides inheritance. This flag is only supported on\
//                 Storage Spaces and ReFS volumes. It is not included in an ordinary directory listing.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_OFFLINE)
//         cout<<"FILE_ATTRIBUTE_OFFLINE:\nThe data of a file is not available immediately. This attribute indicates that the file data is physically moved to offline storage. This attribute is used by Remote Storage,\
//                  which is the hierarchical storage management software. Applications should not arbitrarily change this attribute.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_READONLY)
//         cout<<"FILE_ATTRIBUTE_READONLY:\nA file that is read-only. Applications can read the file, but cannot write to it or delete it. This attribute is not honored on directories.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
//         cout<<"FILE_ATTRIBUTE_REPARSE_POINT:\nA file or directory that has an associated reparse point, or a file that is a symbolic link.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
//         cout<<"FILE_ATTRIBUTE_SPARSE_FILE:\nA file that is a sparse file.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_SYSTEM)
//         cout<<"FILE_ATTRIBUTE_SYSTEM:\nA file or directory that the operating system uses a part of, or uses exclusively.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_TEMPORARY)
//         cout<<"FILE_ATTRIBUTE_TEMPORARY:\nA file that is being used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application\
//                  deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed.\n--\n";
//     if (fileAttributes & FILE_ATTRIBUTE_VIRTUAL)
//         cout<<"FILE_ATTRIBUTE_VIRTUAL:\nThis value is reserved for system use.\n--\n";
//
//     FILE* pfile = fopen(fileCharName, "r");
//     HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(pfile));
//
//     if (hFile == NULL){
//         cout << "Невозможно получить обработчик файла!\n";
//         return;
//     }
//
//     FILETIME fileCreatedTime;
//     SYSTEMTIME fileCreatedSystemTime;
//     wchar_t createdLocalDate[255];
//     wchar_t createdLocalTime[255];
//     FILETIME fileAccessedTime;
//     SYSTEMTIME fileAccessedSystemTime;
//     wchar_t accessedLocalDate[255];
//     wchar_t accessedLocalTime[255];
//     FILETIME fileWritedTime;
//     SYSTEMTIME fileWritedSystemTime;
//     wchar_t writedLocalDate[255];
//     wchar_t writedLocalTime[255];
//
//     if (GetFileTime(hFile, &fileCreatedTime, &fileAccessedTime, &fileWritedTime) != 0) {
//         FileTimeToLocalFileTime(&fileCreatedTime, &fileCreatedTime);
//         FileTimeToLocalFileTime(&fileAccessedTime, &fileAccessedTime);
//         FileTimeToLocalFileTime(&fileWritedTime, &fileWritedTime);
//
//         FileTimeToSystemTime(&fileCreatedTime, &fileCreatedSystemTime);
//         FileTimeToSystemTime(&fileAccessedTime, &fileAccessedSystemTime);
//         FileTimeToSystemTime(&fileWritedTime, &fileWritedSystemTime);
//
//         GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fileCreatedSystemTime, NULL, createdLocalDate, 255);
//         GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fileAccessedSystemTime, NULL, accessedLocalDate, 255);
//         GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &fileWritedSystemTime, NULL, writedLocalDate, 255);
//
//         GetTimeFormat(LOCALE_USER_DEFAULT, 0, &fileCreatedSystemTime, NULL, createdLocalTime, 255);
//         GetTimeFormat(LOCALE_USER_DEFAULT, 0, &fileAccessedSystemTime, NULL, accessedLocalTime, 255);
//         GetTimeFormat(LOCALE_USER_DEFAULT, 0, &fileWritedSystemTime, NULL, writedLocalTime, 255);
//
//         cout << "Время создания: " << fileCreatedSystemTime.wDay << "." << fileCreatedSystemTime.wMonth << "."
//             << fileCreatedSystemTime.wYear << " " << fileCreatedSystemTime.wHour << ":"
//             << fileCreatedSystemTime.wMinute << "\n";
//         cout << "Последнее обращение: " << fileAccessedSystemTime.wDay << "." << fileAccessedSystemTime.wMonth << "."
//             << fileAccessedSystemTime.wYear << " " << fileAccessedSystemTime.wHour << ":"
//             << fileAccessedSystemTime.wMinute << "\n";
//         cout << "Последнее изменение: " << fileWritedSystemTime.wDay << "." << fileWritedSystemTime.wMonth << "."
//             << fileWritedSystemTime.wYear << " " << fileWritedSystemTime.wHour << ":"
//             << fileWritedSystemTime.wMinute << "\n";
//     }
//
//     BY_HANDLE_FILE_INFORMATION fileinfo;
//     if (GetFileInformationByHandle(hFile, &fileinfo)){
//         cout << "\nСерийный номер тома: " << fileinfo.dwVolumeSerialNumber << endl
//             << "Количество ссылок: " << fileinfo.nNumberOfLinks << endl;
//     }
//
//     fclose(pfile);
// }
// void changeFileAttributes() {
//     char fileName[250];
//     cout << "Введите имя файла (латинскими буквами, без пробелов): ";
//     cin >> fileName;
//     DWORD attrs = GetFileAttributesA(fileName);
//
//     char answer;
//
//     cout<<"Сделать архивным? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_ARCHIVE;
//     else
//         attrs &= ~FILE_ATTRIBUTE_ARCHIVE;
//     cout<<"Сделать невидимым? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_HIDDEN;
//     else
//         attrs &= ~FILE_ATTRIBUTE_HIDDEN;
//     cout<<"Сделать обычным? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_NORMAL;
//     else
//         attrs &= ~FILE_ATTRIBUTE_NORMAL;
//     cout<<"Индексировать содержание? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
//     else
//         attrs &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
//     cout<<"Доступен без сети? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_OFFLINE;
//     else
//         attrs &= ~FILE_ATTRIBUTE_OFFLINE;
//     cout<<"Сделать доступным только для чтения? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_READONLY;
//     else
//         attrs &= ~FILE_ATTRIBUTE_READONLY;
//     cout<<"Сделать системным? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_SYSTEM;
//     else
//         attrs &= ~FILE_ATTRIBUTE_SYSTEM;
//     cout<<"Сделать временным? (y/n):";
//     cin >> answer;
//     if (answer == 'y')
//         attrs |= FILE_ATTRIBUTE_TEMPORARY;
//     else
//         attrs &= ~FILE_ATTRIBUTE_TEMPORARY;
//
//     if (SetFileAttributesA(fileName, attrs))
//         cout<<"Аттрибуты успешно установленны!\n";
//     else
//         cout<<"Произошла ошибка, аттрибуты не были установлены!\n";
// }
//
// void changeCreationTime() {
//     wchar_t filename[250];
//     char fileCharName[250];
//     cout << "Введите имя файла: ";
//     cin >> fileCharName;
//     mbstowcs(filename, fileCharName, 250);
//     HANDLE hFile = CreateFile(filename, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
//
//     FILETIME fileTime;
//     SYSTEMTIME systemTimeNow;
//     GetSystemTime(&systemTimeNow);
//     SystemTimeToFileTime(&systemTimeNow, &fileTime);
//
//     if (SetFileTime(hFile, &fileTime, NULL, NULL))
//         cout << "Время успешно установленно\n"<<systemTimeNow.wDay<<"." << systemTimeNow.wMonth << "."
//         << systemTimeNow.wYear << " " << systemTimeNow.wHour+3 << ":" << systemTimeNow.wMinute << "\n";
//     else
//         cout << "Произошла ошибка, время установить не удалось\n";
//
//     CloseHandle(hFile);
// }
