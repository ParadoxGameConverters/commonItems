#include "Color.h"
#include "ParserHelpers.h"
#include <chrono>
#include <random>
#include <sstream>



commonItems::Color::Color(std::istream& theStream, bool hasColorSpacePrefix)
{
	if (hasColorSpacePrefix) auto ColorSpaceString = getNextTokenWithoutMatching(theStream); /// example of when it's needed:	"color2 = rgb { 100 7 15 }"
	const intList rgbList(theStream);
	const auto rgb = rgbList.getInts();
	if (rgb.size() == 3)
	{
		c[0] = rgb[0];
		c[1] = rgb[1];
		c[2] = rgb[2];
		initialized = true;
	}
}


void commonItems::Color::GetRGB(int& r, int& g, int& b) const
{
	r = c[0];
	g = c[1];
	b = c[2];
}


void commonItems::Color::RandomlyFluctuate(const int stdDev)
{
	static std::mt19937 generator(
		 static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	const auto allChange = std::normal_distribution<double>(0.0, stdDev)(generator);

	std::normal_distribution<double> distribution(0.0, stdDev / 4.0);
	for (auto& component: c)
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


std::ostream& commonItems::operator<<(std::ostream& out, const Color& color)
{
	out << color.c[0] << ' ' << color.c[1] << ' ' << color.c[2];
	return out;
}


commonItems::Color commonItems::Color::Factory::getColor(std::istream& theStream)
{
	const intList rgbList(theStream);
	const auto rgb = rgbList.getInts();
	if (rgb.size() == 3)
	{
		return Color(rgb[0], rgb[1], rgb[2]);
	}

	return Color{};
}