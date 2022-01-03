#include "CommonFunctions.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>

std::string trimPath(const std::string& fileName)
{
	auto lastSlash = fileName.find_last_of('\\');
	auto trimmedFileName = fileName.substr(lastSlash + 1, fileName.length());
	lastSlash = trimmedFileName.find_last_of('/');
	trimmedFileName = trimmedFileName.substr(lastSlash + 1, trimmedFileName.length());
	return trimmedFileName;
}

std::string getPath(const std::string& fileName)
{
	const auto rawFile = trimPath(fileName);
	const auto filePos = fileName.find(rawFile);
	return fileName.substr(0, filePos);
}

std::string trimExtension(const std::string& fileName)
{
	const auto rawFile = trimPath(fileName);
	const auto dotPos = rawFile.find_last_of('.');
	if (dotPos == std::string::npos)
		return fileName;
	else
		return fileName.substr(0, fileName.find(rawFile) + dotPos);
}

std::string getExtension(const std::string& fileName)
{
	const auto rawFile = trimPath(fileName);
	const auto dotPos = rawFile.find_last_of('.');
	if (dotPos == std::string::npos)
		return std::string();
	else
		return rawFile.substr(dotPos + 1);
}

std::string replaceCharacter(std::string fileName, const char character)
{
	std::ranges::replace(fileName, character, '_');
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

std::string cardinalToRoman(int number)
{
	int num[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
	std::string sym[] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
	int i = 12;
	std::string toReturn;
	while (number > 0)
	{
		auto div = number / num[i];
		number = number % num[i];
		while (div--)
		{
			toReturn += sym[i];
		}
		i--;
	}
	return toReturn;
}

std::string normalizeStringPath(const std::string& stringPath)
{
	auto toReturn = commonItems::normalizeUTF8Path(stringPath);
	toReturn = replaceCharacter(toReturn, '-');
	toReturn = replaceCharacter(toReturn, ' ');

	return toReturn;
}
