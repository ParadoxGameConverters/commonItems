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
	newColor(std::array<int, 3> rgbComponents): Color(rgbComponents) {}
	newColor(std::array<float, 3> hsvComponents): Color(hsvComponents) {}
	newColor(Color rhs)
	{
		rgbComponents = rhs.getRgbComponents();
		hsvComponents = rhs.getHsvComponents();
	}
};

class newColor::Factory: public Color::Factory
{
};

} // namespace commonItems



#endif // NEW_COLOR_H