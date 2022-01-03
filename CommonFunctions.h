#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H
#include <string>

namespace commonItems
{
constexpr char utf8BOM[] = "\xEF\xBB\xBF";
}

std::string trimPath(const std::string& fileName);
std::string trimExtension(const std::string& fileName);
std::string getPath(const std::string& fileName);
std::string getExtension(const std::string& fileName);
std::string replaceCharacter(std::string fileName, char character);
std::string cardinalToOrdinal(int cardinal);
[[deprecated("Use cardinalToOrdinal (lower case 'c')")]] inline std::string CardinalToOrdinal(const int cardinal)
{
	return cardinalToOrdinal(cardinal);
}
std::string cardinalToRoman(int number);
std::string normalizeStringPath(const std::string& stringPath);

#endif // COMMON_FUNCTIONS_H