#include "newParser.h"
#include "Log.h"
#include <cctype>
#include <fstream>


namespace commonItems
{

std::string getNextLexeme(std::istream& theStream);

}


void commonItems::parser::registerKeyword(std::string keyword, parsingFunction function)
{
	registeredKeywordStrings.insert(std::make_pair(keyword, function));
}


void commonItems::parser::registerRegex(std::string keyword, parsingFunction function)
{
	registeredKeywordRegexes.insert(std::make_pair(keyword, function));
}


void commonItems::parser::registerKeyword(std::regex keyword, parsingFunction function)
{
	registeredRegexes.push_back(std::make_pair(keyword, function));
}


void commonItems::parser::parseStream(std::istream& theStream)
{
	int braceDepth = 0;

	while (true)
	{
		auto token = getNextToken(theStream);
		if (token)
		{
			if (*token == "=")
			{
				continue;
			}

			else if (*token == "{")
			{
				braceDepth++;
			}

			else if (*token == "}")
			{
				braceDepth--;
				if (braceDepth == 0)
				{
					break;
				}
			}

			else
			{
				LOG(LogLevel::Warning) << "Unknown token while parsing stream: " << *token;
			}
		}
		else
		{
			break;
		}
	}
}


void commonItems::parser::parseFile(const std::string& filename)
{
	std::ifstream theFile(filename);
	if (!theFile.is_open())
	{
		LOG(LogLevel::Error) << "Could not open " << filename << " for parsing.";
		return;
	}

	const char firstChar = theFile.peek();
	if (firstChar == '\xEF')
	{
		char bitBucket[3];
		theFile.read(bitBucket, sizeof(bitBucket));
	}

	parseStream(theFile);
	theFile.close();
}


void commonItems::parser::clearRegisteredKeywords() noexcept
{
	registeredKeywordStrings.clear();
	registeredKeywordRegexes.clear();
	registeredRegexes.clear();
}


std::optional<std::string> commonItems::parser::getNextToken(std::istream& theStream)
{
	theStream >> std::noskipws;

	std::string toReturn;

	bool gotToken = false;
	while (!gotToken)
	{
		if (theStream.eof())
		{
			return {};
		}

		toReturn = getNextLexeme(theStream);

		bool matched = false;
		if (const auto& match = registeredKeywordStrings.find(toReturn); match != registeredKeywordStrings.end())
		{
			match->second(toReturn, theStream);
			matched = true;
		}

		if (!matched)
		{
			for (auto registration : registeredKeywordRegexes)
			{
				std::smatch match;
				if (std::regex_match(toReturn, match, std::regex(registration.first)))
				{
					registration.second(toReturn, theStream);
					matched = true;
					break;
				}
			}
		}

		if (!matched)
		{
			for (auto registration: registeredRegexes)
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
			gotToken = true;
		}
	}

	if (toReturn.size() > 0)
	{
		return toReturn;
	}
	else
	{
		return {};
	}
}


std::optional<std::string> commonItems::parser::getNextTokenWithoutMatching(std::istream& theStream)
{
	theStream >> std::noskipws;

	std::string toReturn;

	bool gotToken = false;
	while (!gotToken)
	{
		if (theStream.eof())
		{
			return {};
		}

		toReturn = getNextLexeme(theStream);
		gotToken = true;
	}

	if (toReturn.size() > 0)
	{
		return toReturn;
	}
	else
	{
		return {};
	}
}


std::string commonItems::getNextLexeme(std::istream& theStream)
{
	std::string toReturn;

	bool inString = false;
	while (true)
	{
		char inputChar;
		theStream >> inputChar;
		if (theStream.eof())
		{
			break;
		}
		else if (!inString && (inputChar == '#'))
		{
			std::string bitbucket;
			std::getline(theStream, bitbucket);
			if (toReturn.size() > 0)
			{
				break;
			}
		}
		else if (!inString && inputChar == '\n')
		{
			if (toReturn.size() > 0)
			{
				break;
			}
		}
		else if (inString && inputChar == '\n')
		{
			// fix paradox' mistake and don't break proper names in half
			inputChar = (" ")[0];
		}
		else if ((inputChar == '\"') && !inString && (toReturn.size() == 0))
		{
			inString = true;
			toReturn += inputChar;
		}
		else if ((inputChar == '\"') && inString)
		{
			inString = false;
			toReturn += inputChar;
			break;
		}
		else if (!inString && std::isspace(inputChar))
		{
			if (toReturn.size() > 0)
			{
				break;
			}
		}
		else if (!inString && (inputChar == '{'))
		{
			if (toReturn.size() == 0)
			{
				toReturn += inputChar;
			}
			else
			{
				theStream.putback('{');
			}
			break;
		}
		else if (!inString && (inputChar == '}'))
		{
			if (toReturn.size() == 0)
			{
				toReturn += inputChar;
			}
			else
			{
				theStream.putback('}');
			}
			break;
		}
		else if (!inString && (inputChar == '='))
		{
			if (toReturn.size() == 0)
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