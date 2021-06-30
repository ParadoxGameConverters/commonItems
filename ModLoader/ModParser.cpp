#include "ModParser.h"
#include "../CommonFunctions.h"
#include "../CommonRegexes.h"
#include "../ParserHelpers.h"

commonItems::ModParser::ModParser(std::istream& theStream)
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

void commonItems::ModParser::registerKeys()
{
	registerSetter("name", name);
	registerRegex("path|archive", [this](const std::string& unused, std::istream& theStream) {
		path = getString(theStream);
	});
	registerRegex(catchallRegex, ignoreItem);
}
