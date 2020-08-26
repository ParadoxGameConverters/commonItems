#ifndef NEW_COLOR_H
#define NEW_COLOR_H



#include "Parser.h"
#include <array>
#include <iostream>



namespace commonItems
{

class newColor
{
  public:
	class Factory;
	newColor() = default;
	newColor(const std::array<int, 3> rgbComponents): rgbComponents(rgbComponents) { deriveHsvFromRgb(); }
	newColor(const std::array<float, 3> hsvComponents): hsvComponents(hsvComponents) { deriveRgbFromHsv(); }

	bool operator==(const newColor& rhs) const;
	bool operator!=(const newColor& rhs) const;

	[[nodiscard]] const auto& getRgbComponents() const { return rgbComponents; }
	[[nodiscard]] const auto& getHsvComponents() const { return hsvComponents; }
	[[nodiscard]] const auto& r() const { return rgbComponents[0]; }
	[[nodiscard]] const auto& g() const { return rgbComponents[1]; }
	[[nodiscard]] const auto& b() const { return rgbComponents[2]; }
	[[nodiscard]] const auto& h() const { return hsvComponents[0]; }
	[[nodiscard]] const auto& s() const { return hsvComponents[1]; }
	[[nodiscard]] const auto& v() const { return hsvComponents[2]; }

	[[nodiscard]] std::string outputRgb() const;
	[[nodiscard]] std::string outputHsv() const;

	// All three color components will go up or down by the some amount (according to stdDev), and then each is tweaked a
	// bit more (with a much smaller standard deviation).
	void RandomlyFluctuate(int stdDev);

	friend std::ostream& operator<<(std::ostream&, const newColor&);

  private:
	void deriveHsvFromRgb();
	void deriveRgbFromHsv();

	std::array<int, 3> rgbComponents{0, 0, 0};
	std::array<float, 3> hsvComponents{0.0f, 0.0f, 0.0f};
};


std::ostream& operator<<(std::ostream&, const newColor&);


class newColor::Factory: parser
{
  public:
	static newColor getColor(std::istream& theStream);
};

} // namespace commonItems



#endif // NEW_COLOR_H