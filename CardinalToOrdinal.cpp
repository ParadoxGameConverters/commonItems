#include "CardinalToOrdinal.h"



std::string CardinalToOrdinal(const int cardinal)
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