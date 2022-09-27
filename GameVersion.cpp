#include "GameVersion.h"
#include "CommonRegexes.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>

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
	if (testL < testR)
		return false;

	testL = 0;
	testR = 0;
	if (secondPart)
		testL = *secondPart;
	if (rhs.secondPart)
		testR = *rhs.secondPart;

	if (testL > testR)
		return true;
	if (testL < testR)
		return false;

	testL = 0;
	testR = 0;
	if (thirdPart)
		testL = *thirdPart;
	if (rhs.thirdPart)
		testR = *rhs.thirdPart;

	if (testL > testR)
		return true;
	if (testL < testR)
		return false;

	testL = 0;
	testR = 0;
	if (fourthPart)
		testL = *fourthPart;
	if (rhs.fourthPart)
		testR = *rhs.fourthPart;

	return testL > testR;
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
	if (testL > testR)
		return false;

	testL = 0;
	testR = 0;
	if (secondPart)
		testL = *secondPart;
	if (rhs.secondPart)
		testR = *rhs.secondPart;

	if (testL < testR)
		return true;
	if (testL > testR)
		return false;

	testL = 0;
	testR = 0;
	if (thirdPart)
		testL = *thirdPart;
	if (rhs.thirdPart)
		testR = *rhs.thirdPart;

	if (testL < testR)
		return true;
	if (testL > testR)
		return false;

	testL = 0;
	testR = 0;
	if (fourthPart)
		testL = *fourthPart;
	if (rhs.fourthPart)
		testR = *rhs.fourthPart;

	return testL < testR;
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

	return testL == testR;
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

std::string GameVersion::toString() const
{
	std::string toReturn;
	if (firstPart)
		toReturn += std::to_string(*firstPart) + ".";
	else
		toReturn += "0.";
	if (secondPart)
		toReturn += std::to_string(*secondPart) + ".";
	else
		toReturn += "0.";
	if (thirdPart)
		toReturn += std::to_string(*thirdPart) + ".";
	else
		toReturn += "0.";
	if (fourthPart)
		toReturn += std::to_string(*fourthPart);
	else
		toReturn += "0";

	return toReturn;
}

std::string GameVersion::toShortString() const
{
	std::string nameString;
	if (fourthPart)
		nameString = "." + std::to_string(*fourthPart);

	if (thirdPart)
		nameString = "." + std::to_string(*thirdPart) + nameString;

	if (secondPart)
		nameString = "." + std::to_string(*secondPart) + nameString;

	if (firstPart)
		nameString = std::to_string(*firstPart) + nameString;

	return nameString;
}

std::string GameVersion::toWildCard() const
{
	std::string nameString;
	if (fourthPart)
		nameString = "." + std::to_string(*fourthPart);
	else if (thirdPart)
		nameString = ".*";

	if (thirdPart)
		nameString = "." + std::to_string(*thirdPart) + nameString;
	else if (secondPart)
		nameString = ".*";

	if (secondPart)
		nameString = "." + std::to_string(*secondPart) + nameString;
	else if (firstPart)
		nameString = ".*";

	if (firstPart)
		nameString = std::to_string(*firstPart) + nameString;
	else
		nameString = "*";

	return nameString;
}

bool GameVersion::isLargerishThan(const GameVersion& rhs) const
{
	// Largerish is intended for fuzzy comparisons like "converter works with up to 1.9",
	// so everything incoming on rhs from 0.0.0.0 to 1.9.x.y will match, (where x and y are >= 0),
	// thus overshooting the internal "1.9.0.0" setup. This works if ".0.0" are actually undefined.

	auto testDigit = 0;
	if (rhs.firstPart)
		testDigit = *rhs.firstPart;
	if (firstPart && testDigit > *firstPart)
		return false;
	if (firstPart && testDigit < *firstPart)
		return true;

	testDigit = 0;
	if (rhs.secondPart)
		testDigit = *rhs.secondPart;
	if (secondPart && testDigit > *secondPart)
		return false;
	if (secondPart && testDigit < *secondPart)
		return true;

	testDigit = 0;
	if (rhs.thirdPart)
		testDigit = *rhs.thirdPart;
	if (thirdPart && testDigit > *thirdPart)
		return false;
	if (thirdPart && testDigit < *thirdPart)
		return true;

	testDigit = 0;
	if (rhs.fourthPart)
		testDigit = *rhs.fourthPart;
	return !fourthPart || testDigit <= *fourthPart;
}

std::optional<GameVersion> GameVersion::extractVersionFromLauncher(const std::string& filePath)
{
	// use this for modern PDX games, point filePath to launcher-settings.json to get installation version.

	if (!commonItems::DoesFileExist(filePath))
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " does not exist.";
		return std::nullopt;
	}

	auto result = extractVersionByStringFromLauncher("rawVersion", filePath);
	if (!result)
	{
		// imperator rome?
		result = extractVersionByStringFromLauncher("version", filePath);
	}
	if (!result)
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " does not contain installation version!";
		return std::nullopt;
	}
	return result;
}

std::optional<GameVersion> GameVersion::extractVersionByStringFromLauncher(const std::string& versionString, const std::string& filePath)
{
	std::ifstream versionFile(filePath);
	if (!versionFile.is_open())
		return std::nullopt;

	while (!versionFile.eof())
	{
		std::string line;
		std::getline(versionFile, line);
		if (line.find(versionString) == std::string::npos)
			continue;
		auto pos = line.find(':');
		if (pos == std::string::npos)
		{
			versionFile.close();
			return std::nullopt;
		}

		line = line.substr(pos + 1, line.length());
		pos = line.find_first_of('\"');
		if (pos == std::string::npos)
		{
			versionFile.close();
			return std::nullopt;
		}

		line = line.substr(pos + 1, line.length());
		pos = line.find_first_of('\"');
		if (pos == std::string::npos)
		{
			versionFile.close();
			return std::nullopt;
		}

		line = line.substr(0, pos);

		try
		{
			return GameVersion(line);
		}
		catch (std::exception&)
		{
			versionFile.close();
			return std::nullopt;
		}
	}

	versionFile.close();
	return std::nullopt;
}

std::optional<GameVersion> GameVersion::extractVersionFromReadMe(const std::string& filePath)
{
	// Use this for Vic2 ReadMe.txt/Readme.txt. Be sure to check both as name changes across versions, and it's not
	// internally consistent on windows. Steam update from one version to another can in fact *not* change the case on
	// the file, even if fresh reinstallation would!

	std::ifstream versionFile(filePath);
	if (!versionFile.is_open())
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " cannot be opened.";
		return std::nullopt;
	}

	std::string line;
	std::getline(versionFile, line); // skip first line
	if (versionFile.eof())
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " is broken.";
		return std::nullopt;
	}
	std::getline(versionFile, line);

	auto pos = line.find(" below");
	if (pos == std::string::npos)
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " is broken.";
		return std::nullopt;
	}

	line = line.substr(0, pos);
	pos = line.find_last_of(' ');
	line = line.substr(pos + 1, line.length());

	if (!std::isdigit(*line.begin()))
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken version.";
		return std::nullopt;
	}

	try
	{
		return GameVersion(line);
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << line << " is broken: " << e.what();
		return std::nullopt;
	}
}

std::optional<GameVersion> GameVersion::extractVersionFromChangeLog(const std::string& filePath)
{
	// Use this to get CK2 installation version from CK2's ChangeLog.txt.

	if (!commonItems::DoesFileExist(filePath))
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " does not exist.";
		return std::nullopt;
	}

	std::ifstream versionFile(filePath);
	if (!versionFile.is_open())
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " cannot be opened.";
		return std::nullopt;
	}

	std::string line;
	std::getline(versionFile, line);
	std::getline(versionFile, line);
	if (versionFile.eof())
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " is broken.";
		return std::nullopt;
	}
	std::getline(versionFile, line);

	auto pos = line.find_first_of(' ');
	if (pos == std::string::npos)
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken version.";
		return std::nullopt;
	}
	line = line.substr(pos + 1, line.length());
	pos = line.find_first_of(' ');
	if (pos == std::string::npos)
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken version.";
		return std::nullopt;
	}
	line = line.substr(0, pos);
	if (!std::isdigit(*line.begin()))
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << filePath << " has broken version.";
		return std::nullopt;
	}
	try
	{
		return GameVersion(line);
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Warning) << "Failure extracting version: " << line << " is broken: " << e.what();
		return std::nullopt;
	}
}
