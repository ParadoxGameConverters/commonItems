#include "Color.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include "StringUtils.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <random>
#include <regex>
#include <sstream>



bool commonItems::Color::operator==(const Color& rhs) const
{
	return rgbComponents == rhs.rgbComponents;
}


bool commonItems::Color::operator!=(const Color& rhs) const
{
	return !(*this == rhs);
}


std::string commonItems::Color::outputRgb() const
{
	return "= rgb { " + std::to_string(rgbComponents[0]) + ' ' + std::to_string(rgbComponents[1]) + ' ' + std::to_string(rgbComponents[2]) + " }";
}


std::string commonItems::Color::outputHex() const
{
	std::stringstream output;
	output << "= hex { ";
	output << std::setw(2) << std::setfill('0') << std::hex << rgbComponents[0];
	output << std::setw(2) << std::setfill('0') << std::hex << rgbComponents[1];
	output << std::setw(2) << std::setfill('0') << std::hex << rgbComponents[2];
	output << " }";
	return output.str();
}


std::string commonItems::Color::outputHsv() const
{
	std::stringstream output;
	output << std::setprecision(2);
	output << "= hsv { " << hsvComponents[0] << ' ' << hsvComponents[1] << ' ' << hsvComponents[2] << ' ';
	if (alpha)
		output << *alpha << ' ';
	output << "}";
	return output.str();
}


std::string commonItems::Color::outputHsv360() const
{
	std::stringstream output;
	output << std::setprecision(3);
	output << "= hsv360 { " << hsvComponents[0] * 360;
	output << std::setprecision(2);
	output << ' ' << hsvComponents[1] * 100 << ' ' << hsvComponents[2] * 100 << " }";
	return output.str();
}


void commonItems::Color::RandomlyFluctuate(const int stdDev)
{
	static std::mt19937 generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

	const auto allChange = std::normal_distribution(0.0F, static_cast<float>(stdDev))(generator);

	std::normal_distribution distribution(0.0F, static_cast<float>(stdDev) / 4.0F);
	for (auto& component: rgbComponents)
	{
		component += static_cast<int>(std::round(allChange + distribution(generator)));
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


void commonItems::Color::deriveHsvFromRgb()
{
	const auto r = static_cast<float>(rgbComponents[0]) / 255.0F;
	const auto g = static_cast<float>(rgbComponents[1]) / 255.0F;
	const auto b = static_cast<float>(rgbComponents[2]) / 255.0F;
	const auto xMax = std::max({r, g, b});
	const auto xMin = std::min({r, g, b});
	const auto chroma = xMax - xMin;

	auto h = 0.0F;
	if (chroma == 0.0F)
	{
		h = 0.0F;
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
	h /= 6.0F;
	if (h < 0)
	{
		h += 1.0F;
	}
	hsvComponents[0] = h;

	if (xMax == 0.0F)
	{
		hsvComponents[1] = 0.0F;
	}
	else
	{
		hsvComponents[1] = chroma / xMax;
	}
	hsvComponents[2] = xMax;
}


void commonItems::Color::deriveRgbFromHsv()
{
	auto [h, s, v] = hsvComponents;

	float r = 0.0F;
	float g = 0.0F;
	float b = 0.0F;
	if (s == 0.0F) // achromatic (grey)
	{
		r = g = b = v;
	}
	else
	{
		if (h >= 1.0F)
			h = 0.0F;
		const int sector = static_cast<int>(std::floor(h * 6.0F));
		const float fraction = h * 6.0F - static_cast<float>(sector);
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

	rgbComponents = std::array{static_cast<int>(r), static_cast<int>(g), static_cast<int>(b)};
}


std::ostream& commonItems::operator<<(std::ostream& out, const Color& color)
{
	out << "= { " << color.rgbComponents[0] << ' ' << color.rgbComponents[1] << ' ' << color.rgbComponents[2] << " }";
	return out;
}


commonItems::Color commonItems::Color::Factory::getColor(std::istream& theStream) const
{
	getNextTokenWithoutMatching(theStream); // equals sign

	auto token = getNextTokenWithoutMatching(theStream);
	if (token)
		token = remQuotes(*token);
	if (token == "rgb")
	{
		const auto rgb = getDoubles(theStream);
		if (rgb.size() != 3)
		{
			throw std::runtime_error("Color has wrong number of components");
		}
		return Color(std::array{static_cast<int>(rgb[0]), static_cast<int>(rgb[1]), static_cast<int>(rgb[2])});
	}
	if (token == "hex")
	{
		const auto hex = singleString{theStream}.getString();
		if (hex.size() != 6)
		{
			throw std::runtime_error("Color has wrong number of digits");
		}
		const auto r = std::stoi(hex.substr(0, 2), nullptr, 16);
		const auto g = std::stoi(hex.substr(2, 2), nullptr, 16);
		const auto b = std::stoi(hex.substr(4, 2), nullptr, 16);
		return Color(std::array{r, g, b});
	}
	if (token == "hsv" || token == "HSV")
	{
		const auto hsv = doubleList{theStream}.getDoubles();
		if (hsv.size() == 3)
			return Color(std::array{static_cast<float>(hsv[0]), static_cast<float>(hsv[1]), static_cast<float>(hsv[2])});
		if (hsv.size() == 4)
			return Color(std::array{static_cast<float>(hsv[0]), static_cast<float>(hsv[1]), static_cast<float>(hsv[2])}, static_cast<float>(hsv[3]));

		throw std::runtime_error("Color has wrong number of components");
	}
	if (token == "hsv360")
	{
		const auto hsv = doubleList{theStream}.getDoubles();
		if (hsv.size() != 3)
		{
			throw std::runtime_error("Color has wrong number of components");
		}
		return Color(std::array{static_cast<float>(hsv[0] / 360.0), static_cast<float>(hsv[1] / 100.0), static_cast<float>(hsv[2] / 100.0)});
	}
	if (std::regex_match(*token, std::regex(catchallRegex)))
	{
		const auto color = namedColors.find(*token);
		if (color == namedColors.end())
		{
			throw std::runtime_error(*token + " was not a cached color");
		}
		else
		{
			return color->second;
		}
	}

	auto actualToken = *token;
	for (auto i = actualToken.rbegin(); i != actualToken.rend(); ++i)
	{
		theStream.putback(*i);
	}
	const auto questionableList = stringOfItem(theStream).getString();
	if (questionableList.find('.') != std::string::npos)
	{
		// This is a double list.
		auto doubleStream = std::stringstream(questionableList);
		auto rgb = getDoubles(doubleStream);
		if (rgb.size() == 3) // This is not HSV, this is RGB doubles. Multiply by 255 and round to get normal rgb.
			return Color(std::array{static_cast<int>(std::round(rgb[0] * 255.0)),
				 static_cast<int>(std::round(rgb[1] * 255.0)),
				 static_cast<int>(std::round(rgb[2] * 255.0))});
		if (rgb.size() == 4) // this is a RGBA double situation. We shouldn't touch alpha.
			return Color(std::array{static_cast<int>(std::round(rgb[0] * 255.0)),
								  static_cast<int>(std::round(rgb[1] * 255.0)),
								  static_cast<int>(std::round(rgb[2] * 255.0))},
				 static_cast<float>(rgb[3]));

		throw std::runtime_error("Color has wrong number of components");
	}
	else
	{
		// integer list
		auto integerStream = std::stringstream(questionableList);
		auto rgb = getInts(integerStream);
		if (rgb.size() == 3)
			return Color(std::array{rgb[0], rgb[1], rgb[2]});
		if (rgb.size() == 4)
		{
			// This is an absolute RGBA list, eg. { 128 128 255 1 }. Maybe.
			return Color(std::array{rgb[0], rgb[1], rgb[2]}, static_cast<float>(rgb[3]));
		}

		throw std::runtime_error("Color has wrong number of components");
	}
}


commonItems::Color commonItems::Color::Factory::getColor(const std::string& colorName) const
{
	const auto color = namedColors.find(colorName);
	if (color == namedColors.end())
	{
		throw std::runtime_error(colorName + " was not a cached color");
	}

	return color->second;
}


void commonItems::Color::Factory::addNamedColor(const std::string& name, const Color& color)
{
	namedColors[name] = color;
}

void commonItems::Color::Factory::addNamedColor(const std::string& name, std::istream& theStream)
{
	namedColors[name] = getColor(theStream);
}

void commonItems::Color::Factory::addNamedColorMap(const std::map<std::string, Color>& colorMap)
{
	for (const auto& [first, second]: colorMap)
		namedColors[first] = second;
}
