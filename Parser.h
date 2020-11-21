#ifndef PARSER_H
#define PARSER_H



#include <functional>
#include <map>
#include <optional>
#include <regex>
#include "ctre.hpp"



namespace commonItems
{

typedef std::function<void(const std::string&, std::istream&)> parsingFunction;


// compile time regexes, cool stuff
// catchall:
//		We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already
//		drop
//		in the parser.
static constexpr ctll::fixed_string catchall{R"([^=^{^}]+|".+ ")"};
[[nodiscard]] constexpr bool catchallRegexMatch(std::string_view sv) noexcept
{
	return ctre::match<catchall>(sv);
}



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

	void registerKeyword(const std::string& keyword, const parsingFunction& function);
	void registerRegex(const std::string& keyword, const parsingFunction& function);
	// for compile time regex metchers, but will work with any function that matches the return and argument type
	void registerMatcher(bool (*matcher)(std::string_view), const parsingFunction& function);
	
	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(const std::string& filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

	
  private:
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;

	std::vector<std::pair<bool (*)(std::string_view), parsingFunction>> registeredCompileTimeRegexes;
};

} // namespace commonItems



#endif // PARSER_H