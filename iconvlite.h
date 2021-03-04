/*
iconvlite.h
Iconv Lite
Simple cpp functions to convert strings from cp1251 to utf8 and from utf8 to cp1251
*/



#ifndef ICONVLITE_H
#define ICONVLITE_H



#include <string>



namespace commonItems
{

void cp2utf1(char* out, const char* in);
std::string cp2utf(const std::string& input);
bool convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t inputLength);
std::string utf2cp(const std::string& input);

} // namespace commonItems



[[deprecated("Use commonItems::cp2utf1")]] inline void cp2utf1(char* out, const char* in) { commonItems::cp2utf1(out, in); }
[[deprecated("Use commonItems::cp2utf")]] inline std::string cp2utf(const std::string& input) { return commonItems::cp2utf(input); }
[[deprecated("Use commonItems::convert_utf8_to_windows1251")]] inline bool convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t inputLength) { return commonItems::convert_utf8_to_windows1251(utf8, windows1251, inputLength); }
[[deprecated("Use commonItems::utf2cp")]] inline std::string utf2cp(const std::string& input) { return commonItems::utf2cp(input); }


#endif // ICONVLITE_H