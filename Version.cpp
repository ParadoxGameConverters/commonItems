#include "Version.h"
#include "Log.h"
#include "ParserHelpers.h"



Version::Version(std::string version)
{
	if (version.length() == 0)
	{
		return;
	}

	auto dot = version.find_first_of('.');
	firstPart = std::stoi(version.substr(0, dot));

	if (dot != std::string::npos)
	{
		version = version.substr(dot + 1, version.size());
		dot = version.find_first_of('.');
		secondPart = std::stoi(version.substr(0, dot));

		if (dot != std::string::npos)
		{
			version = version.substr(dot + 1, version.size());
			dot = version.find_first_of('.');
			thirdPart = std::stoi(version.substr(0, dot));

			if (dot != std::string::npos)
			{
				version = version.substr(dot + 1, version.size());
				dot = version.find_first_of('.');
				fourthPart = std::stoi(version.substr(0, dot));
			}
		}
	}
}


Version::Version(std::istream& theStream)
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


bool Version::operator>=(const Version& rhs) const
{
	return (*this > rhs) || (*this == rhs);
}


bool Version::operator>(const Version& rhs) const
{
	return (firstPart > rhs.firstPart) || (firstPart == rhs.firstPart && secondPart > rhs.secondPart) ||
			 (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart > rhs.thirdPart) ||
			 (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart == rhs.thirdPart &&
				  fourthPart > rhs.fourthPart);
}

bool Version::operator<(const Version& rhs) const
{
	return (firstPart < rhs.firstPart) || (firstPart == rhs.firstPart && secondPart < rhs.secondPart) ||
			 (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart < rhs.thirdPart) ||
			 (firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart == rhs.thirdPart &&
				  fourthPart < rhs.fourthPart);
}

bool Version::operator<=(const Version& rhs) const
{
	return (*this < rhs) || (*this == rhs);
}

bool Version::operator==(const Version& rhs) const
{
	return firstPart == rhs.firstPart && secondPart == rhs.secondPart && thirdPart == rhs.thirdPart &&
			 fourthPart == rhs.fourthPart;
}

bool Version::operator!=(const Version& rhs) const
{
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& out, const Version& version)
{
	out << version.firstPart << '.';
	out << version.secondPart << '.';
	out << version.thirdPart << '.';
	out << version.fourthPart;
	return out;
}


Version::Factory::Factory()
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


Version Version::Factory::getVersion(std::istream& theStream)
{
	firstPart = 0;
	secondPart = 0;
	thirdPart = 0;
	fourthPart = 0;

	parseStream(theStream);

	return Version(firstPart, secondPart, thirdPart, fourthPart);
}