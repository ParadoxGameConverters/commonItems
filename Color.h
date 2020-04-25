#ifndef COLOR_H
#define COLOR_H



#include "newParser.h"
#include <array>
#include <iostream>



namespace commonItems
{

class Color: parser
{
  public:
	class Factory;
	Color() = default;
	Color(const int r, const int g, const int b): initialized(true), c({r, g, b}) {}
	explicit Color(std::istream& theStream);

	void GetRGB(int& r, int& g, int& b) const;

	[[nodiscard]] int r() const { return c[0]; }
	[[nodiscard]] int g() const { return c[1]; }
	[[nodiscard]] int b() const { return c[2]; }

	explicit operator bool() const { return initialized; } // Returns true if the color has been initialized with an RGB triplet.

	// All three color components will go up or down by the some amount (according to stdDev), and then each is tweaked a
	// bit more (with a much smaller standard deviation).
	[[deprecated]] void RandomlyFlunctuate(const int stdDev) { RandomlyFluctuate(stdDev); }
	void RandomlyFluctuate(int stdDev);

	friend std::ostream& operator<<(std::ostream&, const Color&);

  private:
	bool initialized = false;
	std::array<int, 3> c{0, 0, 0};
};

	
std::ostream& operator<<(std::ostream&, const Color&);


class Color::Factory: parser
{
  public:
	static Color getColor(std::istream& theStream);
};

} // namespace commonItems



#endif