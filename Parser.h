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

	~parser() = default;
	parser(const parser&) = default;
	parser(parser&&) noexcept = default;
	parser& operator=(const parser&) = default;
	parser& operator=(parser&&) = default;

	void registerKeyword(const std::string& keyword, const parsingFunction& function);
	void registerRegex(const std::string& keyword, const parsingFunction& function);

	static const unsigned short CATCHALL = 0;
	void registerRegex(const unsigned short regexId, const parsingFunction& function);

	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(const std::string& filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);


	[[nodiscard]] bool matchCTRegex(unsigned short regexId, std::string_view subject) const;

  protected:
	// compile time regexes, cool stuff
	[[nodiscard]] constexpr bool catchallRegexMatch(std::string_view sv) const noexcept { return ctre::match<catchall>(sv); }


  private:
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;

	
	// compile time regexes
	static constexpr ctll::fixed_string catchall{R"([^=^{^}]+|".+")"};
	std::vector<std::pair<const unsigned short, parsingFunction>> registeredCompileTimeRegexes;
};

} // namespace commonItems



#endif // PARSER_H