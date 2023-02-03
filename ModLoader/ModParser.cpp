#include "ModParser.h"
#include "../CommonFunctions.h"
#include "../CommonRegexes.h"
#include "../ParserHelpers.h"
#include "../StringUtils.h"
#include "../external/json/single_include/nlohmann/json.hpp"
#include "../external/json/single_include/nlohmann/json_fwd.hpp"
#include <fstream>



void commonItems::ModParser::parseMod(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	if (!path.empty())
	{
		const auto ending = getExtension(path);
		compressed = ending == "zip" || ending == "bin";
	}
}

void commonItems::ModParser::parseMod(const std::string& fileName)
{
	registerKeys();
	parseFile(fileName);
	clearRegisteredKeywords();

	if (!path.empty())
	{
		const auto ending = getExtension(path);
		compressed = ending == "zip" || ending == "bin";
	}
}

void commonItems::ModParser::registerKeys()
{
	registerSetter("name", name);
	registerRegex("path|archive", [this](const std::string& unused, std::istream& theStream) {
		path = getString(theStream);
	});
	registerKeyword("dependencies", [this](std::istream& theStream) {
		const auto theDependencies = getStrings(theStream);
		dependencies.insert(theDependencies.begin(), theDependencies.end());
	});
	registerKeyword("replace_path", [this](std::istream& theStream) {
		replacedPaths.emplace(getString(theStream));
	});
	registerRegex(catchallRegex, ignoreItem);
}


void commonItems::ModParser::parseMetadata(std::istream& theStream)
{
	nlohmann::json data = nlohmann::json::parse(theStream);
	name = commonItems::remQuotes(data.value("name", ""));

	if (const auto game_custom_data = data.find("game_custom_data"); game_custom_data != data.end())
	{
		if (const auto replace_paths = game_custom_data->find("replace_paths"); replace_paths != game_custom_data->end())
		{
			for (const auto& path: *replace_paths)
			{
				replacedPaths.emplace(path);
			}
		}
	}
}


void commonItems::ModParser::parseMetadata(const std::string& fileName)
{
	std::filesystem::path fs_path(fileName);
	fs_path = fs_path.parent_path(); // remove metadata.json
	fs_path = fs_path.parent_path(); // remove /.metadata
	const std::string path_string = fs_path.generic_string();

	std::filesystem::path root_path = fs_path.parent_path(); // the mods folder
	root_path = root_path.parent_path();							// the parent of the mods folder
	const std::string root_path_string = root_path.generic_string();

	path = path_string.substr(root_path_string.size(), path_string.size());
	while (path.starts_with('/'))
	{
		path.erase(0, 1);
	}
	while (path.starts_with('\\'))
	{
		path.erase(0, 1);
	}

	std::ifstream file_stream(fileName);
	if (file_stream.is_open())
	{
		parseMetadata(file_stream);
		file_stream.close();
	}
}