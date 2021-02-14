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


class registeredAnything
{
  public:
	virtual ~registeredAnything() = default;
	virtual bool match(const std::string& lexeme, std::istream& theStream) = 0;
	virtual bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) = 0;
};
class registeredRegex: public registeredAnything
{
  private:
	std::regex regex;
	parsingFunction function;
  public:
	registeredRegex(const std::string& keyword, const parsingFunction& function):
		 regex(std::regex(keyword)), function{function}
	{
	}
	bool match(const std::string& lexeme, std::istream& theStream)
	{
		if (!std::regex_match(lexeme, regex))
			return false;
		else
		{
			function(lexeme, theStream);
			return true;
		}
	}
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) {
		if (!std::regex_match(strippedLexeme, regex))
			return false;
		else
		{
			function(lexeme, theStream);
			return true;
		}
	}
};
class registeredMatcher: public registeredAnything
{
  private:
	bool (*matcher)(std::string_view);
	parsingFunction function;
  public:
	registeredMatcher(bool (*matcher)(std::string_view), const parsingFunction& function):
		 matcher(matcher), function{function}
	{
	}
	bool match(const std::string& lexeme, std::istream& theStream)
	{
		if (!matcher(lexeme))
			return false;
		else
		{
			function(lexeme, theStream);
			return true;
		}
	}
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream)
	{
		if (!matcher(strippedLexeme))
			return false;
		else
		{
			function(lexeme, theStream);
			return true;
		}
	}
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
	void registerMatcher(bool (*matcher)(std::string_view), const parsingFunction& function);
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

	std::vector<std::unique_ptr<registeredAnything>> registeredThings;
};

} // namespace commonItems



#endif // PARSER_H