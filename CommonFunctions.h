#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H
#include <string>

std::string trimPath(const std::string& fileName);
std::string trimExtension(const std::string& fileName);
std::string replaceCharacter(std::string fileName, char character);
std::string cardinalToOrdinal(int cardinal);
[[deprecated("Use cardinalToOrdinal (lower case 'c')")]] inline std::string CardinalToOrdinal(const int cardinal) { return cardinalToOrdinal(cardinal); }

#endif // COMMON_FUNCTIONS_H