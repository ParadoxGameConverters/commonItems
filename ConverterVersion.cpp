#include "ConverterVersion.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include <fstream>

void commonItems::ConverterVersion::loadVersion(const std::string& fileName)
{
	registerKeys();
	parseFile(fileName);
	clearRegisteredKeywords();
}

void commonItems::ConverterVersion::loadVersion(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void commonItems::ConverterVersion::registerKeys()
{
	registerSetter("name", name);
	registerSetter("version", version);
	registerSetter("source", source);
	registerSetter("target", target);
	registerKeyword("minSource", [this](std::istream& theStream) {
		minSource = GameVersion(getString(theStream));
	});
	registerKeyword("maxSource", [this](std::istream& theStream) {
		maxSource = GameVersion(getString(theStream));
	});
	registerKeyword("minTarget", [this](std::istream& theStream) {
		minTarget = GameVersion(getString(theStream));
	});
	registerKeyword("maxTarget", [this](std::istream& theStream) {
		maxTarget = GameVersion(getString(theStream));
	});
	registerRegex(catchallRegex, ignoreItem);
}

std::string commonItems::ConverterVersion::getDescription() const
{
	auto toReturn = "Compatible with " + source + " [v" + minSource.toShortString();
	if (maxSource != minSource)
		toReturn += "-v" + maxSource.toShortString();
	toReturn += "] and " + target + " [v" + minTarget.toShortString();
	if (maxTarget != minTarget)
		toReturn += "-v" + maxTarget.toShortString();
	toReturn += "]";

	return toReturn;
}

std::ostream& commonItems::operator<<(std::ostream& output, const ConverterVersion& version)
{
	output << "\n\n";
	output << "************ -= The Paradox Game Converters Group =- *****************\n";

	if (!version.version.empty() && !version.name.empty())
		output << "* Converter version " << version.version << " \"" << version.name << "\"\n";

	output << "* " << version.getDescription() << "\n";
	output << "* Built on " << __TIMESTAMP__ << "\n";

	std::string footerTitle = " + " + version.source + " To " + version.target + " + ";
	if (footerTitle.length() >= 68)
	{
		footerTitle = "*" + footerTitle + "*\n";
	}
	else
	{
		auto target = static_cast<int>((70 - footerTitle.length()) / 2);
		for (auto counter = 0; counter < target; counter++)
			footerTitle = "*" + footerTitle;

		target = static_cast<int>(70 - footerTitle.length());
		for (auto counter = 0; counter < target; counter++)
			footerTitle += "*";
		footerTitle += "\n";
	}

	output << footerTitle;
	return output;
}