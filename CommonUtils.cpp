#include "OSCompatibilityLayer.h"

std::optional<std::string> Utils::GetFileFromTag(const std::string& directoryPath, const std::string& tag)
{
	std::set<std::string> foundFiles;
	GetAllFilesInFolder(directoryPath, foundFiles);
	for (std::string file: foundFiles)
	{
		if (tag == file.substr(0, 3))
		{
			return file;
		}
	}

	return {};
}