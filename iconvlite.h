/*
iconvlite.h
Iconv Lite
Simple cpp functions to convert strings from cp1251 to utf8 and from utf8 to cp1251
*/



#ifndef ICONVLITE_H
#define ICONVLITE_H



#include <string>



void cp2utf1(char* out, const char* in);
std::string cp2utf(const std::string& input);
bool convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t inputLength);
std::string utf2cp(const std::string& input);



#endif // ICONVLITE_H