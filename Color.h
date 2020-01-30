#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <iostream>
#include <memory>
#include "newParser.h"

class Object;

namespace commonItems
{
	class Color: parser
	{
		public:
			Color();
			Color(int r, int g, int b);
			explicit Color(std::shared_ptr<Object> colorObject);
			explicit Color(std::istream& theStream);

			void RandomlyFlunctuate(int stdDev);

			friend std::ostream& operator<<(std::ostream&, const Color&);

			void GetRGB(int& r, int& g, int& b) const;
			[[nodiscard]] int r() const { return c[0]; }
			[[nodiscard]] int g() const { return c[1]; }
			[[nodiscard]] int b() const { return c[2]; }

			operator bool() const; // Returns true if the color has been initialized with an RGB triplet.

		private:
			bool initialized;
			std::array<int, 3> c;
	};
	std::ostream& operator<<(std::ostream&, const Color&);
}



#endif