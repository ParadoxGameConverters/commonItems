#include "ParserHelpers.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "StringUtils.h"
#include <locale>
#include <sstream>



namespace commonItems
{

std::string getNextLexeme(std::istream& theStream);


void ignoreItem(const std::string& unused, std::istream& theStream)
{
	auto next = getNextLexeme(theStream);
	if (next == "=")
	{
		next = getNextLexeme(theStream);
	}
	if (next == "rgb" || next == "hsv") // Needed for ignoring color. Example: "color2 = rgb { 2 4 8 }"
	{
		if (theStream.peek() == '{')
		{
			next = getNextLexeme(theStream);
		}
		else // don't go further in cases like "type = rgb"
		{
			return;
		}
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
	registerRegex(integerRegex, [this](const std::string& theInt, std::istream& unused) {
		integers.push_back(std::stoi(theInt));
	});
	registerRegex(quotedIntegerRegex, [this](const std::string& theInt, std::istream& unused) {
		const auto newInt = theInt.substr(1, theInt.size() - 2);
		integers.push_back(std::stoi(newInt));
	});

	parseStream(theStream);
}


llongList::llongList(std::istream& theStream)
{
	registerRegex(integerRegex, [this](const std::string& theLongLong, std::istream& unused) {
		llongs.push_back(std::stoll(theLongLong));
	});
	registerRegex(quotedIntegerRegex, [this](const std::string& theLongLong, std::istream& unused) {
		const auto newLlong = theLongLong.substr(1, theLongLong.size() - 2);
		llongs.push_back(std::stoll(newLlong));
	});

	parseStream(theStream);
}


ullongList::ullongList(std::istream& theStream)
{
	registerRegex(integerRegex, [this](const std::string& theUnsignedLongLong, std::istream& unused) {
		ullongs.push_back(std::stoull(theUnsignedLongLong));
	});
	registerRegex(quotedIntegerRegex, [this](const std::string& theUnsignedLongLong, std::istream& unused) {
		const auto newULlong = theUnsignedLongLong.substr(1, theUnsignedLongLong.size() - 2);
		ullongs.push_back(std::stoull(newULlong));
	});

	parseStream(theStream);
}


singleInt::singleInt(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

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


singleLlong::singleLlong(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	try
	{
		theLongLong = std::stoll(token);
	}
	catch (std::exception&)
	{
		Log(LogLevel::Warning) << "Expected a long long, but instead got " << token;
		theLongLong = 0;
	}
}


singleULlong::singleULlong(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	try
	{
		theUnsignedLongLong = std::stoull(token);
	}
	catch (std::exception&)
	{
		Log(LogLevel::Warning) << "Expected an unsigned long long, but instead got " << token;
		theUnsignedLongLong = 0;
	}
}


simpleObject::simpleObject(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals

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
		else if (!std::isspace(inputChar, std::locale("en_US.UTF8")))
		{
			key += inputChar;
		}
	}
}


std::string simpleObject::getValue(const std::string& key) const
{
	if (const auto valueItr = values.find(key); valueItr != values.end())
	{
		return valueItr->second;
	}
	return std::string();
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
	registerRegex(floatRegex, [this](const std::string& theDouble, std::istream& unused) {
		doubles.push_back(std::stod(theDouble));
	});
	registerRegex(quotedFloatRegex, [this](const std::string& theDouble, std::istream& unused) {
		const auto newDouble = remQuotes(theDouble);
		doubles.push_back(std::stod(newDouble));
	});

	parseStream(theStream);
}


singleDouble::singleDouble(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	try
	{
		theDouble = stod(token);
	}
	catch (std::exception&)
	{
		Log(LogLevel::Warning) << "Expected a double, but instead got " << token;
		theDouble = 0.0;
	}
}


blobList::blobList(std::istream& theStream)
{
	auto next = getNextLexeme(theStream);
	if (next == "=")
	{
		next = getNextLexeme(theStream);
	}
	while (true)
	{
		if (next != "{")
		{
			break;
		}

		auto braceDepth = 0;
		std::string toReturn;
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
				if (braceDepth > 0)
				{
					toReturn += inputChar;
				}
				braceDepth++;
			}
			else if (inputChar == '}')
			{
				braceDepth--;
				if (braceDepth > 0)
				{
					toReturn += inputChar;
				}
				else if (braceDepth == 0)
				{
					blobs.emplace_back(toReturn);
					toReturn.clear();
				}
				else if (braceDepth == -1)
				{
					return;
				}
			}
			else if (braceDepth == 0)
			{
				// Ignore this character. Only look for blobs.
			}
			else
			{
				toReturn += inputChar;
			}
		}
	}
}


stringList::stringList(std::istream& theStream)
{
	registerKeyword(R"("")", [](std::istream& unused) {
	});
	registerRegex(stringRegex, [this](const std::string& theString, std::istream& unused) {
		strings.push_back(theString);
	});
	registerRegex(quotedStringRegex, [this](const std::string& theString, std::istream& unused) {
		strings.emplace_back(remQuotes(theString));
	});

	parseStream(theStream);
}


singleString::singleString(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // equals sign
	theString = remQuotes(*getNextTokenWithoutMatching(theStream));
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
	auto next = getNextLexeme(theStream);
	if (next == "=")
	{
		theString += next + " ";
		next = getNextLexeme(theStream);
	}
	theString += next;

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
		getNextTokenWithoutMatching(theStream); // remove equals
		auto assignmentValue = getNextTokenWithoutMatching(theStream);
		theAssignments.emplace(std::make_pair(assignmentName, *assignmentValue));
	});

	parseStream(theStream);
}

} // namespace commonItems