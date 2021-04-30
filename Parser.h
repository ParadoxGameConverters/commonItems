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


class RegisteredRegexBase
{
  protected:
	std::regex regex;
  public:
	RegisteredRegexBase(const std::string& keyword);
	virtual ~RegisteredRegexBase() = default;
	virtual bool match(const std::string& lexeme, std::istream& theStream) = 0;
	virtual bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) = 0;
};


class RegisteredRegex: public RegisteredRegexBase
{
  private:
	parsingFunction function;
  public:
	RegisteredRegex(const std::string& keyword, const parsingFunction& function);
	bool match(const std::string& lexeme, std::istream& theStream);
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream);
};

class RegisteredRegexStreamOnly: public RegisteredRegexBase
{
  private:
	parsingFunctionStreamOnly function;
  public:
	RegisteredRegexStreamOnly(const std::string& keyword, const parsingFunctionStreamOnly& function);
	bool match(const std::string& lexeme, std::istream& theStream);
	bool matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream);
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
	void registerRegex(const std::string& keyword, const parsingFunctionStreamOnly& function);
	void registerRegex(const std::string& keyword, const parsingFunction& function);
	
	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(std::string_view filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

	
  private:
	std::vector<std::unique_ptr<RegisteredRegexBase>> registeredRegexes;
};

} // namespace commonItems



#endif // PARSER_H