#ifndef PARSER_H
#define PARSER_H



#include <functional>
#include <map>
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include "re2/re2.h"



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
	void registerRegex(const std::string& regex, const parsingFunction& function, RE2::Options options = {});
	void clearRegisteredKeywords() noexcept;

	void parseStream(std::istream& theStream);
	void parseFile(const std::string& filename);

	std::optional<std::string> getNextToken(std::istream& theStream);
	static std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);


  private:
	std::map<std::string, parsingFunction> registeredKeywordStrings;
	std::vector<std::pair<std::shared_ptr<RE2>, parsingFunction>> generatedRegexes;
};

} // namespace commonItems



#endif // PARSER_H