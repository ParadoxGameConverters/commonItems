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

	enum class ctRegex{CATCHALL};
	void registerCTRegex(ctRegex regexEnum, const parsingFunction& function);

	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(const std::string& filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

	
	bool matchCTRegex(ctRegex regex, std::string_view subject);

  protected:
	
	static constexpr ctll::fixed_string catchall{R"([^=^{^}]+|".+")"};
	constexpr auto catchallRegexMatch(std::string_view sv) const noexcept { return ctre::match<catchall>(sv); }


  private:
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;

	
	// compile time regexes
	
	std::vector<std::pair<ctRegex, parsingFunction>> registeredCompileTimeRegexes;

	
	//static constexpr ctll::fixed_string date{R"(\d+)"};
	//constexpr auto dateMatch(std::string_view sv) noexcept { return ctre::match<date>(sv); }
	//static constexpr ctll::fixed_string quotedDate{R"("\d+")"};
	//constexpr auto quotedDateMatch(std::string_view sv) noexcept { return ctre::match<quotedDate>(sv); }
	//static constexpr ctll::fixed_string number{R"([0-9]+)"};
	//constexpr auto numberMatch(std::string_view sv) noexcept { return ctre::match<number>(sv); }
};

} // namespace commonItems



#endif // PARSER_H