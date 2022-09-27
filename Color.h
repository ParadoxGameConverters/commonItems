#ifndef COLOR_H
#define COLOR_H



// Represents a Paradox - defined color.
//
// Can be directly created in either the RGB or HSV color spaces.
//
// Can be imported in :
// * Unspecified with ints(becomes RGB) - "= { 64 128 128 }"
// * Unspecified with floats(becomes RGB) - "= { 0.5 0.9 0.1 }"
// * RGB - "= rgb { 64 128 128 }"
// * Hex - "= hex { 408080 }"
// * HSV - "= hsv { 0.5 0.5 0.5 }"
// * HSV360 - "= hsv360 { 180 50 50 }"
// * Name(requires caching definitions for the named colors in advance) - "= dark_moderate_cyan"
//
// Can be output in :
// * unspecified(rgb) - "= { 64 128 128 }"
// * RGB - "= rgb { 64 128 128 }"
// * hex - "= hex { 408080 }"
// * HSV - "= hsv { 0.5 0.5 0.5 }"
// * HSV360 - "= hsv360 { 180 50 50 }"
//
// The individual components can be accessed in both RGB and HSV color spaces, equality and inequality can be checked, the color cache can be reviewed and
// modified, and colors can have a random fluctuation be applied automatically.



#include "Parser.h"
#include <array>
#include <iostream>



namespace commonItems
{

class Color
{
  public:
	class Factory;
	Color() = default;
	explicit Color(std::array<int, 3> rgbComponents): rgbComponents(rgbComponents) { deriveHsvFromRgb(); }
	explicit Color(std::array<float, 3> hsvComponents): hsvComponents(hsvComponents) { deriveRgbFromHsv(); }

	bool operator==(const Color& rhs) const;
	bool operator!=(const Color& rhs) const;

	[[nodiscard]] const auto& getRgbComponents() const { return rgbComponents; }
	[[nodiscard]] const auto& getHsvComponents() const { return hsvComponents; }
	[[nodiscard]] const auto& r() const { return rgbComponents[0]; }
	[[nodiscard]] const auto& g() const { return rgbComponents[1]; }
	[[nodiscard]] const auto& b() const { return rgbComponents[2]; }
	[[nodiscard]] const auto& h() const { return hsvComponents[0]; }
	[[nodiscard]] const auto& s() const { return hsvComponents[1]; }
	[[nodiscard]] const auto& v() const { return hsvComponents[2]; }

	[[nodiscard]] std::string outputRgb() const;
	[[nodiscard]] std::string outputHex() const;
	[[nodiscard]] std::string outputHsv() const;
	[[nodiscard]] std::string outputHsv360() const;

	// All three color components will go up or down by the some amount (according to stdDev), and then each is tweaked a
	// bit more (with a much smaller standard deviation).
	void RandomlyFluctuate(int stdDev);

	friend std::ostream& operator<<(std::ostream& out, const Color& color);

  private:
	void deriveHsvFromRgb();
	void deriveRgbFromHsv();

	std::array<int, 3> rgbComponents{0, 0, 0};
	std::array<float, 3> hsvComponents{0.0F, 0.0F, 0.0F};
};


std::ostream& operator<<(std::ostream& out, const Color& color);


class Color::Factory: parser
{
  public:
	[[nodiscard]] Color getColor(std::istream& theStream) const;
	[[nodiscard]] Color getColor(const std::string& colorName) const;
	[[nodiscard]] const auto& getRegisteredColors() const { return namedColors; }

	void addNamedColor(const std::string& name, const Color& color);
	void addNamedColor(const std::string& name, std::istream& theStream);
	void addNamedColorMap(const std::map<std::string, Color>& colorMap);

	void clear() { namedColors.clear(); }

  private:
	std::unordered_map<std::string, Color> namedColors;
};

} // namespace commonItems



#endif // COLOR_H