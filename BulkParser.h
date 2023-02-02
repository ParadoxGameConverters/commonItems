#ifndef BULK_PARSER_H
#define BULK_PARSER_H
#include "ModLoader/ModFilesystem.h"
#include "Parser.h"

// Bulk-parsing entire files and folders across incoming modFilesystem.

namespace commonItems
{
class bulkParser: public parser
{
  public:
	// Parses a game file in either vanilla game or mods directory.
	// For example:
	//		relativePath may be "map_data/areas.txt"
	void ParseGameFile(const std::string& relative_path, const ModFilesystem& mod_fs);

	// Parses a game folder in both vanilla game and mods directory.
	// For example:
	//		relativePath may be "common/governments"
	//		extensions may be "txt", "text"
	void ParseGameFolder(const std::string& relative_path, const ModFilesystem& mod_fs, const std::set<std::string>& extensions, bool recursive);
};
} // namespace commonItems
#endif // BULK_PARSER_H