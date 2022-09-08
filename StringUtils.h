#ifndef STRING_UTILS_H
#define STRING_UTILS_H



// Utilities for modifying strings.



#include <string>



namespace commonItems
{

bool IsQuoted(const std::string& string);
std::string remQuotes(const std::string& in);
std::string addQuotes(const std::string& in);

} // namespace commonItems



#endif // STRING_UTILS_H
