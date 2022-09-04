#ifndef OS_COMPATIBILITY_LAYER_H
#define OS_COMPATIBILITY_LAYER_H



// A module to abstract many Operating System dependent functions. To use it, link OSCommonLayer.cpp and either WinUtils.cpp or LinuxUtils.cpp, depending on
// your system (MacUtils.cpp is waiting on a friendly Apple developer to write it).



#include "Log.h"
#include "iconvlite.h"
#include <cstdio>
#include <optional>
#include <set>
#include <stdint.h>
#include <string>



#ifndef MAX_PATH
#define MAX_PATH 260
#endif // MAX_PATH



namespace commonItems
{

//
// Directory search functions
//

// Confirms the specified file exists (and is not a folder).
bool DoesFileExist(const std::string& path);

// Confirms the specified folder exists (and is actually a folder).
bool DoesFolderExist(const std::string& path);

// Returns the names of all files in the specified folder.
std::set<std::string> GetAllFilesInFolder(const std::string& path);
[[deprecated("Use std::set<std::string> GetAllFilesInFolder(const std::string& path)")]] inline void GetAllFilesInFolder(const std::string& path,
	 std::set<std::string>& fileNames)
{
	fileNames = GetAllFilesInFolder(path);
}

// Returns the names of all subfolders in the specified folder.
std::set<std::string> GetAllSubfolders(const std::string& path);
[[deprecated("Use std::set<std::string> GetAllSubfolders(const std::string& path)")]] inline void GetAllSubfolders(const std::string& path,
	 std::set<std::string>& subFolders)
{
	subFolders = GetAllSubfolders(path);
}

// Returns the names of all files in the specified folder and all its subfolders.
std::set<std::string> GetAllFilesInFolderRecursive(const std::string& path);
[[deprecated("Use std::set<std::string> GetAllFilesInFolderRecursive(const std::string& path)")]] inline void GetAllFilesInFolderRecursive(
	 const std::string& path,
	 std::set<std::string>& fileNames)
{
	fileNames = GetAllFilesInFolderRecursive(path);
}

// Returns the current directory in UTF-16.
std::wstring GetCurrentDirectoryWString();

// Given a Steam AppId, returns the install path for the corresponding game.
std::optional<std::wstring> getSteamInstallPath(const std::string& steamID);


//
// Directory modification functions
//

// Attempts to create the specified directory.
bool TryCreateFolder(const std::string& path);

// Attempts to copy the specified file to the specified location, overwriting any existing file.
bool TryCopyFile(const std::string& sourcePath, const std::string& destPath);

// Attempts to recursively copy to specified folder to the specified location.
bool CopyFolder(const std::string& sourceFolder, const std::string& destFolder);

// Attempts to rename the specified folder to the specified name.
bool RenameFolder(const std::string& sourceFolder, const std::string& destFolder);

// Attempt to delete the specified folder (and everything in it).
bool DeleteFolder(const std::string& folder);


//
// Assorted functions
//

// Writes the specified message to the console at the specified log level. On Windows this colors the message appropriately.
void WriteToConsole(LogLevel level, const std::string& logMessage);

// Get the latest global error as a message.
std::string GetLastErrorString();


//
// Encoding conversion functions
//

// Attempt to convert the specified text from the first encoding to the second encoding.
std::string convertUTF8ToASCII(const std::string& UTF8);
std::string convertUTF8To8859_15(const std::string& UTF8);
std::string convertUTF8ToWin1252(const std::string& UTF8);
std::string UTF16ToUTF8(const std::wstring& UTF16);
[[deprecated("Use UTF16ToUTF8")]] inline std::string convertUTF16ToUTF8(const std::wstring& UTF16)
{
	return UTF16ToUTF8(UTF16);
}
std::string convert8859_15ToASCII(const std::string& input);
std::string convert8859_15ToUTF8(const std::string& input);
std::wstring convert8859_15ToUTF16(const std::string& input);
std::string convertWin1252ToASCII(const std::string& Win1252);
std::string convertWin1252ToUTF8(const std::string& Win1252);
std::string convertWin1250ToUTF8(const std::string& Win1250);
std::wstring convertWin1252ToUTF16(const std::string& Win1252);
std::wstring convertWin1250ToUTF16(const std::string& Win1250);
std::wstring convertUTF8ToUTF16(const std::string& UTF8);
std::string convertWin1251ToUTF8(const std::string& Win1251);
std::string convertUTF8toWin1251(const std::string& UTF8);
std::string convertUTF8ToWin1251(const std::string& UTF8);
std::string convertUTF8ToWin1250(const std::string& UTF8);

// converts a string in the system dependent wchar_t encoding to UTF-8
std::string convertToUTF8(const std::wstring& input);

// Converts an UTF8 path to the system dependent filesystem path encodingand removes non - allowed characters.
std::string normalizeUTF8Path(const std::string& utf_8_path);

} // namespace commonItems



// Linux specific defines
#ifdef __linux

// sprintf_s is not implemented on Linux. It functions similarly to the function snprintf, but the return values are
// different. Since return values are not used, they are directly translated for now. If return values will some day be
// used, the compiler will give an error due to sprintf_s_Linux returning void.
#define sprintf_s sprintf_s_Linux
void sprintf_s_Linux(char* __restrict __s, size_t __maxlen, const char* __restrict __format, ...);

// See sprintf_s_Linux
#define strcpy_s strcpy_s_Linux
void strcpy_s_Linux(char* __restrict __dest, const char* __restrict __src);

// Very basic implementation, simply returns 0 if FILE* is not NULL
#define fopen_s fopen_s_Linux
int fopen_s_Linux(FILE** file, const char* filename, const char* mode);

#define fprintf_s fprintf_s_Linux
void fprintf_s_Linux(FILE* file, const char* format, ...);

typedef uint64_t HANDLE;
#define STD_OUTPUT_HANDLE -11
#define INVALID_HANDLE_VALUE -1
HANDLE GetStdHandle(int nothing);

typedef int errno_t;

#endif //__linux__



#endif // OS_COMPATIBILITY_LAYER_H
