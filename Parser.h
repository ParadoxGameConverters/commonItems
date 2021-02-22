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
	void registerMatcher(const matcherFunction& matcher, const parsingFunction& function);
	void registerRegex(const std::string& regex, const parsingFunction& function);
	
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

	std::vector<std::pair<matcherFunction, parsingFunction>> registeredMatchers;
};

} // namespace commonItems



#endif // PARSER_H