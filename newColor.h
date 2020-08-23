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
	enum class ColorSpaces
	{
		UNSPECIFIED,
		RGB,
		HSV
	};
	newColor() = default;
	newColor(const std::array<int, 3> components, ColorSpaces colorSpace = ColorSpaces::UNSPECIFIED):
		 components(components), colorSpace(colorSpace)
	{
	}

	bool operator==(const newColor& rhs) const;
	bool operator!=(const newColor& rhs) const;

	[[nodiscard]] const auto& getComponents() const { return components; }
	[[nodiscard]] const auto& getColorSpace() const { return colorSpace; }

	// All three color components will go up or down by the some amount (according to stdDev), and then each is tweaked a
	// bit more (with a much smaller standard deviation).
	void RandomlyFluctuate(int stdDev);

	friend std::ostream& operator<<(std::ostream&, const newColor&);

  private:
	std::array<int, 3> components{0, 0, 0};
	ColorSpaces colorSpace = ColorSpaces::UNSPECIFIED;
};


std::ostream& operator<<(std::ostream&, const newColor&);


class newColor::Factory: parser
{
  public:
	static newColor getColor(std::istream& theStream);
};

} // namespace commonItems



#endif // NEW_COLOR_H