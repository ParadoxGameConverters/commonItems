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
#pragma warning(push)
#pragma warning(disable : 4996)
	if (fs::exists(fs::u8path(path)))
		return true;
	if (fs::create_directory(fs::u8path(path)))
		return true;

	Log(LogLevel::Error) << "Could not create directory: " << path << " : " << GetLastErrorString();
	return false;
#pragma warning(pop)
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


std::set<fs::path> GetAllFilesInFolder(const fs::path& path)
{
	std::set<fs::path> fileNames;
	if (!fs::exists(path))
		return fileNames;
	for (const auto& p: fs::directory_iterator(path))
	{
		if (!p.is_directory())
		{
			fileNames.insert(p.path().filename());
		}
	}
	return fileNames;
}



std::set<std::string> GetAllFilesInFolder(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
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
#pragma warning(pop)
}


void GetAllFilesInFolder(const std::string& path, std::set<std::string>& fileNames)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	fileNames = GetAllFilesInFolder(path);
#pragma warning(pop)
}


std::set<fs::path> GetAllSubfolders(const fs::path& path)
{
	std::set<fs::path> subFolders;
	if (!fs::exists(path))
		return subFolders;
	for (const auto& p: fs::directory_iterator(path))
	{
		if (p.is_directory())
		{
			subFolders.insert(p.path().filename());
		}
	}
	return subFolders;
}


std::set<std::string> GetAllSubfolders(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
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
#pragma warning(pop)
}


void GetAllSubfolders(const std::string& path, std::set<std::string>& subFolders)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	subFolders = GetAllSubfolders(path);
#pragma warning(pop)
}


std::set<fs::path> GetAllFilesInFolderRecursive(const fs::path& path)
{
	if (!fs::exists(path) || !fs::is_directory(path))
	{
		return {};
	}

	std::set<fs::path> fileNames;
	for (const auto& p: fs::recursive_directory_iterator(path))
	{
		if (!p.is_directory())
		{
			std::filesystem::path current_path = p.path();
			current_path.make_preferred();

			// Get the part of current_path that's not in the requested path
			auto current_itr = current_path.begin();
			for (auto path_itr = path.begin(); path_itr != path.end(); ++current_itr, ++path_itr)
			{
				if (path_itr->empty())
				{
					break;
				}
			}

			std::filesystem::path requested_path;
			for (; current_itr != current_path.end(); ++current_itr)
			{
				requested_path /= *current_itr;
			}
			fileNames.insert(requested_path);
		}
	}
	return fileNames;
}


void GetAllFilesInFolderRecursive(const std::string& path, std::set<std::string>& fileNames)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	fileNames = GetAllFilesInFolderRecursive(path);
#pragma warning(pop)
}


bool TryCopyFile(const std::string& sourcePath, const std::string& destPath)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	if (fs::copy_file(fs::u8path(sourcePath), fs::u8path(destPath), std::filesystem::copy_options::overwrite_existing))
		return true;
	LOG(LogLevel::Warning) << "Could not copy file " << sourcePath << " to " << destPath << " - " << GetLastErrorString();
	return false;
#pragma warning(pop)
}


bool CopyFolder(const std::string& sourceFolder, const std::string& destFolder)
{
#pragma warning(push)
#pragma warning(disable : 4996)
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
#pragma warning(pop)
}


bool RenameFolder(const std::string& sourceFolder, const std::string& destFolder)
{
#pragma warning(push)
#pragma warning(disable : 4996)
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
#pragma warning(pop)
}


bool DoesFileExist(const fs::path& path)
{
	return fs::exists(path) && !fs::is_directory(path);
}


bool DoesFileExist(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	return DoesFileExist(fs::u8path(path));
#pragma warning(pop)
}


bool DoesFolderExist(const fs::path& path)
{
	return fs::exists(path) && fs::is_directory(path);
}


bool DoesFolderExist(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	return DoesFolderExist(fs::u8path(path));
#pragma warning(pop)
}


bool DeleteFolder(const std::string& folder)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	if (!fs::exists(fs::u8path(folder)))
		return true;
	if (fs::remove_all(fs::u8path(folder)) != static_cast<std::uintmax_t>(-1))
		return true;
	Log(LogLevel::Error) << "Could not delete folder: " << folder << " " << GetLastErrorString();
	return false;
#pragma warning(pop)
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

} // namespace commonItems
