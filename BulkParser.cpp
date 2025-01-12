#include "BulkParser.h"
#include "CommonFunctions.h"



using std::filesystem::path;



void commonItems::bulkParser::ParseGameFile(const path& relative_path, const ModFilesystem& mod_fs)
{
	if (const auto file_path = mod_fs.GetActualFileLocation(relative_path); file_path)
		parseFile(*file_path);
}


void commonItems::bulkParser::ParseGameFolder(const path& relative_path, const ModFilesystem& mod_fs, const std::set<path>& extensions, bool recursive)
{
	std::set<path> all_files;
	if (recursive)
		all_files = mod_fs.GetAllFilesInFolderRecursive(relative_path);
	else
		all_files = mod_fs.GetAllFilesInFolder(relative_path);

	for (const auto& file: all_files)
		if (extensions.contains(file.extension()))
			parseFile(file);
}