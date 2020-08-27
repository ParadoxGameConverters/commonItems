#include "newColor.h"
#include "ParserHelpers.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <random>
#include <sstream>



bool commonItems::newColor::operator==(const newColor& rhs) const
{
	return rgbComponents == rhs.rgbComponents;
}


bool commonItems::newColor::operator!=(const newColor& rhs) const
{
	return !(*this == rhs);
}


std::string commonItems::newColor::outputRgb() const
{
	return "= rgb { " + std::to_string(rgbComponents[0]) + ' ' + std::to_string(rgbComponents[1]) + ' ' +
			 std::to_string(rgbComponents[2]) + " }";
}


std::string commonItems::newColor::outputHsv() const
{
	std::stringstream output;
	output << std::setprecision(2);
	output << "= hsv { " << hsvComponents[0] << ' ' << hsvComponents[1] << ' ' << hsvComponents[2] << " }";
	return output.str();
}


void commonItems::newColor::RandomlyFluctuate(const int stdDev)
{
	static std::mt19937 generator(
		 static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	const auto allChange = std::normal_distribution<float>(0.0f, static_cast<float>(stdDev))(generator);

	std::normal_distribution<float> distribution(0.0f, static_cast<float>(stdDev) / 4.0f);
	for (auto& component: rgbComponents)
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

	deriveHsvFromRgb();
}


void commonItems::newColor::deriveHsvFromRgb()
{
	const auto r = static_cast<float>(rgbComponents[0]) / 255.0f;
	const auto g = static_cast<float>(rgbComponents[1]) / 255.0f;
	const auto b = static_cast<float>(rgbComponents[2]) / 255.0f;
	const auto xMax = std::max({r, g, b});
	const auto xMin = std::min({r, g, b});
	const auto chroma = xMax - xMin;

	auto h = 0.0f;
	if (chroma == 0.0f)
	{
		h = 0.0f;
	}
	else if (xMax == r)
	{
		h = (g - b) / chroma;
	}
	else if (xMax == g)
	{
		h = (b - r) / chroma;
		h += 2;
	}
	else if (xMax == b)
	{
		h = (r - g) / chroma;
		h += 4;
	}
	h /= 6.0f;
	if (h < 0)
	{
		h += 1.0f;
	}
	hsvComponents[0] = h;

	if (xMax == 0.0f)
	{
		hsvComponents[1] = 0.0f;
	}
	else
	{
		hsvComponents[1] = chroma / xMax;
	}
	hsvComponents[2] = xMax;
}


void commonItems::newColor::deriveRgbFromHsv()
{
	const auto [h, s, v] = hsvComponents;

	float r, g, b;
	if (s == 0) // achromatic (grey)
	{
		r = g = b = v;
	}
	else
	{
		const int sector = static_cast<int>(floor(h * 6.0f));
		const float fraction = h * 6.0f - static_cast<float>(sector);
		const float p = v * (1 - s);
		const float q = v * (1 - s * fraction);
		const float t = v * (1 - s * (1 - fraction));
		switch (sector)
		{
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			case 5:
				r = v;
				g = p;
				b = q;
				break;
			default:
				throw std::runtime_error("Hue greater than 1.0");
		}
	}

	r *= 255;
	g *= 255;
	b *= 255;

	rgbComponents = std::array<int, 3>{static_cast<int>(r), static_cast<int>(g), static_cast<int>(b)};
}


std::ostream& commonItems::operator<<(std::ostream& out, const newColor& color)
{
	out << "= { " << color.rgbComponents[0] << ' ' << color.rgbComponents[1] << ' ' << color.rgbComponents[2] << " }";
	return out;
}


commonItems::newColor commonItems::newColor::Factory::getColor(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // equals sign

	const auto token = getNextTokenWithoutMatching(theStream);
	if (token == "rgb")
	{
		const auto rgb = intList{theStream}.getInts();
		if (rgb.size() != 3)
		{
			throw std::runtime_error("Color has wrong number of components");
		}
		return newColor(std::array<int, 3>{rgb[0], rgb[1], rgb[2]});
	}
	else if (token == "hsv")
	{
		const auto hsv = doubleList{theStream}.getDoubles();
		if (hsv.size() != 3)
		{
			throw std::runtime_error("Color has wrong number of components");
		}
		return newColor(
			 std::array<float, 3>{static_cast<float>(hsv[0]), static_cast<float>(hsv[1]), static_cast<float>(hsv[2])});
	}
	else
	{
		auto actualToken = *token;
		for (auto i = actualToken.rbegin(); i != actualToken.rend(); ++i)
		{
			theStream.putback(*i);
		}
		const auto rgb = intList{theStream}.getInts();
		if (rgb.size() != 3)
		{
			throw std::runtime_error("Color has wrong number of components");
		}
		return newColor(std::array<int, 3>{rgb[0], rgb[1], rgb[2]});
	}
}
