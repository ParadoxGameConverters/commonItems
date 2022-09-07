#ifndef COMMON_REGEXES_H
#define COMMON_REGEXES_H



// A handful of helpful commonly-used regexes.



namespace commonItems
{

// catchall:
//		We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already
//		drop in the parser.
inline constexpr const char* catchallRegex = R"([^=^{^}]+|".+")";

// numbers
inline constexpr const char* integerRegex = R"(-?\d+)";
inline constexpr const char* quotedIntegerRegex = R"("-?\d+")";
inline constexpr const char* floatRegex = R"(-?\d+(.\d+)?)";
inline constexpr const char* quotedFloatRegex = R"("-?\d+(.\d+)?")";

// strings
inline constexpr const char* stringRegex = R"([^[:s:]^=^\{^\}^\^\[^\]"]+)";
inline constexpr const char* quotedStringRegex = R"("[^\n"]+")";

// dates
inline constexpr const char* dateRegex = R"(\d+[.]\d+[.]\d+)";

} // namespace commonItems



#endif
