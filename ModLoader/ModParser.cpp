#include "ModParser.h"
#include "../CommonFunctions.h"
#include "../CommonRegexes.h"
#include "../ParserHelpers.h"

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
	registerRegex(catchallRegex, ignoreItem);
}
