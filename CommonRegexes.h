#ifndef COMMON_REGEXES_H
#define COMMON_REGEXES_H



namespace commonItems
{
// catchall:
//		We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already drop
//		in the parser.
static inline const char* catchallRegex = R"([^=^{^}]+|".+")";

// numbers
static inline const char* integerRegex = R"(-?\d+)";
static inline const char* quotedIntegerRegex = R"("-?\d+")";
static inline const char* floatRegex = R"(-?\d+(.\d+)?)";
static inline const char* quotedFloatRegex = R"("-?\d+(.\d+)?")";

// strings
static inline const char* stringRegex = R"([^[:s:]^=^\{^\}^\"]+)";
static inline const char* quotedStringRegex = R"("[^\n^=^\{^\}^\"]+")";

} // namespace commonItems

#endif
