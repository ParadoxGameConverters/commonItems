#ifndef PARSER_H
#define PARSER_H



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
	void registerKeyword(const std::string& keyword, const parsingFunction& function); // for the few keywords that need to be returned
	// for compile time regex matchers, but will work with any function that has the same return and argument type
	void registerMatcher(bool (*matcher)(std::string_view), const parsingFunction& function);
	
	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(std::string_view filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

	
  private:
	inline bool tryToMatchAgainstKeywords(const std::string& toReturn,
		 const std::string& strippedLexeme,
		 bool isLexemeQuoted,
		 std::istream& theStream);
	inline bool tryToMatchAgainstRegexes(const std::string& toReturn,
		 const std::string& strippedLexeme,
		 bool isLexemeQuoted,
		 std::istream& theStream);

	std::map<std::string, parsingFunctionStreamOnly> registeredKeywordStringsStreamOnly;
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<bool (*)(std::string_view), parsingFunction>> registeredMatchers;
};

} // namespace commonItems



#endif // PARSER_H