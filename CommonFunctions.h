#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H



// A handful of helpful commonly-used functions.



#include <string>



namespace commonItems
{

constexpr auto utf8BOM = "\xEF\xBB\xBF";

} // namespace commonItems


// Given a file with path included (such as '/this/is/a/path.txt' or 'c:\this\is\a\path.txt'), returns the part that's just the filename ('path.txt')
[[deprecated("Use std::filesystem::path::filename()")]] std::string trimPath(const std::string& fileName);


// Given a file with path included (such as '/this/is/a/path.txt' or 'c:\this\is\a\path.txt'), returns the part that's just the path ('/this/is/a/' or
// 'c:\this\is\a\')
[[deprecated("Use std::filesystem::path::parent_path()")]] std::string getPath(const std::string& fileName);


// Given a filename with an extension (such as 'file.extension' or 'file.name.with.extension'), returns the filename ('file' or 'file.name.with')
[[deprecated("Use std::filesystem::path::stem()")]] std::string trimExtension(const std::string& fileName);


// Given a filename with an extension (such as 'file.extension' or 'file.name.with.extension'), returns the extension ('extension')
[[deprecated("Use std::filesystem::path::extension()")]] std::string getExtension(const std::string& fileName);


// Given a string (such as 'a file name.eu4'), replaces all instances of the specified character (such as ' ') with underscores (resulting in 'a_file_name.eu4')
std::string replaceCharacter(std::string fileName, char character);


// Given a cardinal number (1, 2, 15), returns the equivalent ordinal word ending ('st', 'nd', 'th') for appending to the numbers ('1st', '2nd', '15th')
std::string cardinalToOrdinal(int cardinal);
[[deprecated("Use cardinalToOrdinal (lower case 'c')")]] inline std::string CardinalToOrdinal(const int cardinal)
{
	return cardinalToOrdinal(cardinal);
}


// Given a number (3, 12, 2020), returns the number in roman numerals ('III', 'XII', 'MMXX')
std::string cardinalToRoman(int number);


// Given a path, normalizes it in a standard way for all converters that all supported Paradox games will recognize (by replacing all spaces, dashes, and other
// weird characters (<, >, :, ?...) with underscores, and by converting entire string into ASCII)
std::string normalizeStringPath(const std::string& stringPath);



#endif // COMMON_FUNCTIONS_H