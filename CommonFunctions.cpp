#include "CommonFunctions.h"
#include <algorithm>

std::string trimPath(const std::string& fileName)
{
	auto lastSlash = fileName.find_last_of('\\');
	auto trimmedFileName = fileName.substr(lastSlash + 1, fileName.length());
	lastSlash = trimmedFileName.find_last_of('/');
	trimmedFileName = trimmedFileName.substr(lastSlash + 1, trimmedFileName.length());
	return trimmedFileName;
}

std::string trimExtension(const std::string& fileName)
{
	const int length = fileName.find_last_of('.');
	return fileName.substr(0, length);
}

std::string replaceCharacter(std::string fileName, char character)
{
	std::replace(fileName.begin(), fileName.end(), character, '_');
	return fileName;
}

std::string cardinalToOrdinal(const int cardinal)
{
	const auto hundredRemainder = cardinal % 100;
	const auto tenRemainder = cardinal % 10;
	if (hundredRemainder - tenRemainder == 10)
	{
		return "th";
	}

	switch (tenRemainder)
	{
		case 1:
			return "st";
		case 2:
			return "nd";
		case 3:
			return "rd";
		default:
			return "th";
	}
}