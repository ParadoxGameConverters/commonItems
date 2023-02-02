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
	auto file_path = ::getPath(fileName);

	// remove last slash
	if (const auto last_slash = file_path.find_last_of('/'); last_slash != std::string::npos)
	{
		file_path = file_path.substr(0, last_slash);
	}
	else if (const auto last_slash = file_path.find_last_of('\\'); last_slash != std::string::npos)
	{
		file_path = file_path.substr(0, last_slash);
	}

	if (const auto last_slash = file_path.find_last_of('/'); last_slash != std::string::npos)
	{
		path = file_path.substr(last_slash + 1, file_path.size());
	}
	else if (const auto last_slash = file_path.find_last_of('\\'); last_slash != std::string::npos)
	{
		path = file_path.substr(last_slash + 1, file_path.size());
	}

	std::ifstream file_stream(fileName);
	if (file_stream.is_open())
	{
		parseMetadata(file_stream);
		file_stream.close();
	}
}