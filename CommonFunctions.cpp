#include "CommonFunctions.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <array>



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
