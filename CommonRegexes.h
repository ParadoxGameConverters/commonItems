#ifndef COMMON_REGEXES_H
#define COMMON_REGEXES_H



namespace commonItems
{
// catchall:
//		We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already drop
//		in the parser.
extern const char* catchallRegex;

// numbers
extern const char* integerRegex;
extern const char* quotedIntegerRegex;
extern const char* floatRegex;
extern const char* quotedFloatRegex;

// strings
extern const char* stringRegex;
extern const char* quotedStringRegex;

} // namespace commonItems

#endif
