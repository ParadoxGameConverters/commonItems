#ifndef NEW_COLOR_H
#define NEW_COLOR_H


#pragma message("Warning: Use Color.h instead of newColor.h")



#include "Color.h"



namespace commonItems
{

class newColor: public Color
{
  public:
	class Factory;
	newColor() = default;
	explicit newColor(std::array<int, 3> rgbComponents): Color(rgbComponents) {}
	explicit newColor(std::array<float, 3> hsvComponents): Color(hsvComponents) {}
	explicit newColor(Color rhs)
	{
		rgbComponents = rhs.getRgbComponents();
		hsvComponents = rhs.getHsvComponents();
	}
	newColor& operator=(Color rhs)
	{
		rgbComponents = rhs.getRgbComponents();
		hsvComponents = rhs.getHsvComponents();
		return *this;
	}
};

class newColor::Factory: public Color::Factory
{
};

} // namespace commonItems



#endif // NEW_COLOR_H