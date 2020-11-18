#ifndef COLOR_H
#define COLOR_H



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

	friend std::ostream& operator<<(std::ostream&, const Color&);

  private:
	void deriveHsvFromRgb();
	void deriveRgbFromHsv();

	std::array<int, 3> rgbComponents{0, 0, 0};
	std::array<float, 3> hsvComponents{0.0f, 0.0f, 0.0f};
};


std::ostream& operator<<(std::ostream&, const Color&);


class Color::Factory: public parser
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