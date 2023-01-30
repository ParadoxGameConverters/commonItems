#include "BulkParser.h"
#include "CommonFunctions.h"

void commonItems::bulkParser::ParseGameFile(const std::string& relative_path, const ModFilesystem& mod_fs)
{
	if (const auto file_path = mod_fs.GetActualFileLocation(relative_path); file_path)
		parseFile(*file_path);
}

void commonItems::bulkParser::ParseGameFolder(const std::string& relative_path,
	 const ModFilesystem& mod_fs,
	 const std::set<std::string>& extensions,
	 bool recursive)
{
	std::set<std::string> all_files;
	if (recursive)
		all_files = mod_fs.GetAllFilesInFolderRecursive(relative_path);
	else
		all_files = mod_fs.GetAllFilesInFolder(relative_path);

	for (const auto& file: all_files)
		if (extensions.contains(getExtension(file)))
			parseFile(file);
}
