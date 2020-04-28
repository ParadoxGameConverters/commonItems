#include "GameVersion.h"
#include "Log.h"
#include "ParserHelpers.h"



GameVersion::GameVersion(std::string version)
{
	if (version.empty())
	{
		return;
	}

	auto dot = version.find_first_of('.');
	firstPart = std::stoi(version.substr(0, dot));
	if (dot == std::string::npos)
	{
		return;
	}

	version = version.substr(dot + 1, version.size());
	dot = version.find_first_of('.');
	secondPart = std::stoi(version.substr(0, dot));
	if (dot == std::string::npos)
	{
		return;
	}

	version = version.substr(dot + 1, version.size());
	dot = version.find_first_of('.');
	thirdPart = std::stoi(version.substr(0, dot));
	if (dot == std::string::npos)
	{
		return;
	}

	version = version.substr(dot + 1, version.size());
	dot = version.find_first_of('.');
	fourthPart = std::stoi(version.substr(0, dot));
}


GameVersion::GameVersion(std::istream& theStream)
{
	registerKeyword("first", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		firstPart = firstString.getInt();
	});
	registerKeyword("second", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		secondPart = firstString.getInt();
	});
	registerKeyword("third", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		thirdPart = firstString.getInt();
	});
	registerKeyword("forth", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		fourthPart = firstString.getInt();
	});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	parseStream(theStream);
	clearRegisteredKeywords();
}


bool GameVersion::operator>=(const GameVersion& rhs) const
{
	return (*this > rhs) || (*this == rhs);
}


bool GameVersion::operator>(const GameVersion& rhs) const
{
	if (firstPart > rhs.firstPart)
	{
		return true;
	}
	if (firstPart == rhs.firstPart && secondPart > rhs.secondPart)
	{
		return true;
	}
	if (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart > rhs.thirdPart)
	{
		return true;
	}
	if (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart == rhs.thirdPart &&
		 fourthPart > rhs.fourthPart)
	{
		return true;
	}

	return false;
}


bool GameVersion::operator<(const GameVersion& rhs) const
{
	if (firstPart < rhs.firstPart)
	{
		return true;
	}
	if (firstPart == rhs.firstPart && secondPart < rhs.secondPart)
	{
		return true;
	}
	if (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart < rhs.thirdPart)
	{
		return true;
	}
	if (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart == rhs.thirdPart &&
		 fourthPart < rhs.fourthPart)
	{
		return true;
	}

	return false;
}


bool GameVersion::operator<=(const GameVersion& rhs) const
{
	return (*this < rhs) || (*this == rhs);
}


bool GameVersion::operator==(const GameVersion& rhs) const
{
	return firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart == rhs.thirdPart &&
			 fourthPart == rhs.fourthPart;
}


bool GameVersion::operator!=(const GameVersion& rhs) const
{
	return !(*this == rhs);
}


std::ostream& operator<<(std::ostream& out, const GameVersion& version)
{
	out << version.firstPart << '.';
	out << version.secondPart << '.';
	out << version.thirdPart << '.';
	out << version.fourthPart;
	return out;
}


GameVersion::Factory::Factory()
{
	registerKeyword("first", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		firstPart = firstString.getInt();
	});
	registerKeyword("second", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		secondPart = firstString.getInt();
	});
	registerKeyword("third", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		thirdPart = firstString.getInt();
	});
	registerKeyword("forth", [this](const std::string& unused, std::istream& theStream) {
		const commonItems::singleInt firstString(theStream);
		fourthPart = firstString.getInt();
	});
}


GameVersion GameVersion::Factory::getVersion(std::istream& theStream)
{
	firstPart = 0;
	secondPart = 0;
	thirdPart = 0;
	fourthPart = 0;

	parseStream(theStream);

	return GameVersion(firstPart, secondPart, thirdPart, fourthPart);
}