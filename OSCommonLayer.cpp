#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <codecvt>
#include <filesystem>



using std::filesystem::copy_options;
using std::filesystem::current_path;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::filesystem::recursive_directory_iterator;
using std::filesystem::u8path;



namespace commonItems
{

bool TryCreateFolder(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	if (exists(u8path(path)))
		return true;
	if (create_directory(u8path(path)))
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
		const auto path = current_path().string();
		return convertUTF8ToUTF16(path);
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Error) << "Cannot determine current path; " << e.what();
		return {};
	}
}


std::set<path> GetAllFilesInFolder(const path& folder_path)
{
	std::set<path> fileNames;
	if (!exists(folder_path))
		return fileNames;
	for (const auto& p: directory_iterator(folder_path))
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
	if (!exists(u8path(path)))
		return fileNames;
	for (const auto& p: directory_iterator(u8path(path)))
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


std::set<path> GetAllSubfolders(const path& folder_path)
{
	std::set<path> subFolders;
	if (!exists(folder_path))
		return subFolders;
	for (const auto& p: directory_iterator(folder_path))
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
	if (!exists(u8path(path)))
		return subFolders;
	for (const auto& p: directory_iterator(u8path(path)))
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


std::set<path> GetAllFilesInFolderRecursive(const path& folder_path)
{
	if (!exists(folder_path) || !is_directory(folder_path))
	{
		return {};
	}

	std::set<path> fileNames;
	for (const auto& p: recursive_directory_iterator(folder_path))
	{
		if (!p.is_directory())
		{
			path current_path = p.path();
			current_path.make_preferred();

			// Get the part of current_path that's not in the requested path
			auto current_itr = current_path.begin();
			for (auto path_itr = folder_path.begin(); path_itr != folder_path.end(); ++current_itr, ++path_itr)
			{
				if (path_itr->empty())
				{
					break;
				}
			}

			path requested_path;
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
	if (copy_file(u8path(sourcePath), u8path(destPath), copy_options::overwrite_existing))
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
		copy(u8path(sourceFolder), u8path(destFolder), copy_options::recursive);
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
		rename(u8path(sourceFolder), u8path(destFolder));
		return true;
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Error) << "Could not rename folder: " << e.what() << " " << GetLastErrorString();
		return false;
	}
#pragma warning(pop)
}


bool DoesFileExist(const path& path)
{
	return exists(path) && !is_directory(path);
}


bool DoesFileExist(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	return DoesFileExist(u8path(path));
#pragma warning(pop)
}


bool DoesFolderExist(const path& path)
{
	return exists(path) && is_directory(path);
}


bool DoesFolderExist(const std::string& path)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	return DoesFolderExist(u8path(path));
#pragma warning(pop)
}


bool DeleteFolder(const std::string& folder)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	if (!exists(u8path(folder)))
		return true;
	if (remove_all(u8path(folder)) != static_cast<std::uintmax_t>(-1))
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
