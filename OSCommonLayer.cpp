#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <codecvt>
#include <filesystem>

namespace fs = std::filesystem;

namespace commonItems
{

bool TryCreateFolder(const std::string& path)
{
	if (fs::exists(fs::u8path(path)))
		return true;
	if (fs::create_directory(fs::u8path(path)))
		return true;

	Log(LogLevel::Error) << "Could not create directory: " << path << " : " << GetLastErrorString();
	return false;
}

std::wstring GetCurrentDirectoryWString()
{
	// Tried straight returning wstring, but on Linux it will break if filesystem uses characters
	// outside ascii, apparently inherent conversion is broken.
	try
	{
		const auto path = fs::current_path().string();
		return convertUTF8ToUTF16(path);
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Error) << "Cannot determine current path; " << e.what();
		return {};
	}
}

std::set<std::string> GetAllFilesInFolder(const std::string& path)
{
	std::set<std::string> fileNames;
	if (!fs::exists(fs::u8path(path)))
		return fileNames;
	for (const auto& p: fs::directory_iterator(fs::u8path(path)))
	{
		if (!p.is_directory())
		{
			fileNames.insert(p.path().filename().string());
		}
	}
	return fileNames;
}

std::set<std::string> GetAllSubfolders(const std::string& path)
{
	std::set<std::string> subFolders;
	if (!fs::exists(fs::u8path(path)))
		return subFolders;
	for (const auto& p: fs::directory_iterator(fs::u8path(path)))
	{
		if (p.is_directory())
		{
			subFolders.insert(p.path().filename().string());
		}
	}
	return subFolders;
}

bool TryCopyFile(const std::string& sourcePath, const std::string& destPath)
{
	if (fs::copy_file(fs::u8path(sourcePath), fs::u8path(destPath), std::filesystem::copy_options::overwrite_existing))
		return true;
	LOG(LogLevel::Warning) << "Could not copy file " << sourcePath << " to " << destPath << " - " << GetLastErrorString();
	return false;
}

bool CopyFolder(const std::string& sourceFolder, const std::string& destFolder)
{
	try
	{
		fs::copy(fs::u8path(sourceFolder), fs::u8path(destFolder), fs::copy_options::recursive);
		return true;
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Error) << "Could not copy folder: " << e.what() << " " << GetLastErrorString();
		return false;
	}
}


bool RenameFolder(const std::string& sourceFolder, const std::string& destFolder)
{
	try
	{
		fs::rename(fs::u8path(sourceFolder), fs::u8path(destFolder));
		return true;
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Error) << "Could not rename folder: " << e.what() << " " << GetLastErrorString();
		return false;
	}
}

bool DoesFileExist(const std::string& path)
{
	if (const auto tempPath = fs::u8path(path); fs::exists(tempPath) && !fs::is_directory(tempPath))
		return true;
	return false;
}


bool DoesFolderExist(const std::string& path)
{
	if (const auto tempPath = fs::u8path(path); fs::exists(tempPath) && fs::is_directory(tempPath))
		return true;
	return false;
}

bool DeleteFolder(const std::string& folder)
{
	if (!fs::exists(fs::u8path(folder)))
		return true;
	if (fs::remove_all(fs::u8path(folder)) != static_cast<std::uintmax_t>(-1))
		return true;
	Log(LogLevel::Error) << "Could not delete folder: " << folder << " " << GetLastErrorString();
	return false;
}

std::string normalizeUTF8Path(const std::string& utf_8_path)
{
	std::string asciiPath = convertUTF8ToASCII(utf_8_path);
	std::ranges::replace(asciiPath, '/', '_');
	std::ranges::replace(asciiPath, '\\', '_');
	std::ranges::replace(asciiPath, ':', '_');
	std::ranges::replace(asciiPath, '*', '_');
	std::ranges::replace(asciiPath, '?', '_');
	std::ranges::replace(asciiPath, '\"', '_');
	std::ranges::replace(asciiPath, '<', '_');
	std::ranges::replace(asciiPath, '>', '_');
	std::ranges::replace(asciiPath, '|', '_');
	asciiPath.erase(std::ranges::remove(asciiPath, '\t').begin(), asciiPath.end());
	asciiPath.erase(std::ranges::remove(asciiPath, '\n').begin(), asciiPath.end());
	asciiPath.erase(std::ranges::remove(asciiPath, '\r').begin(), asciiPath.end());

	return asciiPath;
}

#if _MSC_VER >= 1900 && _MSC_VER < 1920
std::string utf16_to_utf8(std::u16string utf16_string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> conversion;
	auto p = reinterpret_cast<const int16_t*>(utf16_string.data());
	return conversion.to_bytes(p, p + utf16_string.size());
}
#else
std::string utf16_to_utf8(const std::u16string& utf16_string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conversion;
	return conversion.to_bytes(utf16_string);
}
#endif


std::string UTF16ToUTF8(const std::wstring& UTF16)
{
	const std::u16string u16str(UTF16.begin(), UTF16.end());
	return utf16_to_utf8(u16str);
}

} // namespace commonItems
