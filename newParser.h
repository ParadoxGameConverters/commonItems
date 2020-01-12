#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include <istream>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <utility>


namespace commonItems
{
	typedef std::function<void(const std::string&, std::istream&)> parsingFunction;

	class parser
	{
	public:
		parser() = default;
		~parser() = default;
		parser(const parser&) = default;
		parser(parser&&) = default;
		parser& operator=(const parser&) = default;
		parser& operator=(parser&&) = default;

		void registerKeyword(std::string keyword, parsingFunction);
		void registerRegex(std::string keyword, parsingFunction);
		void registerKeyword(std::regex keyword, parsingFunction);
		void parseStream(std::istream& theStream);
		void parseFile(const std::string& filename);

		void clearRegisteredKeywords() noexcept;

		std::optional<std::string> getNextToken(std::istream& theStream);
		std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

	private:
		std::map<std::string, parsingFunction> registeredKeywordStrings;
		std::map<std::string, parsingFunction> registeredKeywordRegexes;
		std::list<std::pair<std::regex, parsingFunction>> registeredRegexes;
		std::string nextToken;
		int braceDepth = 0;
	};
}

#endif // NEW_PARSER_H