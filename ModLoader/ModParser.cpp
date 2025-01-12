#include "ModParser.h"

#include <external/json/single_include/nlohmann/json.hpp>
#include <external/json/single_include/nlohmann/json_fwd.hpp>
#include <fstream>

#include "../CommonFunctions.h"
#include "../CommonRegexes.h"
#include "../ParserHelpers.h"
#include "../StringUtils.h"



using std::filesystem::path;
using std::filesystem::u8path;



void commonItems::ModParser::parseMod(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	if (!path_.empty())
	{
		const auto ending = path_.extension();
		compressed_ = ending == ".zip" || ending == ".bin";
	}
}

void commonItems::ModParser::parseMod(const path& fileName)
{
	registerKeys();
	parseFile(fileName);
	clearRegisteredKeywords();

	if (!path_.empty())
	{
		const auto ending = path_.extension();
		compressed_ = ending == ".zip" || ending == ".bin";
	}
}


void commonItems::ModParser::parseMod(const std::string& fileName)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	parseMod(u8path(fileName));
#pragma warning(pop)
#pragma GCC diagnostic pop
}


void commonItems::ModParser::registerKeys()
{
	registerSetter("name", name_);
	registerRegex("path|archive", [this]([[maybe_unused]] const std::string& unused, std::istream& theStream) {
		path_ = getString(theStream);
	});
	registerKeyword("dependencies", [this](std::istream& theStream) {
		const auto theDependencies = getStrings(theStream);
		dependencies_.insert(theDependencies.begin(), theDependencies.end());
	});
	registerKeyword("replace_path", [this](std::istream& theStream) {
		replacedPaths_.emplace(path(getString(theStream)).make_preferred());
	});
	registerRegex(catchallRegex, ignoreItem);
}


void commonItems::ModParser::parseMetadata(std::istream& theStream)
{
	nlohmann::json data = nlohmann::json::parse(theStream);
	name_ = commonItems::remQuotes(data.value("name", ""));

	if (const auto game_custom_data = data.find("game_custom_data"); game_custom_data != data.end())
	{
		if (const auto replace_paths = game_custom_data->find("replace_paths"); replace_paths != game_custom_data->end())
		{
			for (const auto& replace_path: *replace_paths)
			{
				replacedPaths_.emplace(path(std::string(replace_path)).make_preferred());
			}
		}
	}
}


void commonItems::ModParser::parseMetadata(const path& metadata_path)
{
	path fs_path(metadata_path);
	fs_path = fs_path.parent_path(); // remove metadata.json
	fs_path = fs_path.parent_path(); // remove /.metadata

	// path_ should be the final two path components
	path last;
	path second_to_last;
	for (const auto component: fs_path)
	{
		second_to_last = last;
		last = component;
	}
	path_ = second_to_last / last;
	path_.make_preferred();

	std::ifstream file_stream(metadata_path);
	if (file_stream.is_open())
	{
		parseMetadata(file_stream);
		file_stream.close();
	}
}


void commonItems::ModParser::parseMetadata(const std::string& fileName)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	parseMetadata(u8path(fileName));
#pragma warning(pop)
#pragma GCC diagnostic pop
}


const std::set<std::string> commonItems::ModParser::getReplacedPaths() const
{
	std::set<std::string> replaced_paths;
	for (const path& path: replacedPaths_)
	{
		replaced_paths.emplace(path.string());
	}

	return replaced_paths;
}


void commonItems::ModParser::setPath(const std::string& path)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	path_ = u8path(path).make_preferred();
#pragma warning(pop)
#pragma GCC diagnostic pop
}