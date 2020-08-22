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
	newColor(const std::array<int, 3> components): components(components) {}

	[[nodiscard]] const auto& getComponents() const { return components; }

	// All three color components will go up or down by the some amount (according to stdDev), and then each is tweaked a
	// bit more (with a much smaller standard deviation).
	void RandomlyFluctuate(int stdDev);

	friend std::ostream& operator<<(std::ostream&, const newColor&);

  private:
	std::array<int, 3> components{0, 0, 0};
};


std::ostream& operator<<(std::ostream&, const newColor&);


class newColor::Factory: parser
{
  public:
	static newColor getColor(std::istream& theStream);
};

} // namespace commonItems



#endif // NEW_COLOR_H