#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "iconvlite.h"
#include <Windows.h>
#include <filesystem>
#include <iostream>



namespace fs = std::filesystem;



namespace commonItems
{

std::string GetLastErrorString()
{
	const DWORD errorCode = GetLastError();
	constexpr DWORD errorBufferSize = 256;
	if (wchar_t errorBuffer[errorBufferSize]; static_cast<bool>(FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
			  nullptr,
			  errorCode,
			  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			  errorBuffer,
			  errorBufferSize - 1,
			  nullptr)))
	{
		return UTF16ToUTF8(errorBuffer);
	}
	return "Unknown error";
}

std::string convertUTF8ToASCII(const std::string& UTF8)
{
	const int requiredSize = WideCharToMultiByte(20127 /*US-ASCII (7-bit)*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, nullptr, 0, "0", nullptr);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(20127 /*US-ASCII (7-bit)*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", nullptr))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8To8859_15(const std::string& UTF8)
{
	const int requiredSize = WideCharToMultiByte(28605 /*8859-15*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, nullptr, 0, "0", nullptr);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(28605 /*8859-15*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", nullptr))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8ToWin125_(const std::string& UTF8, const int codepage)
{
	const int requiredSize = WideCharToMultiByte(codepage, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, nullptr, 0, "0", nullptr);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(codepage, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", nullptr))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}

std::string convertUTF8ToWin1252(const std::string& UTF8)
{
	return convertUTF8ToWin125_(UTF8, 1252);
}


std::string UTF16ToUTF8(const std::wstring& UTF16)
{
	const int requiredSize = WideCharToMultiByte(CP_UTF8, 0, UTF16.c_str(), -1, nullptr, 0, NULL, NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(CP_UTF8, 0, UTF16.c_str(), -1, asciiArray, requiredSize, NULL, NULL))
	{
		// Can only return error code, as getting the string requires calling this function, possibly leading to a recursive error
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << std::to_string(GetLastError());
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8ToWin1251(const std::string& UTF8)
{
	return convertUTF8ToWin125_(UTF8, 1251);
}

std::string convertUTF8ToWin1250(const std::string& UTF8)
{
	return convertUTF8ToWin125_(UTF8, 1250);
}

std::string convert8859_15ToASCII(const std::string& input)
{
	return convertUTF8ToASCII(convert8859_15ToUTF8(input));
}


std::string convert8859_15ToUTF8(const std::string& input)
{
	return UTF16ToUTF8(convert8859_15ToUTF16(input));
}


std::wstring convert8859_15ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(28605 /* 8859-15*/, MB_PRECOMPOSED, input.c_str(), -1, nullptr, 0);
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(28605 /* 8859-15*/, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::string convertWin1252ToASCII(const std::string& Win1252)
{
	return convertUTF8ToASCII(convertWin1252ToUTF8(Win1252));
}


std::string convertWin1252ToUTF8(const std::string& Win1252)
{
	return UTF16ToUTF8(convertWin1252ToUTF16(Win1252));
}

std::string convertWin1250ToUTF8(const std::string& Win1250)
{
	return UTF16ToUTF8(convertWin1250ToUTF16(Win1250));
}

std::wstring convertWin1250ToUTF16(const std::string& Win1250)
{
	const int requiredSize = MultiByteToWideChar(1250, MB_PRECOMPOSED, Win1250.c_str(), -1, nullptr, 0);
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(1250, MB_PRECOMPOSED, Win1250.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}

std::wstring convertWin1252ToUTF16(const std::string& Win1252)
{
	const int requiredSize = MultiByteToWideChar(1252, MB_PRECOMPOSED, Win1252.c_str(), -1, nullptr, 0);
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(1252, MB_PRECOMPOSED, Win1252.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::wstring convertUTF8ToUTF16(const std::string& UTF8)
{
	const int requiredSize = MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, nullptr, 0);
	if (requiredSize == 0)
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	auto* wideKeyArray = new wchar_t[requiredSize];

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
	return UTF16ToUTF8(input);
}

void WriteToConsole(const LogLevel level, const std::string& logMessage)
{
	if (level == LogLevel::Debug)
	{ // Don't log debug messages to console.
		return;
	}

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // a handle to the console window
	if (console != INVALID_HANDLE_VALUE)
	{
		CONSOLE_SCREEN_BUFFER_INFO oldConsoleInfo; // the current (soon to be outdated) console data
		if (static_cast<bool>(GetConsoleScreenBufferInfo(console, &oldConsoleInfo)))
		{
			WORD color;
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
				case LogLevel::Notice:
				case LogLevel::Progress:
				default:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
					break;
			}
			SetConsoleTextAttribute(console, color);
			DWORD bytesWritten = 0;
			WriteConsoleW(console, convertUTF8ToUTF16(logMessage).c_str(), static_cast<DWORD>(logMessage.size()), &bytesWritten, nullptr);

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

std::optional<std::filesystem::path> getSteamInstallPath(const std::string& steamID)
{
	if (steamID.empty())
		return std::nullopt;

	wchar_t value[255];
	value[0] = 0;
	DWORD BufferSize = 8192;
	std::wstring registryPath = convertUTF8ToUTF16(R"(SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	const std::wstring installPath = convertUTF8ToUTF16(R"(InstallLocation)");

	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, static_cast<PVOID>(&value), &BufferSize);

	if (value[0] != 0)
	{
		if (auto result = std::wstring(value); result.length() > 2)
		{
			return result;
		}
	}

	registryPath = convertUTF8ToUTF16(R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, static_cast<PVOID>(&value), &BufferSize);

	if (value[0] != 0)
	{
		if (auto result = std::wstring(value); result.length() > 2)
		{
			return result;
		}
	}

	return std::nullopt;
}

} // namespace commonItems
