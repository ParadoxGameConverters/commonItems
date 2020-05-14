#include "OSCompatibilityLayer.h"
#include <Windows.h>
#include <iostream>
#include <Shellapi.h>
#include <list>
#include <algorithm>
#include "Log.h"
#include <filesystem>
namespace fs = std::filesystem;



#pragma warning(disable: 4996)	// supress warnings about wcscmp()



namespace Utils
{



bool TryCreateFolder(const std::string& path)
{
	BOOL success = ::CreateDirectoryW(convertUTF8ToUTF16(path).c_str(), NULL);
	if (success || GetLastError() == 183)	// 183 is if the folder already exists
	{
		return true;
	}
	else
	{
		Log(LogLevel::Warning) << "Could not create folder " << path << " - " << GetLastErrorString();
		return false;
	}
}


std::string getCurrentDirectory()
{
	wchar_t curDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curDir);
	return Utils::convertUTF16ToUTF8(curDir);
}


void GetAllFilesInFolder(const std::string& path, std::set<std::string>& fileNames)
{
	if (!exists(fs::u8path(path))) return;
	if (fs::is_empty(fs::u8path(path))) return;
	for (auto& p : fs::directory_iterator(fs::u8path(path)))
	{
		if (!p.is_directory())
		{
			fileNames.insert(p.path().filename().string());
		}
			
	}
}


void GetAllSubfolders(const std::string& path, std::set<std::string>& subfolders)
{
	WIN32_FIND_DATA findData;
	HANDLE findHandle = FindFirstFileW(convertUTF8ToUTF16(path + "/*").c_str(), &findData);
	if (findHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			subfolders.insert(convertUTF16ToUTF8(findData.cFileName));
		}
	} while (FindNextFileW(findHandle, &findData) != 0);
	FindClose(findHandle);
}


void GetAllFilesInFolderRecursive(const std::string& path, std::set<std::string>& filenames)
{
	for (auto& p : fs::recursive_directory_iterator(fs::u8path(path)))
	{
		if (!p.is_directory())
		{
			auto lastSlash = p.path().native().find_last_of(L"\\");
			auto tempDir = p.path().native().substr(0, lastSlash);
			lastSlash = tempDir.find_last_of(L"\\");
			auto dirName = tempDir.substr(lastSlash + 1, tempDir.length());
			auto returnName = "/" + convertUTF16ToUTF8(dirName) + "/" + p.path().filename().string();
			filenames.insert(returnName);
		}
	}
}


bool TryCopyFile(const std::string& sourcePath, const std::string& destPath)
{
    const auto success = fs::copy_file(fs::u8path(sourcePath), fs::u8path(destPath));    
    if (success) return true;
        LOG(LogLevel::Warning) << "Could not copy file " << sourcePath << " to " << destPath << " - " << GetLastErrorString();
    return false;
}


bool copyFolder(const std::string& sourceFolder, const std::string& destFolder)
{
	std::wstring wideSource = convertUTF8ToUTF16(sourceFolder);
	wchar_t* from = new wchar_t[wideSource.size() + 2];
	wcscpy(from, wideSource.c_str());
	from[wideSource.size() + 1] = '\0';

	std::wstring wideDest = convertUTF8ToUTF16(destFolder);
	wchar_t* to = new wchar_t[wideDest.size() + 1];
	wcscpy(to, wideDest.c_str());
	to[wideDest.size() + 1] = '\0';

	SHFILEOPSTRUCT fileOptStruct;
	fileOptStruct.hwnd = NULL;
	fileOptStruct.wFunc = FO_COPY;
	fileOptStruct.pFrom = from;
	fileOptStruct.pTo = to;
	fileOptStruct.fFlags	= FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

	const int result = SHFileOperation(&fileOptStruct);
	if (result != 0)
	{
		Log(LogLevel::Error) << "Could not copy " << sourceFolder << " to " << destFolder << ". Error code: " << result;
	}
	else if (fileOptStruct.fAnyOperationsAborted)
	{
		Log(LogLevel::Error) << "Could not copy " << sourceFolder << " to " << destFolder << ". Operation aborted";
	}

	delete[] from;
	delete[] to;
	return true;
}


bool renameFolder(const std::string& sourceFolder, const std::string& destFolder)
{
	std::wstring wideSource = convertUTF8ToUTF16(sourceFolder);
	wchar_t* from = new wchar_t[wideSource.size() + 2];
	wcscpy(from, wideSource.c_str());
	from[wideSource.size() + 1] = '\0';

	std::wstring wideDest = convertUTF8ToUTF16(destFolder);
	wchar_t* to = new wchar_t[wideDest.size() + 1];
	wcscpy(to, wideDest.c_str());
	to[wideDest.size() + 1] = '\0';

	SHFILEOPSTRUCT fileOptStruct;
	fileOptStruct.hwnd = NULL;
	fileOptStruct.wFunc = FO_MOVE;
	fileOptStruct.pFrom = from;
	fileOptStruct.pTo = to;
	fileOptStruct.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

	int result = SHFileOperation(&fileOptStruct);
	if (result != 0)
	{
		Log(LogLevel::Error) << "Could not rename " << sourceFolder << " to " << destFolder << ". Error code: " << result;
		return false;
	}
	else if (fileOptStruct.fAnyOperationsAborted)
	{
		Log(LogLevel::Error) << "Could not rename " << sourceFolder << " to " << destFolder << ". Operation aborted.";
		return false;
	}

	delete[] from;
	delete[] to;
	return doesFolderExist(destFolder);
}


bool DoesFileExist(const std::string& path)
{
	const auto tempPath = fs::u8path(path);
	if (exists(tempPath) && !is_directory(tempPath)) return true;
	return false;
}


bool doesFolderExist(const std::string& path)
{
	const auto tempPath = fs::u8path(path);
	if (exists(tempPath) && is_directory(tempPath)) return true;
	return false;
}


std::string GetLastErrorString()
{
	const DWORD errorCode = ::GetLastError();
	const DWORD errorBufferSize = 256;
	wchar_t errorBuffer[errorBufferSize];
	const BOOL success = ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errorBuffer,
		errorBufferSize - 1,
		NULL);
	if (success)
	{
		return convertUTF16ToUTF8(errorBuffer);
	}
	else
	{
		return "Unknown error";
	}
}


bool deleteFolder(const std::string& folder)
{
	std::wstring wideFolder = convertUTF8ToUTF16(folder);
	wchar_t* folderStr = new wchar_t[wideFolder.size() + 2];
	wcscpy(folderStr, wideFolder.c_str());
	folderStr[wideFolder.size() + 1] = '\0';

	SHFILEOPSTRUCT fileOptStruct;
	fileOptStruct.hwnd	= NULL;
	fileOptStruct.wFunc	= FO_DELETE;
	fileOptStruct.pFrom	= folderStr;
	fileOptStruct.fFlags	= FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

	const int result = SHFileOperation(&fileOptStruct);
	if (result != 0)
	{
		Log(LogLevel::Error) << "Could not delete " << folder << ". Error code: " << result;
		return false;
	}
	else if (fileOptStruct.fAnyOperationsAborted)
	{
		Log(LogLevel::Error) << "Could not delete " << folder << ". Operation aborted.";
		return false;
	}

	delete[] folderStr;
	return true;
}


std::string convertUTF8ToASCII(const std::string& UTF8)
{
	int requiredSize = WideCharToMultiByte(20127 /*US-ASCII (7-bit)*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, NULL, 0, "0", NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(20127 /*US-ASCII (7-bit)*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8To8859_15(const std::string& UTF8)
{
	int requiredSize = WideCharToMultiByte(28605 /*8859-15*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, NULL, 0, "0", NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(28605 /*8859-15*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8ToWin1252(const std::string& UTF8)
{
	int requiredSize = WideCharToMultiByte(1252, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, NULL, 0, "0", NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(1252, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF16ToUTF8(const std::wstring& UTF16)
{
	const int requiredSize = WideCharToMultiByte(CP_UTF8, 0, UTF16.c_str(), -1, NULL, 0, NULL, NULL);
	char* utf8array = new char[requiredSize];

	if (0 == WideCharToMultiByte(CP_UTF8, 0, UTF16.c_str(), -1, utf8array, requiredSize, NULL, NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-8 - " << GetLastErrorString();
	}
	std::string returnable(utf8array);

	delete[] utf8array;

	return returnable;
}


std::string convert8859_15ToASCII(const std::string& input)
{
	return Utils::convertUTF8ToASCII(Utils::convert8859_15ToUTF8(input));
}


std::string convert8859_15ToUTF8(const std::string& input)
{
	return convertUTF16ToUTF8(convert8859_15ToUTF16(input));
}


std::wstring convert8859_15ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(28605 /* 8859-15*/, MB_PRECOMPOSED, input.c_str(), -1, NULL, 0);
	wchar_t* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(28605 /* 8859-15*/, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::string convertWin1252ToASCII(const std::string& input)
{
	return Utils::convertUTF8ToASCII(Utils::convertWin1252ToUTF8(input));
}


std::string convertWin1252ToUTF8(const std::string& input)
{
	return convertUTF16ToUTF8(convertWin1252ToUTF16(input));
}

std::string convertWin1250ToUTF8(const std::string& input)
{
	return convertUTF16ToUTF8(convertWin1250ToUTF16(input));
}

std::wstring convertWin1250ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(1250, MB_PRECOMPOSED, input.c_str(), -1, NULL, 0);
	wchar_t* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(1250, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}

std::wstring convertWin1252ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(1252, MB_PRECOMPOSED, input.c_str(), -1, NULL, 0);
	wchar_t* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(1252, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::wstring convertUTF8ToUTF16(const std::string& UTF8)
{
	const int requiredSize = MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, NULL, 0);
	if (requiredSize == 0)
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	wchar_t* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::string convertToUTF8(const std::wstring& input)
{
	return convertUTF16ToUTF8(input);
}


std::string normalizeUTF8Path(const std::string& utf_8_path)
{
	std::string asciiPath = convertUTF8ToASCII(utf_8_path);
	std::replace(asciiPath.begin(), asciiPath.end(), '/', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '\\', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), ':', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '*', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '?', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '\"', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '<', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '>', '_');
	std::replace(asciiPath.begin(), asciiPath.end(), '|', '_');
	asciiPath.erase(std::remove(asciiPath.begin(), asciiPath.end(), '\t'), asciiPath.end());

	return asciiPath;
};


void WriteToConsole(LogLevel level, const std::string& logMessage)
{
	if (level == LogLevel::Debug)
	{	// Don't log debug messages to console.
		return;
	}

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);	// a handle to the console window
	if (console != INVALID_HANDLE_VALUE)
	{
		CONSOLE_SCREEN_BUFFER_INFO oldConsoleInfo;	// the current (soon to be outdated) console data
		const BOOL success = GetConsoleScreenBufferInfo(console, &oldConsoleInfo);	// whether or not the console data could be retrieved
		if (success)
		{
			WORD color = 0x0;
			switch (level)
			{
				case LogLevel::Error:
					color = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;

				case LogLevel::Warning:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
					break;

				case LogLevel::Info:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
					break;

				case LogLevel::Debug:
				default:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
					break;
			}
			SetConsoleTextAttribute(console, color);
			DWORD bytesWritten = 0;
			WriteConsoleW(console, Utils::convertUTF8ToUTF16(logMessage).c_str(), logMessage.size(), &bytesWritten, NULL);

			// Restore old console color.
			SetConsoleTextAttribute(console, oldConsoleInfo.wAttributes);

			return;
		}
	}

	std::cout << logMessage;
}

std::string convertWin1251ToUTF8(const std::string& Win1251)
{
	return cp2utf(Win1251);
}

std::string convertUTF8toWin1251(const std::string& UTF8)
{
	return utf2cp(UTF8);
}

} // namespace Utils
