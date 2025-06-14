#ifndef OS_COMPATIBILITY_LAYER_H
#define OS_COMPATIBILITY_LAYER_H



// A module to abstract many Operating System dependent functions. To use it, link OSCommonLayer.cpp and either WinUtils.cpp or LinuxUtils.cpp, depending on
// your system (MacUtils.cpp is waiting on a friendly Apple developer to write it).



#include "Log.h"
#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <string>



namespace commonItems
{

//
// Directory search functions
//

// Confirms the specified file exists (and is not a folder).
bool DoesFileExist(const std::filesystem::path& path);

// Confirms the specified folder exists (and is actually a folder).
bool DoesFolderExist(const std::filesystem::path& path);

// Returns the names of all files in the specified folder.
std::set<std::filesystem::path> GetAllFilesInFolder(const std::filesystem::path& path);

// Returns the names of all subfolders in the specified folder.
std::set<std::filesystem::path> GetAllSubfolders(const std::filesystem::path& path);

// Returns the names of all files in the specified folder and all its subfolders.
std::set<std::filesystem::path> GetAllFilesInFolderRecursive(const std::filesystem::path& path);

// Given a Steam AppId, returns the install path for the corresponding game.
std::optional<std::filesystem::path> getSteamInstallPath(const std::string& steamID);


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

// Converts an UTF8 path to the system dependent filesystem path encoding and removes non - allowed characters.
std::string normalizeUTF8Path(const std::string& utf_8_path);

} // namespace commonItems



// Linux specific defines
#ifdef __linux

// sprintf_s is not implemented on Linux. It functions similarly to the function snprintf, but the return values are
// different. Since return values are not used, they are directly translated for now. If return values will some day be
// used, the compiler will give an error due to sprintf_s_Linux returning void.
void sprintf_s_Linux(char* __restrict __s, size_t __maxlength, const char* __restrict __format, ...);
const auto sprintf_s = sprintf_s_Linux;

// See sprintf_s_Linux
void strcpy_s_Linux(char* __restrict __dest, const char* __restrict __src);
const auto strcpy_s = strcpy_s_Linux;

// Very basic implementation, simply returns 0 if FILE* is not NULL
int fopen_s_Linux(FILE** file, const char* filename, const char* mode);
const auto fopen_s = fopen_s_Linux;

void fprintf_s_Linux(FILE* file, const char* format, ...);
const auto fprintf_s = fprintf_s_Linux;

using HANDLE = uint64_t;
constexpr int STD_OUTPUT_HANDLE = -11;
constexpr int INVALID_HANDLE_VALUE = -1;
HANDLE GetStdHandle(int nothing);

using errno_t = int;

#endif //__linux__



#endif // OS_COMPATIBILITY_LAYER_H
