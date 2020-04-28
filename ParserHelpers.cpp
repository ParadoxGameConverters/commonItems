#include "ParserHelpers.h"
#include "Log.h"
#include "StringUtils.h"
#include <cctype>
#include <sstream>


namespace commonItems
{

std::string getNextLexeme(std::istream& theStream);
std::string getNextLexemeWithNewlines(std::istream& theStream);


void ignoreItem(const std::string& unused, std::istream& theStream)
{
	auto next = getNextLexeme(theStream);
	if (next == "=")
	{
		next = getNextLexeme(theStream);
	}
	if (next == "{")
	{
		auto braceDepth = 1;
		while (true)
		{
			if (theStream.eof())
			{
				return;
			}

			auto token = getNextLexeme(theStream);
			if (token == "{")
			{
				braceDepth++;
			}
			else if (token == "}")
			{
				braceDepth--;
				if (braceDepth == 0)
				{
					return;
				}
			}
		}
	}
}


std::string singleItem(const std::string& unused, std::istream& theStream)
{
	auto next = getNextLexeme(theStream);
	if (next == "=")
	{
		next = getNextLexeme(theStream);
	}
	auto toReturn = next;
	if (next == "{")
	{
		auto braceDepth = 1;
		while (true)
		{
			if (theStream.eof())
			{
				return toReturn;
			}

			auto token = getNextLexemeWithNewlines(theStream);
			toReturn += token;
			if (token == "{")
			{
				braceDepth++;
			}
			else if (token == "}")
			{
				braceDepth--;
				if (braceDepth == 0)
				{
					return toReturn;
				}
			}
		}
	}

	return stringutils::remQuotes(toReturn);
}


void ignoreObject(const std::string& unused, std::istream& theStream)
{
	auto braceDepth = 0;
	while (true)
	{
		if (theStream.eof())
		{
			return;
		}

		auto token = getNextLexeme(theStream);
		if (token == "{")
		{
			braceDepth++;
		}
		else if (token == "}")
		{
			braceDepth--;
			if (braceDepth == 0)
			{
				return;
			}
		}
	}
}


void ignoreString(const std::string& unused, std::istream& theStream)
{
	singleString ignore(theStream);
}


intList::intList(std::istream& theStream)
{
	registerRegex(R"(\d+)", [this](const std::string& theInt, std::istream& theStream) {
		integers.push_back(std::stoi(theInt));
	});
	registerRegex(R"(\"\d+\")", [this](const std::string& theInt, std::istream& theStream) {
		const auto newInt = theInt.substr(1, theInt.size() - 2);
		integers.push_back(std::stoi(newInt));
	});

	parseStream(theStream);
}


singleInt::singleInt(std::istream& theStream)
{
	auto equals = getNextTokenWithoutMatching(theStream);
	const auto token = stringutils::remQuotes(*getNextTokenWithoutMatching(theStream));

	try
	{
		theInt = stoi(token);
	}
	catch (std::exception&)
	{
		Log(LogLevel::Warning) << "Expected an int, but instead got " << token;
		theInt = 0;
	}
}


simpleObject::simpleObject(std::istream& theStream)
{
	auto equals = getNextTokenWithoutMatching(theStream);

	auto braceDepth = 0;
	std::string key;
	while (true)
	{
		if (theStream.eof())
		{
			return;
		}

		char inputChar;
		theStream >> inputChar;

		if (inputChar == '{')
		{
			braceDepth++;
		}
		else if (inputChar == '}')
		{
			braceDepth--;
			if (braceDepth == 0)
			{
				return;
			}
		}
		else if (braceDepth > 1)
		{
			// Internal object; ignore.
		}
		else if (inputChar == '=')
		{
			auto value = getNextTokenWithoutMatching(theStream);
			values[key] = *value;
			key.clear();
		}
		else if (!isspace(inputChar))
		{
			key += inputChar;
		}
	}
}


std::string simpleObject::getValue(const std::string& key) const
{
	if (values.find(key) == values.end())
	{
		return "";
	}
	return values.at(key);
}


int simpleObject::getValueAsInt(const std::string& key) const
{
	const auto value = getValue(key);
	if (value.empty())
	{
		return 0;
	}
	return std::stoi(value);
}


doubleList::doubleList(std::istream& theStream)
{
	registerRegex(R"(-?\d+(.\d+)?)", [this](const std::string& theDouble, std::istream& theStream) {
		doubles.push_back(std::stof(theDouble));
	});
	registerRegex(R"(\"-?\d+(.\d+)?\")", [this](const std::string& theDouble, std::istream& theStream) {
		const auto newDouble = stringutils::remQuotes(theDouble);
		doubles.push_back(std::stof(newDouble));
	});

	parseStream(theStream);
}


singleDouble::singleDouble(std::istream& theStream)
{
	auto equals = getNextTokenWithoutMatching(theStream);
	const auto token = stringutils::remQuotes(*getNextTokenWithoutMatching(theStream));

	try
	{
		theDouble = stof(token);
	}
	catch (std::exception&)
	{
		Log(LogLevel::Warning) << "Expected a double, but instead got " << token;
		theDouble = 0.0;
	}
}


stringList::stringList(std::istream& theStream)
{
	registerKeyword(R"("")", [](const std::string& unused, std::istream& theStream) {
	});
	registerRegex(R"([^[:s:]^=^\{^\}^\"]+)", [this](const std::string& theString, std::istream& theStream) {
		strings.push_back(theString);
	});
	registerRegex(R"(\"[^\n^=^\{^\}^\"]+\")", [this](const std::string& theString, std::istream& theStream) {
		strings.emplace_back(stringutils::remQuotes(theString));
	});

	parseStream(theStream);
}


singleString::singleString(std::istream& theStream)
{
	auto equals = getNextTokenWithoutMatching(theStream);
	theString = stringutils::remQuotes(*getNextTokenWithoutMatching(theStream));
}


stringOfObject::stringOfObject(std::istream& theStream)
{
	auto braceDepth = 0;
	while (true)
	{
		if (theStream.eof())
		{
			return;
		}

		char inputChar;
		theStream >> inputChar;

		theString += inputChar;

		if (inputChar == '{')
		{
			braceDepth++;
		}
		else if (inputChar == '}')
		{
			braceDepth--;
			if (braceDepth == 0)
			{
				return;
			}
		}
	}
}


stringOfItem::stringOfItem(std::istream& theStream)
{
	const auto equals = getNextLexeme(theStream);
	const auto next = getNextLexeme(theStream);
	theString = equals + " " + next;

	if (next == "{")
	{
		auto braceDepth = 1;
		while (true)
		{
			if (theStream.eof())
			{
				return;
			}

			char inputChar;
			theStream >> inputChar;

			theString += inputChar;

			if (inputChar == '{')
			{
				braceDepth++;
			}
			else if (inputChar == '}')
			{
				braceDepth--;
				if (braceDepth == 0)
				{
					return;
				}
			}
		}
	}
}


stringsOfItems::stringsOfItems(std::istream& theStream)
{
	registerRegex(catchallRegex, [this](const std::string& itemName, std::istream& theStream) {
		const stringOfItem theItem(theStream);
		theStrings.push_back(itemName + " " + theItem.getString() + "\n");
	});

	parseStream(theStream);
}


stringsOfItemNames::stringsOfItemNames(std::istream& theStream)
{
	registerRegex(catchallRegex, [this](const std::string& itemName, std::istream& theStream) {
		ignoreItem(itemName, theStream);
		theStrings.push_back(itemName);
	});

	parseStream(theStream);
}


assignments::assignments(std::istream& theStream)
{
	registerRegex(catchallRegex, [this](const std::string& assignmentName, std::istream& theStream) {
		auto equals = getNextTokenWithoutMatching(theStream);
		auto assignmentValue = getNextTokenWithoutMatching(theStream);
		theAssignments.emplace(std::make_pair(assignmentName, *assignmentValue));
	});

	parseStream(theStream);
}

} // namespace commonItems