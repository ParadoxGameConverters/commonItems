#ifndef PARSER_H
#define PARSER_H



#include <functional>
#include <map>
#include <optional>
#include <regex>
#include <variant>



namespace commonItems
{

using parsingFunction = std::function<void(const std::string&, std::istream&)>;
using parsingFunctionStreamOnly = std::function<void(std::istream&)>;



void absorbBOM(std::istream& theStream);



struct registeredRegex {
	std::regex regex;
	parsingFunction function;
	bool match(const std::string& lexeme, std::istream& theStream) const;
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const;
};
struct registeredRegexStreamOnly {
	std::regex regex;
	parsingFunctionStreamOnly function;
	bool match(const std::string& lexeme, std::istream& theStream) const;
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const;
};
struct registeredMatcher {
	bool (*matcher)(std::string_view);
	parsingFunction function;
	bool match(const std::string& lexeme, std::istream& theStream) const;
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const;
};
struct registeredMatcherStreamOnly {
	bool (*matcher)(std::string_view);
	parsingFunctionStreamOnly function;
	bool match(const std::string& lexeme, std::istream& theStream) const;
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const;
};

using registeredVariant = std::variant<registeredMatcher, registeredMatcherStreamOnly, registeredRegex, registeredRegexStreamOnly>;

struct CallMatch
{
	const std::string& lexeme;
	std::istream& theStream;
	auto operator()(const registeredMatcher& reg) const { return reg.match(lexeme, theStream); }
	auto operator()(const registeredMatcherStreamOnly& reg) const { return reg.match(lexeme, theStream); }
	auto operator()(const registeredRegex& reg) const { return reg.match(lexeme, theStream); }
	auto operator()(const registeredRegexStreamOnly& reg) const { return reg.match(lexeme, theStream); }
};
struct CallMatchStripped
{
	const std::string& lexeme;
	const std::string& strippedLexeme;
	std::istream& theStream;
	auto operator()(const registeredMatcher& reg) const { return reg.matchStripped(lexeme, strippedLexeme, theStream); }
	auto operator()(const registeredMatcherStreamOnly& reg) const { return reg.matchStripped(lexeme, strippedLexeme, theStream); }
	auto operator()(const registeredRegex& reg) const { return reg.matchStripped(lexeme, strippedLexeme, theStream); }
	auto operator()(const registeredRegexStreamOnly& reg) const { return reg.matchStripped(lexeme, strippedLexeme, theStream); }
};
	

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
	void registerMatcher(bool (*matcher)(std::string_view), const parsingFunctionStreamOnly& function);
	void registerMatcher(bool (*matcher)(std::string_view), const parsingFunction& function);
	void registerRegex(const std::string& keyword, const parsingFunctionStreamOnly& function);
	void registerRegex(const std::string& keyword, const parsingFunction& function);
	
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

	std::map<std::string, parsingFunctionStreamOnly> registeredKeywordStringsStreamOnly;
	std::map<std::string, parsingFunction> registeredKeywordStrings;

	std::vector<registeredVariant> registeredRegexesAndMatchers;
};

} // namespace commonItems



#endif // PARSER_H