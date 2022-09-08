#ifndef PARSER_H
#define PARSER_H



// A parser for reading Paradox Script.



#include "ModLoader/ModFilesystem.h"
#include <functional>
#include <map>
#include <optional>
#include <regex>



namespace commonItems
{

typedef std::function<void(const std::string&, std::istream&)> parsingFunction;
typedef std::function<void(std::istream&)> parsingFunctionStreamOnly;


void absorbBOM(std::istream& theStream);


class parser
{
  public:
	parser() = default;
	~parser() = default;
	parser(const parser&) = default;
	parser(parser&&) noexcept = default;
	parser& operator=(const parser&) = default;
	parser& operator=(parser&&) = default;

	void registerKeyword(const std::string& keyword, const parsingFunctionStreamOnly& function);
	void registerKeyword(const std::string& keyword,
		 const parsingFunction& function); // for the few keywords that need to be returned
	void registerRegex(const std::string& keyword, const parsingFunction& function);
	void IgnoreUnregisteredItems();
	void IgnoreAndLogUnregisteredItems();
	void IgnoreAndStoreUnregisteredItems(std::set<std::string>& ignored_tokens);

	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(std::string_view filename);

	// Parses a game file in either vanilla game or mods directory.
	// For example:
	//		relativePath may be "map_data/areas.txt"
	void ParseGameFile(const std::string& relative_path, const ModFilesystem& mod_fs);

	// Parses a game folder in both vanilla game and mods directory.
	// For example:
	//		relativePath may be "common/governments"
	//		extensions may be "txt", "text"
	void ParseGameFolder(const std::string& relative_path, const ModFilesystem& mod_fs, const std::set<std::string>& extensions, bool recursive);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);


  private:
	inline bool tryToMatchAgainstKeywords(const std::string& toReturn, const std::string& strippedLexeme, bool isLexemeQuoted, std::istream& theStream);
	inline bool tryToMatchAgainstRegexes(const std::string& toReturn, const std::string& strippedLexeme, bool isLexemeQuoted, std::istream& theStream);

	std::map<std::string, parsingFunctionStreamOnly> registeredKeywordStringsStreamOnly;
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;
};

} // namespace commonItems



#endif // PARSER_H