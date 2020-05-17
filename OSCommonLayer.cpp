#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
namespace fs = std::filesystem;

namespace Utils
{

bool TryCreateFolder(const std::string& path)
{
	if (fs::exists(fs::u8path(path)))
		return true;
	const auto success = fs::create_directory(fs::u8path(path));
	if (success)
		return true;

	Log(LogLevel::Error) << "Could not create directory: " << path << " : " << GetLastErrorString();
	return false;
}

std::string GetCurrentDirectory()
{
	return fs::current_path().string();
}

std::set<std::string> GetAllFilesInFolder(const std::string& path)
{
	std::set<std::string> fileNames;
	if (!fs::exists(fs::u8path(path)))
		return fileNames;
	for (auto& p: fs::directory_iterator(fs::u8path(path)))
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
	for (auto& p: fs::directory_iterator(fs::u8path(path)))
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
	const auto success = fs::copy_file(fs::u8path(sourcePath), fs::u8path(destPath));
	if (success)
		return true;
	LOG(LogLevel::Warning) << "Could not copy file " << sourcePath << " to " << destPath << " - "
								  << GetLastErrorString();
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
	const auto tempPath = fs::u8path(path);
	if (fs::exists(tempPath) && !fs::is_directory(tempPath))
		return true;
	return false;
}


bool DoesFolderExist(const std::string& path)
{
	const auto tempPath = fs::u8path(path);
	if (fs::exists(tempPath) && fs::is_directory(tempPath))
		return true;
	return false;
}

bool DeleteFolder(const std::string& folder)
{
	if (!fs::exists(fs::u8path(folder)))
		return true;
	const auto success = fs::remove_all(fs::u8path(folder));
	if (success)
		return true;
	Log(LogLevel::Error) << "Could not delete folder: " << folder << " " << GetLastErrorString();
	return false;
}

} // namespace Utils
