#include "Parser.h"
#include "Log.h"
#include "StringUtils.h"
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


namespace commonItems
{
std::string getNextLexeme(std::istream& theStream);



bool registeredRegex::match(const std::string& lexeme, std::istream& theStream) const {
	if (!std::regex_match(lexeme, regex))
		return false;
	
	function(lexeme, theStream);
	return true;
}
bool registeredRegex::matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const {
	if (!std::regex_match(strippedLexeme, regex))
		return false;

	function(lexeme, theStream);
	return true;
}


bool registeredRegexStreamOnly::match(const std::string& lexeme, std::istream& theStream) const {
	if (!std::regex_match(lexeme, regex))
		return false;
	
	function(theStream);
	return true;
}
bool registeredRegexStreamOnly::matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const {
	if (!std::regex_match(strippedLexeme, regex))
		return false;
	
	function(theStream);
	return true;
}


bool registeredMatcher::match(const std::string& lexeme, std::istream& theStream) const {
	if (!matcher(lexeme))
		return false;
	
	function(lexeme, theStream);
	return true;
}
bool registeredMatcher::matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const {
	if (!matcher(strippedLexeme))
		return false;
	
	function(lexeme, theStream);
	return true;
}


bool registeredMatcherStreamOnly::match(const std::string& lexeme, std::istream& theStream) const {
	if (!matcher(lexeme))
		return false;
	
	function(theStream);
	return true;
}
bool registeredMatcherStreamOnly::matchStripped(const std::string& lexeme, const std::string& strippedLexeme, std::istream& theStream) const {
	if (!matcher(strippedLexeme))
		return false;
	
	function(theStream);
	return true;
}
} // namespace commonItems


void commonItems::absorbBOM(std::istream& theStream)
{
	const auto firstChar = static_cast<char>(theStream.peek());
	if (firstChar == '\xEF')
	{
		char bitBucket[3];
		theStream.read(bitBucket, sizeof bitBucket);
	}
}


void commonItems::parser::registerKeyword(const std::string& keyword, const parsingFunction& function)
{
	registeredKeywordStrings.insert(std::make_pair(keyword, function));
}


void commonItems::parser::registerKeyword(const std::string& keyword, const parsingFunctionStreamOnly& function)
{
	registeredKeywordStringsStreamOnly.insert(std::make_pair(keyword, function));
}


void commonItems::parser::registerMatcher(bool (*matcher)(std::string_view), const parsingFunction& function)
{
	registeredRegexesAndMatchers.emplace_back(registeredMatcher{matcher, function});
}


void commonItems::parser::registerMatcher(bool (*matcher)(std::string_view), const parsingFunctionStreamOnly& function)
{
	registeredRegexesAndMatchers.emplace_back(registeredMatcherStreamOnly{matcher, function});
}


void commonItems::parser::registerRegex(const std::string& keyword, const parsingFunction& function)
{
	registeredRegexesAndMatchers.emplace_back(registeredRegex{std::regex(keyword), function});
}


void commonItems::parser::registerRegex(const std::string& keyword, const parsingFunctionStreamOnly& function)
{
	registeredRegexesAndMatchers.emplace_back(registeredRegexStreamOnly{std::regex(keyword), function});
}


void commonItems::parser::parseStream(std::istream& theStream)
{
	auto braceDepth = 0;
	auto value = false; // tracker to indicate whether we reached the value part of key=value pair
	std::string tokensSoFar;

	while (true)
	{
		auto token = getNextToken(theStream);
		if (token)
		{
			tokensSoFar += *token;
			if (*token == "=")
			{
				if (!value)
				{
					value = true; // swapping to value part.
					continue;
				}
				else // leaving else to be noticeable.
				{
					// value is positive, meaning we were at value, and now we're hitting an equal. This is bad. We need to
					// manually fast-forward to brace-lvl 0 and die.
					char inputChar;
					while (braceDepth)
					{
						theStream >> inputChar;
						if (inputChar == '{')
							braceDepth++;
						if (inputChar == '}')
							braceDepth--;
						if (!isspace(inputChar))
							tokensSoFar += inputChar;
					}
					Log(LogLevel::Warning) << "Broken token syntax at " << tokensSoFar;
					return;
				}
			}
			if (*token == "{")
				braceDepth++;
			else if (*token == "}")
			{
				braceDepth--;
				if (braceDepth == 0)
					break;
			}
			else
				Log(LogLevel::Warning) << "Unknown token while parsing stream: " << *token;
		}
		else
			break;
	}
}


void commonItems::parser::parseFile(std::string_view filename)
{
	std::ifstream theFile(fs::u8path(filename));
	if (!theFile.is_open())
	{
		Log(LogLevel::Error) << "Could not open " << filename << " for parsing.";
		return;
	}

	absorbBOM(theFile);
	parseStream(theFile);
	theFile.close();
}


void commonItems::parser::clearRegisteredKeywords() noexcept
{
	std::map<std::string, parsingFunction>().swap(registeredKeywordStrings);
	std::map<std::string, parsingFunctionStreamOnly>().swap(registeredKeywordStringsStreamOnly);
	std::vector<registeredVariant>().swap(registeredRegexesAndMatchers);
}


std::optional<std::string> commonItems::parser::getNextToken(std::istream& theStream)
{
	theStream >> std::noskipws;
	std::string toReturn;

	auto gotToken = false;
	while (!gotToken)
	{
		if (theStream.eof())
			return {};

		toReturn = getNextLexeme(theStream);

		const auto strippedLexeme = remQuotes(toReturn);
		const auto isLexemeQuoted = (strippedLexeme.size() < toReturn.size());

		auto matched = tryToMatchAgainstKeywords(toReturn, strippedLexeme, isLexemeQuoted, theStream);

		// regexes and matchers
		if (!matched) {
			auto matchCaller = [&toReturn, &theStream](auto& obj) {
				return obj.match(toReturn, theStream);
			};
			for (const auto& var: registeredRegexesAndMatchers) {
				if (std::visit(matchCaller, var)) {
					matched = true;
					break;
				}
			}
			if (!matched && isLexemeQuoted) {
				auto matchStrippedCaller = [&toReturn, &strippedLexeme, &theStream](auto& obj) {
					return obj.matchStripped(toReturn, strippedLexeme, theStream);
				};
				for (const auto& var: registeredRegexesAndMatchers) {
					if (std::visit(matchStrippedCaller, var)) {
						matched = true;
						break;
					}
				}
			}
		}

		if (!matched)
			gotToken = true;
	}

	if (!toReturn.empty())
		return std::move(toReturn);
	return std::nullopt;
}


inline bool commonItems::parser::tryToMatchAgainstKeywords(const std::string& toReturn,
	 const std::string& strippedLexeme,
	 bool isLexemeQuoted,
	 std::istream& theStream)
{
	if (const auto& match = registeredKeywordStringsStreamOnly.find(toReturn); match != registeredKeywordStringsStreamOnly.end())
	{
		match->second(theStream);
		return true;
	}
	else if (const auto& match = registeredKeywordStrings.find(toReturn); match != registeredKeywordStrings.end())
	{
		match->second(toReturn, theStream);
		return true;
	}
	else if (isLexemeQuoted)
	{
		if (const auto& strippedMatch = registeredKeywordStringsStreamOnly.find(strippedLexeme);
			 strippedMatch != registeredKeywordStringsStreamOnly.end())
		{
			strippedMatch->second(theStream);
			return true;
		}
		else if (const auto& strippedMatch = registeredKeywordStrings.find(strippedLexeme);
			 strippedMatch != registeredKeywordStrings.end())
		{
			strippedMatch->second(toReturn, theStream);
			return true;
		}
	}

	return false;
}


std::optional<std::string> commonItems::parser::getNextTokenWithoutMatching(std::istream& theStream)
{
	theStream >> std::noskipws;

	std::string toReturn;

	auto gotToken = false;
	while (!gotToken)
	{
		if (theStream.eof())
			return std::nullopt;

		toReturn = getNextLexeme(theStream);
		gotToken = true;
	}

	if (!toReturn.empty())
		return std::move(toReturn);
	return std::nullopt;
}


std::string commonItems::getNextLexeme(std::istream& theStream)
{
	std::string toReturn;

	auto inQuotes = false;
	auto inLiteralQuote = false;
	unsigned char previousCharacter = '\0';

	while (true)
	{
		unsigned char inputChar;
		theStream >> inputChar;
		if (theStream.eof())
			break;
		if (!inQuotes && inputChar == '#')
		{
			std::string bitBucket;
			std::getline(theStream, bitBucket);
			if (!toReturn.empty())
				break;
		}
		else if (inputChar == '\n')
		{
			if (!inQuotes)
			{
				if (!toReturn.empty())
					break;
			}
			else
			{
				// fix paradox' mistake and don't break proper names in half
				toReturn += " ";
			}
		}
		else if (inputChar == '\"' && !inQuotes && toReturn.empty())
		{
			inQuotes = true;
			toReturn += inputChar;
		}
		else if (inputChar == '\"' && !inQuotes && toReturn.size() == 1 && toReturn.back() == 'R')
		{
			inLiteralQuote = true;
			toReturn.pop_back();
			toReturn += inputChar;
		}
		else if (inputChar == '(' && inLiteralQuote && toReturn.size() == 1)
		{
			continue;
		}
		else if (inputChar == '\"' && inLiteralQuote && previousCharacter == ')')
		{
			toReturn.pop_back();
			toReturn += inputChar;
			break;
		}
		else if (inputChar == '\"' && inQuotes && previousCharacter != '\\')
		{
			toReturn += inputChar;
			break;
		}
		else if (!inQuotes && !inLiteralQuote && std::isspace(inputChar))
		{
			if (!toReturn.empty())
				break;
		}
		else if (!inQuotes && !inLiteralQuote && inputChar == '{')
		{
			if (toReturn.empty())
			{
				toReturn += inputChar;
			}
			else
			{
				theStream.putback('{');
			}
			break;
		}
		else if (!inQuotes && !inLiteralQuote && inputChar == '}')
		{
			if (toReturn.empty())
			{
				toReturn += inputChar;
			}
			else
			{
				theStream.putback('}');
			}
			break;
		}
		else if (!inQuotes && !inLiteralQuote && inputChar == '=')
		{
			if (toReturn.empty())
			{
				toReturn += inputChar;
			}
			else
			{
				theStream.putback('=');
			}
			break;
		}
		else
		{
			toReturn += inputChar;
		}

		previousCharacter = inputChar;
	}
	return toReturn;
}
