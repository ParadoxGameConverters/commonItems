#include "Parser.h"
#include "Log.h"
#include <filesystem>
#include <fstream>



namespace fs = std::filesystem;


namespace commonItems
{
std::string getNextLexeme(std::istream& theStream);
std::string getNextLexemeWithNewlines(std::istream& theStream);
} // namespace commonItems


void commonItems::absorbBOM(std::istream& theStream)
{
	const char firstChar = theStream.peek();
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


void commonItems::parser::registerRegex(const std::string& keyword, const parsingFunction& function)
{
	registeredKeywordRegexes.emplace_back(std::make_pair(keyword, function));
}


void commonItems::parser::registerKeyword(const std::regex& keyword, const parsingFunction& function)
{
	registeredRegexes.emplace_back(std::make_pair(keyword, function));
}


void commonItems::parser::parseStream(std::istream& theStream)
{
	auto braceDepth = 0;
	for (const auto& keywordItr: registeredKeywordRegexes)
	{
		generatedRegexes.emplace_back(std::make_pair(std::regex(keywordItr.first), keywordItr.second));
	}

	auto value = false; // tracker to indicate we we reached the value part of key=value pair
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
	std::vector<std::pair<std::regex, parsingFunction>>().swap(generatedRegexes);
}


void commonItems::parser::parseFile(const std::string& filename)
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
	std::vector<std::pair<std::string, parsingFunction>>().swap(registeredKeywordRegexes);
	std::vector<std::pair<std::regex, parsingFunction>>().swap(registeredRegexes);
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

		auto matched = false;
		if (const auto& match = registeredKeywordStrings.find(toReturn); match != registeredKeywordStrings.end())
		{
			match->second(toReturn, theStream);
			matched = true;
		}

		if (!matched)
		{
			for (const auto& registration: generatedRegexes)
			{
				std::smatch match;
				if (std::regex_match(toReturn, match, registration.first))
				{
					registration.second(toReturn, theStream);
					matched = true;
					break;
				}
			}
		}

		if (!matched)
		{
			for (const auto& registration: registeredRegexes)
			{
				std::smatch match;
				if (std::regex_match(toReturn, match, registration.first))
				{
					registration.second(toReturn, theStream);
					matched = true;
					break;
				}
			}
		}
		if (!matched)
			gotToken = true;
	}

	if (!toReturn.empty())
		return toReturn;
	return std::nullopt;
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
		return toReturn;
	return std::nullopt;
}


std::string commonItems::getNextLexeme(std::istream& theStream)
{
	std::string toReturn;

	auto inQuotes = false;
	while (true)
	{
		char inputChar;
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
		else if (inputChar == '\"' && inQuotes)
		{
			toReturn += inputChar;
			break;
		}
		else if (!inQuotes && std::isspace(inputChar))
		{
			if (!toReturn.empty())
				break;
		}
		else if (!inQuotes && inputChar == '{')
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
		else if (!inQuotes && inputChar == '}')
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
		else if (!inQuotes && inputChar == '=')
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
	}
	return toReturn;
}


std::string commonItems::getNextLexemeWithNewlines(std::istream& theStream)
{
	std::string toReturn;

	auto inQuotes = false;
	while (true)
	{
		char inputChar;
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
				{
					toReturn += inputChar;
					break;
				}
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
		else if (inputChar == '\"' && inQuotes)
		{
			toReturn += inputChar;
			toReturn += '\n';
			break;
		}
		else if (!inQuotes && inputChar == '\t')
		{
			if (!toReturn.empty())
			{
				toReturn += "\n";
				break;
			}
		}
		else if (!inQuotes && std::isspace(inputChar))
		{
			if (!toReturn.empty())
			{
				toReturn += inputChar;
				break;
			}
		}
		else if (!inQuotes && inputChar == '{')
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
		else if (!inQuotes && inputChar == '}')
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
		else if (!inQuotes && inputChar == '=')
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
	}
	return toReturn;
}