#include "newColor.h"
#include "ParserHelpers.h"
#include <chrono>
#include <random>
#include <sstream>



commonItems::newColor::newColor(std::istream& theStream)
{
	const intList rgbList(theStream);
	const auto rgb = rgbList.getInts();
	if (rgb.size() == 3)
	{
		components[0] = rgb[0];
		components[1] = rgb[1];
		components[2] = rgb[2];
	}
}


void commonItems::newColor::RandomlyFluctuate(const int stdDev)
{
	static std::mt19937 generator(
		 static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	const auto allChange = std::normal_distribution<double>(0.0, stdDev)(generator);

	std::normal_distribution<double> distribution(0.0, stdDev / 4.0);
	for (auto& component: components)
	{
		component += lround(allChange + distribution(generator));
		if (component < 0)
		{
			component = 0;
		}
		else if (component > 255)
		{
			component = 255;
		}
	}
}


std::ostream& commonItems::operator<<(std::ostream& out, const newColor& color)
{
	out << color.components[0] << ' ' << color.components[1] << ' ' << color.components[2];
	return out;
}


commonItems::newColor commonItems::newColor::Factory::getColor(std::istream& theStream)
{
	const intList rgbList(theStream);
	const auto rgb = rgbList.getInts();
	if (rgb.size() == 3)
	{
		return newColor(rgb[0], rgb[1], rgb[2]);
	}

	return newColor{};
}