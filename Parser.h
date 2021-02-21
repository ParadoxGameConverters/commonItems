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

using matcherFunction = std::function<bool(const std::string&)>;


void absorbBOM(std::istream& theStream);


struct registeredFunction {
	parsingFunction function;
	void execute(const std::string& lexeme, std::istream& theStream) const;
};
struct registeredFunctionStreamOnly {
	parsingFunctionStreamOnly function;
	void execute(std::istream& theStream) const;
};

using parsingFunctionVariant = std::variant<registeredFunction, registeredFunctionStreamOnly>;

struct CallExecute
{
	const std::string& lexeme;
	std::istream& theStream;
	void operator()(const registeredFunction& fun) const { fun.execute(lexeme, theStream); }
	void operator()(const registeredFunctionStreamOnly& fun) const { fun.execute(theStream); }
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
	void registerMatcher(const matcherFunction& matcher, const parsingFunctionStreamOnly& function);
	void registerMatcher(const matcherFunction& matcher, const parsingFunction& function);
	void registerRegex(const std::string& regex, const parsingFunctionStreamOnly& function);
	void registerRegex(const std::string& regex, const parsingFunction& function);
	
	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(std::string_view filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

  protected:
	std::map<std::string, parsingFunctionStreamOnly> registeredKeywordStringsStreamOnly;
	std::map<std::string, parsingFunction> registeredKeywordStrings;

	std::vector<std::pair<matcherFunction, parsingFunctionVariant>> registeredRegexesAndMatchers;

  private:
	inline bool tryToMatchAgainstKeywords(const std::string& toReturn,
		 const std::string& strippedLexeme,
		 bool isLexemeQuoted,
		 std::istream& theStream);

};

} // namespace commonItems



#endif // PARSER_H