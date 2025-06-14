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


bool DoesFileExist(const path& path)
{
	return exists(path) && !is_directory(path);
}


bool DoesFolderExist(const path& path)
{
	return exists(path) && is_directory(path);
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