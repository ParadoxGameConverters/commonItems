#ifndef OS_COMPATABILITY_LAYER_H
#define OS_COMPATABILITY_LAYER_H

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



// Linux specific defines
#ifdef __linux

// sprintf_s is not implemented on Linux. It functions similarly to the function snprintf, but the return values are
// different. Since return values are not used, they are directly translated for now. If return values will some day be
// used, the compiler will give an error due to sprintf_s_Linux returning void.
#define sprintf_s sprintf_s_Linux
void sprintf_s_Linux(char* __restrict __s, size_t __maxlen, const char* __restrict __format, ...);

// See sprintf_s_LInux
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

namespace Utils
{

// Creates a new folder corresponding to the given path.
// Returns true on success or if the folder already exists.
// Returns false and logs a warning on failure.
bool TryCreateFolder(const std::string& path);
std::string GetCurrentDirectory();
// Adds all files (just the file name) in the specified folder to the given collection.
void GetAllFilesInFolder(const std::string& path, std::set<std::string>& fileNames);
void GetAllSubfolders(const std::string& path, std::set<std::string>& subfolders);

// For the specified folder and all subfolders, adds all files (just the subdirectory and file name)
// to the given collection.
void GetAllFilesInFolderRecursive(const std::string& path, std::set<std::string>& filenames);
// Copies the file specified by sourcePath as destPath.
// Returns true on success.
// Returns false and logs a warning on failure.
bool TryCopyFile(const std::string& sourcePath, const std::string& destPath);
bool CopyFolder(const std::string& sourceFolder, const std::string& destFolder);
bool RenameFolder(const std::string& sourceFolder, const std::string& destFolder);
// Returns true if the specified file exists (and is a file rather than a folder).
bool DoesFileExist(const std::string& path);
// Returns true if the specified folder exists (and is a folder rather than a file).
bool DoesFolderExist(const std::string& path);

void WriteToConsole(LogLevel level, const std::string& logMessage);

// Returns a formatted string describing the last error on the WinAPI.
std::string GetLastErrorString();

bool DeleteFolder(const std::string& folder);


std::optional<std::string> GetFileFromTag(const std::string& directoryPath, const std::string& tag);

std::string convertUTF8ToASCII(const std::string& UTF8);
std::string convertUTF8To8859_15(const std::string& UTF8);
std::string convertUTF8ToWin1252(const std::string& UTF8);
std::string convertUTF16ToUTF8(const std::wstring& UTF16);
std::string convert8859_15ToASCII(const std::string& input);
std::string convert8859_15ToUTF8(const std::string& input);
std::wstring convert8859_15ToUTF16(const std::string& input);
std::string convertWin1252ToASCII(const std::string& Win1252);
std::string convertWin1252ToUTF8(const std::string& Win1252);
std::string convertWin1250ToUTF8(const std::string& Win1252);
std::wstring convertWin1252ToUTF16(const std::string& Win1252);
std::wstring convertWin1250ToUTF16(const std::string& Win1250);
std::wstring convertUTF8ToUTF16(const std::string& UTF8);
std::string convertWin1251ToUTF8(const std::string& Win1251);
std::string convertUTF8toWin1251(const std::string& UTF8);

// converts a string in the system dependent wchar_t encoding to UTF-8
std::string convertToUTF8(const std::wstring& input);

// converts an UTF8 path to the system dependent filesystem path encoding
std::string normalizeUTF8Path(const std::string& utf_8_path);

} // namespace Utils


#endif // OS_COMPATABILITY_LAYER_H
