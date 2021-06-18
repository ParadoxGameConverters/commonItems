#include "GameVersion.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include <sstream>

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
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void GameVersion::registerKeys()
{
	registerKeyword("first", [this](std::istream& theStream) {
		firstPart = commonItems::getInt(theStream);
	});
	registerKeyword("second", [this](std::istream& theStream) {
		secondPart = commonItems::getInt(theStream);
	});
	registerKeyword("third", [this](std::istream& theStream) {
		thirdPart = commonItems::getInt(theStream);
	});
	registerKeyword("forth", [this](std::istream& theStream) {
		fourthPart = commonItems::getInt(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

bool GameVersion::operator>=(const GameVersion& rhs) const
{
	return *this > rhs || *this == rhs;
}

bool GameVersion::operator>(const GameVersion& rhs) const
{
	auto testL = 0;
	auto testR = 0;
	if (firstPart)
		testL = *firstPart;
	if (rhs.firstPart)
		testR = *rhs.firstPart;

	if (testL > testR)
		return true;

	if (testL == testR)
	{
		testL = 0;
		testR = 0;
		if (secondPart)
			testL = *secondPart;
		if (rhs.secondPart)
			testR = *rhs.secondPart;

		if (testL > testR)
			return true;

		if (testL == testR)
		{
			testL = 0;
			testR = 0;
			if (thirdPart)
				testL = *thirdPart;
			if (rhs.thirdPart)
				testR = *rhs.thirdPart;

			if (testL > testR)
				return true;

			if (testL == testR)
			{
				testL = 0;
				testR = 0;
				if (fourthPart)
					testL = *fourthPart;
				if (rhs.fourthPart)
					testR = *rhs.fourthPart;

				if (testL > testR)
					return true;
			}
		}
	}

	return false;
}

bool GameVersion::operator<(const GameVersion& rhs) const
{
	auto testL = 0;
	auto testR = 0;
	if (firstPart)
		testL = *firstPart;
	if (rhs.firstPart)
		testR = *rhs.firstPart;

	if (testL < testR)
		return true;

	if (testL == testR)
	{
		testL = 0;
		testR = 0;
		if (secondPart)
			testL = *secondPart;
		if (rhs.secondPart)
			testR = *rhs.secondPart;

		if (testL < testR)
			return true;

		if (testL == testR)
		{
			testL = 0;
			testR = 0;
			if (thirdPart)
				testL = *thirdPart;
			if (rhs.thirdPart)
				testR = *rhs.thirdPart;

			if (testL < testR)
				return true;

			if (testL == testR)
			{
				testL = 0;
				testR = 0;
				if (fourthPart)
					testL = *fourthPart;
				if (rhs.fourthPart)
					testR = *rhs.fourthPart;

				if (testL < testR)
					return true;
			}
		}
	}

	return false;
}

bool GameVersion::operator<=(const GameVersion& rhs) const
{
	return *this < rhs || *this == rhs;
}

bool GameVersion::operator==(const GameVersion& rhs) const
{
	auto testL = 0;
	auto testR = 0;
	if (firstPart)
		testL = *firstPart;
	if (rhs.firstPart)
		testR = *rhs.firstPart;

	if (testL != testR)
		return false;

	testL = 0;
	testR = 0;
	if (secondPart)
		testL = *secondPart;
	if (rhs.secondPart)
		testR = *rhs.secondPart;

	if (testL != testR)
		return false;

	testL = 0;
	testR = 0;
	if (thirdPart)
		testL = *thirdPart;
	if (rhs.thirdPart)
		testR = *rhs.thirdPart;

	if (testL != testR)
		return false;

	testL = 0;
	testR = 0;
	if (fourthPart)
		testL = *fourthPart;
	if (rhs.fourthPart)
		testR = *rhs.fourthPart;

	if (testL != testR)
		return false;

	return true;
}

bool GameVersion::operator!=(const GameVersion& rhs) const
{
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& out, const GameVersion& version)
{
	if (version.firstPart)
		out << *version.firstPart << '.';
	else
		out << "0.";
	if (version.secondPart)
		out << *version.secondPart << '.';
	else
		out << "0.";
	if (version.thirdPart)
		out << *version.thirdPart << '.';
	else
		out << "0.";
	if (version.fourthPart)
		out << *version.fourthPart;
	else
		out << "0";
	return out;
}

GameVersion::Factory::Factory()
{
	registerKeyword("first", [this](std::istream& theStream) {
		firstPart = commonItems::getInt(theStream);
	});
	registerKeyword("second", [this](std::istream& theStream) {
		secondPart = commonItems::getInt(theStream);
	});
	registerKeyword("third", [this](std::istream& theStream) {
		thirdPart = commonItems::getInt(theStream);
	});
	registerKeyword("forth", [this](std::istream& theStream) {
		fourthPart = commonItems::getInt(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

GameVersion GameVersion::Factory::getVersion(std::istream& theStream)
{
	firstPart.reset();
	secondPart.reset();
	thirdPart.reset();
	fourthPart.reset();

	parseStream(theStream);

	return GameVersion(firstPart, secondPart, thirdPart, fourthPart);
}

bool GameVersion::smallerish(const GameVersion& rhs) const
{
	// Smallerish is intended for fuzzy comparisons like "converter works with 1.9",
	// so everything from 1.0.0.0 to 1.9.x.y will match.

	auto testDigit = 0;
	if (rhs.firstPart)
		testDigit = *rhs.firstPart;
	if (firstPart && testDigit > *firstPart)
		return false;

	testDigit = 0;
	if (rhs.secondPart)
		testDigit = *rhs.secondPart;
	if (secondPart && testDigit > *secondPart)
		return false;

	testDigit = 0;
	if (rhs.thirdPart)
		testDigit = *rhs.thirdPart;
	if (thirdPart && testDigit > *thirdPart)
		return false;

	testDigit = 0;
	if (rhs.fourthPart)
		testDigit = *rhs.fourthPart;
	if (fourthPart && testDigit > *fourthPart)
		return false;

	return true;
}
