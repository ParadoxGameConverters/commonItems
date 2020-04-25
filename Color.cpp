#include "Color.h"
#include <chrono>
#include <random>
#include <sstream>

commonItems::Color::Color() : initialized(false), c({0, 0, 0}) {}

commonItems::Color::Color(const int r, const int g, const int b)
    : initialized(true), c({r, g, b})
{
}

commonItems::Color::Color(std::istream& theStream)
    : initialized(false), c({0, 0, 0})
{
  unsigned int colorsInitialized = 0;

  auto token = getNextToken(theStream);
  while (token && (*token != "}"))
  {
    if ((token->find('=') == std::string::npos) &&
        (token->find('{') == std::string::npos))
    {
      if (token->substr(0, 1) == "\"")
      {
	token = token->substr(1, token->length() - 2);
      }
      c[colorsInitialized] = std::stoi(*token);
      colorsInitialized++;
    }
    token = getNextToken(theStream);
  }

  initialized = (colorsInitialized > 2);
}

void commonItems::Color::RandomlyFlunctuate(const int stdDev)
{
  // All three color components will go up or down by the some amount (according
  // to stdDev), and then each is tweaked a bit more (with a much smaller
  // standard deviation).
  static std::mt19937 generator(static_cast<unsigned int>(
      std::chrono::system_clock::now().time_since_epoch().count()));
  const double allChange = std::normal_distribution<double>(0.0, stdDev)(
      generator); // the amount the colors all change by
  std::normal_distribution<double> distribution(0.0, stdDev / 4.0);
  for (auto& component : c) // the component under consideration
  {
    component += lround(allChange + distribution(generator));
    if (component < 0)
    {
      component = 0;
    }
    else if (component > 255)
    {
      component = 255;
    }
  }
}

std::ostream& commonItems::operator<<(std::ostream& out,
                                      const commonItems::Color& color)
{
  out << color.c[0] << ' ' << color.c[1] << ' ' << color.c[2];
  return out;
}

void commonItems::Color::GetRGB(int& r, int& g, int& b) const
{
  r = c[0];
  g = c[1];
  b = c[2];
}

commonItems::Color::operator bool() const { return initialized; }
