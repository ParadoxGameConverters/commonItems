#include "ParserHelpers.h"
#include "Log.h"
#include <cctype>
#include <sstream>


namespace commonItems
{
std::string getNextLexeme(std::istream& theStream);
std::string getNextLexemeWithNewlines(std::istream& theStream);
}


void commonItems::absorbBOM(std::istream& theStream)
{
	const char firstChar = theStream.peek();
	if (firstChar == '\xEF')
	{
		char bitBucket[3];
		theStream.read(bitBucket, sizeof(bitBucket));
	}
}


void commonItems::ignoreItem(const std::string& unused, std::istream& theStream)
{
	std::string next = getNextLexeme(theStream); // equals
	if (next == "=")
	{
		next = getNextLexeme(theStream);
	}
	if (next == "{")
	{
		int braceDepth = 1;
		while (true)
		{
			if (theStream.eof())
			{
				return;
			}

			std::string token = getNextLexeme(theStream);
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

std::string commonItems::singleItem(const std::string& unused, std::istream& theStream)
{
	std::string next = getNextLexeme(theStream); // equals
	if (next == "=")
	{
		next = getNextLexeme(theStream);
	}
	std::string toReturn = next;	
	if (next == "{")
	{
		int braceDepth = 1;
		while (true)
		{
			if (theStream.eof())
			{
				return toReturn;
			}

			std::string token = getNextLexemeWithNewlines(theStream);
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
	if (toReturn.substr(0, 1) == "\"")
	{
		toReturn = toReturn.substr(1, toReturn.length() - 2);
	}
	return toReturn;
}

void commonItems::ignoreObject(const std::string& unused, std::istream& theStream)
{
	int braceDepth = 0;
	while (true)
	{
		if (theStream.eof())
		{
			return;
		}

		std::string token = getNextLexeme(theStream);
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


void commonItems::ignoreString(const std::string& unused, std::istream& theStream)
{
	commonItems::singleString ignore(theStream);
}


commonItems::intList::intList(std::istream& theStream)
{
	registerKeyword(std::regex("\\d+"), [this](const std::string& theInt, std::istream& theStream){
		ints.push_back(std::stoi(theInt));
	});
	registerKeyword(std::regex("\\\"\\d+\\\""), [this](const std::string& theInt, std::istream& theStream){
		auto newInt = theInt.substr(1, theInt.size() - 2);
		ints.push_back(std::stoi(newInt));
	});

	parseStream(theStream);
}


commonItems::singleInt::singleInt(std::istream& theStream):
	theInt()
{
	auto equals = getNextTokenWithoutMatching(theStream);
	auto token = *getNextTokenWithoutMatching(theStream);
	if (token.substr(0,1) == "\"")
	{
		token = token.substr(1, token.length() - 2);
	}
	try
	{
		theInt = stoi(token);
	}
	catch (std::exception&)
	{
		LOG(LogLevel::Warning) << "Expected an int, but instead got " << token;
		theInt = 0;
	}
}


commonItems::simpleObject::simpleObject(std::istream& theStream) : values()
{
        auto equals = getNextTokenWithoutMatching(theStream);
        int braceDepth = 0;
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
                        continue;
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

std::string commonItems::simpleObject::getValue(const std::string& key) const
{
        if (values.find(key) == values.end())
        {
                return "";
        }
        return values.at(key);
}

int commonItems::simpleObject::getValueAsInt(const std::string& key) const
{
        auto value = getValue(key);
        if (value.empty())
        {
                return 0;
        }
        return std::stoi(value);
}

commonItems::doubleList::doubleList(std::istream& theStream):
	doubles()
{
	registerKeyword(std::regex("-?\\d+(.\\d+)?"), [this](const std::string& theDouble, std::istream& theStream)
	{
		doubles.push_back(std::stof(theDouble));
	}
	);
	registerKeyword(std::regex("\\\"-?\\d+(.\\d+)?\\\""), [this](const std::string& theDouble, std::istream& theStream)
	{
		auto newDouble = theDouble.substr(1, theDouble.size() - 2);
		doubles.push_back(std::stof(newDouble));
	}
	);

	parseStream(theStream);
}


commonItems::singleDouble::singleDouble(std::istream& theStream):
	theDouble()
{
	auto equals = getNextTokenWithoutMatching(theStream);
	auto token = *getNextTokenWithoutMatching(theStream);
	if (token.substr(0,1) == "\"")
	{
		token = token.substr(1, token.length() - 2);
	}
	try
	{
		theDouble = stof(token);
	}
	catch (std::exception&)
	{
		LOG(LogLevel::Warning) << "Expected a double, but instead got " << token;
		theDouble = 0.0;
	}
}


commonItems::stringList::stringList(std::istream& theStream):
	strings()
{
	registerKeyword("\"\"", [](const std::string& unused, std::istream& theStream){});
	registerKeyword(std::regex("[^[:s:]^=^\\{^\\}^\\\"]+"), [this](const std::string& theString, std::istream& theStream)
	{
		strings.push_back(theString);
	}
	);
	registerKeyword(std::regex("\\\"[^\n^=^\\{^\\}^\\\"]+\\\""), [this](const std::string& theString, std::istream& theStream)
	{
		if (theString.substr(0,1) == "\"")
		{
			strings.push_back(theString.substr(1, theString.size() - 2));
		}
		else
		{
			strings.push_back(theString);
		}
	}
	);

	parseStream(theStream);
}


commonItems::singleString::singleString(std::istream& theStream):
	theString()
{
	auto equals = getNextTokenWithoutMatching(theStream);
	theString = *getNextTokenWithoutMatching(theStream);
	if (theString.substr(0,1) == "\"")
	{
		theString = theString.substr(1, theString.size() - 2);
	}
}


commonItems::stringOfObject::stringOfObject(std::istream& theStream)
{
	int braceDepth = 0;
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


commonItems::stringOfItem::stringOfItem(std::istream& theStream)
{
	auto equals = getNextLexeme(theStream);
	auto next = getNextLexeme(theStream);
	theString = equals + " " + next;

	if (next == "{")
	{
		int braceDepth = 1;
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


commonItems::stringsOfItems::stringsOfItems(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& itemName, std::istream& theStream){
		stringOfItem theItem(theStream);
		theStrings.push_back(itemName + theItem.getString() + "\n");
	});

	parseStream(theStream);
}


commonItems::stringsOfItemNames::stringsOfItemNames(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& itemName, std::istream& theStream){
		ignoreItem(itemName, theStream);
		theStrings.push_back(itemName);
	});

	parseStream(theStream);
}


commonItems::assignments::assignments(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& assignmentName, std::istream& theStream) {
		auto equals = getNextTokenWithoutMatching(theStream);
		auto assignmentValue = getNextTokenWithoutMatching(theStream);
		theAssignments.insert(std::make_pair(assignmentName, *assignmentValue));
	});

	parseStream(theStream);
}