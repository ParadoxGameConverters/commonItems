#ifndef PARSER_H
#define PARSER_H



#include <functional>
#include <map>
#include <optional>
#include <regex>


#include <any>
#include <variant>

#include "ctre.hpp"

namespace commonItems
{

typedef std::function<void(const std::string&, std::istream&)> parsingFunction;




void absorbBOM(std::istream& theStream);


class parser
{
  public:

	
	parser() = default;

	virtual ~parser() = default;
	parser(const parser&) = default;
	parser(parser&&) noexcept = default;
	parser& operator=(const parser&) = default;
	parser& operator=(parser&&) = default;

	void registerKeyword(const std::string& keyword, const parsingFunction& function);
	void registerRegex(const std::string& keyword, const parsingFunction& function);

	static const int CATCHALL = 0; // can't replace it with enum, wouldn't work with CTReParser
	void registerRegex(int regexId, const parsingFunction& function);
	[[nodiscard]] bool matchCTRegex(int regexId, std::string_view subject) const;

	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(const std::string& filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

  protected:
	[[nodiscard]] constexpr bool catchallRegexMatch(std::string_view sv) const noexcept { return ctre::match<catchall>(sv); }
	// compile time regexes, cool stuff
	// catchall:
	//		We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already
	//		drop
	//		in the parser.
	static constexpr ctll::fixed_string catchall{R"([^=^{^}]+|".+")"};

  private:
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;

	std::vector<std::pair<const int, parsingFunction>> registeredCompileTimeRegexes;

	
	
};

} // namespace commonItems



#endif // PARSER_H