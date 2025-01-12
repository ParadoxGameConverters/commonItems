#include "CommonFunctions.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <array>



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
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	const auto rawFile = trimPath(fileName);
#pragma warning(pop)
#pragma GCC diagnostic pop
	const auto filePos = fileName.find(rawFile);
	return fileName.substr(0, filePos);
}

std::string trimExtension(const std::string& fileName)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	const auto rawFile = trimPath(fileName);
#pragma warning(pop)
#pragma GCC diagnostic pop
	const auto dotPos = rawFile.find_last_of('.');
	if (dotPos == std::string::npos)
	{
		return fileName;
	}

	return fileName.substr(0, fileName.find(rawFile) + dotPos);
}

std::string getExtension(const std::string& fileName)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	const auto rawFile = trimPath(fileName);
#pragma warning(pop)
#pragma GCC diagnostic pop
	const auto dotPos = rawFile.find_last_of('.');
	if (dotPos == std::string::npos)
	{
		return {};
	}

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
	const std::array numbers{1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
	const std::array symbols{"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
	int i = static_cast<int>(numbers.size()) - 1;
	std::string toReturn;
	while (number > 0)
	{
		auto div = number / numbers[i];
		number = number % numbers[i];
		while (div-- > 0)
		{
			toReturn += symbols[i];
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
