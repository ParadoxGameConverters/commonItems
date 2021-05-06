#include "CommonRegexes.h"



namespace commonItems
{
// catchall:
//		We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already drop
//		in the parser.
const char* catchallRegex = R"([^=^{^}]+|".+")";

// numbers
const char* integerRegex = R"(-?\d+)";
const char* quotedIntegerRegex = R"("-?\d+")";
const char* floatRegex = R"(-?\d+(.\d+)?)";
const char* quotedFloatRegex = R"("-?\d+(.\d+)?")";

// strings
const char* stringRegex = R"([^[:s:]^=^\{^\}^\"]+)";
const char* quotedStringRegex = R"("[^\n^=^\{^\}^\"]+")";

} // namespace commonItems
