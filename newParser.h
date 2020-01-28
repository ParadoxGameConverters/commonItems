#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include <functional>
#include <map>
#include <optional>
#include <regex>

namespace commonItems
{
	typedef std::function<void(const std::string&, std::istream&)> parsingFunction;

	class parser
	{
	public:
		parser() = default;
		~parser() = default;
		parser(const parser&) = default;
		parser(parser&&) noexcept = default;
		parser& operator=(const parser&) = default;
		parser& operator=(parser&&) = default;

		void registerKeyword(std::string keyword, parsingFunction);
		void registerRegex(std::string keyword, parsingFunction);
		void registerKeyword(std::regex keyword, parsingFunction);
		void parseStream(std::istream& theStream);
		void parseFile(const std::string& filename);
		void parseFile(const std::wstring& filename);

		void clearRegisteredKeywords() noexcept;

		std::optional<std::string> getNextToken(std::istream& theStream);
		std::optional<std::string> getNextTokenWithoutMatching(std::istream& theStream);

	private:
		std::map<std::string, parsingFunction> registeredKeywordStrings;
		std::vector<std::pair<std::string, parsingFunction>> registeredKeywordRegexes;
		std::vector<std::pair<std::regex, parsingFunction>> registeredRegexes;
		std::vector<std::pair<std::regex, parsingFunction>> generatedRegexes;
	};
}

#endif // NEW_PARSER_H