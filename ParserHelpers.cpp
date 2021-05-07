#include "ParserHelpers.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "StringUtils.h"
#include <charconv>
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

void ignoreAndLogItem(const std::string& keyword, std::istream& theStream)
{
	Log(LogLevel::Debug) << "Ignoring keyword: " << keyword;
	ignoreItem(keyword, theStream);
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



template <std::integral T>
T stringToInteger(const std::string& str, bool skipPartialMatchWarning) // for integral types only
{
	T theInteger = 0;
	const auto last = str.data() + str.size();
	const auto [ptr, ec] = std::from_chars(str.data(), last, theInteger);
	if (ec != std::errc() || (!skipPartialMatchWarning && ptr != last)) // conversion either failed or was successful but not all characters matched
	{
		Log(LogLevel::Warning) << "string to integer - invalid argument: " << str;
	}
	return theInteger;
}
template signed char stringToInteger<signed char>(const std::string& str, bool skipPartialMatchWarning);
template unsigned char stringToInteger<unsigned char>(const std::string& str, bool skipPartialMatchWarning);
template short stringToInteger<short>(const std::string& str, bool skipPartialMatchWarning);
template unsigned short stringToInteger<unsigned short>(const std::string& str, bool skipPartialMatchWarning);
template int stringToInteger<int>(const std::string& str, bool skipPartialMatchWarning);
template unsigned int stringToInteger<unsigned int>(const std::string& str, bool skipPartialMatchWarning);
template long stringToInteger<long>(const std::string& str, bool skipPartialMatchWarning);
template unsigned long stringToInteger<unsigned long>(const std::string& str, bool skipPartialMatchWarning);
template long long stringToInteger<long long>(const std::string& str, bool skipPartialMatchWarning);
template unsigned long long stringToInteger<unsigned long long>(const std::string& str, bool skipPartialMatchWarning);

double stringToDouble(const std::string& str)
{
	double theDouble = 0.0;
	const auto last = str.data() + str.size();
	const auto [ptr, ec] = std::from_chars(str.data(), last, theDouble);
	if (ec != std::errc() || ptr != last) // conversion either failed or was successful but not all characters matched
	{
		Log(LogLevel::Warning) << "string to double - invalid argument: " << str;
	}
	return theDouble;
}


[[nodiscard]] std::vector<int> getInts(std::istream& theStream)
{
	return intList{theStream}.getInts();
}


[[nodiscard]] std::vector<long long> getLlongs(std::istream& theStream)
{
	return llongList{theStream}.getLlongs();
}


[[nodiscard]] std::vector<unsigned long long> getULlongs(std::istream& theStream)
{
	return ullongList{theStream}.getULlongs();
}


[[nodiscard]] std::vector<double> getDoubles(std::istream& theStream)
{
	return doubleList{theStream}.getDoubles();
}


[[nodiscard]] std::vector<std::string> getStrings(std::istream& theStream)
{
	return stringList{theStream}.getStrings();
}


[[nodiscard]] int getInt(std::istream& theStream)
{
	return singleInt{theStream}.getInt();
}


[[nodiscard]] long long getLlong(std::istream& theStream)
{
	return singleLlong{theStream}.getLlong();
}


[[nodiscard]] unsigned long long getULlong(std::istream& theStream)
{
	return singleULlong{theStream}.getULlong();
}


[[nodiscard]] double getDouble(std::istream& theStream)
{
	return singleDouble{theStream}.getDouble();
}


[[nodiscard]] std::string getString(std::istream& theStream)
{
	return singleString{theStream}.getString();
}


intList::intList(std::istream& theStream)
{
	registerRegex(integerRegex, [this](const std::string& theInt, std::istream& unused) {
		integers.push_back(stringToInteger<int>(theInt));
	});
	registerRegex(quotedIntegerRegex, [this](const std::string& theInt, std::istream& unused) {
		const auto newInt = theInt.substr(1, theInt.size() - 2);
		integers.push_back(stringToInteger<int>(newInt));
	});

	parseStream(theStream);
}


llongList::llongList(std::istream& theStream)
{
	registerRegex(integerRegex, [this](const std::string& theLongLong, std::istream& unused) {
		llongs.push_back(stringToInteger<long long>(theLongLong));
	});
	registerRegex(quotedIntegerRegex, [this](const std::string& theLongLong, std::istream& unused) {
		const auto newLlong = theLongLong.substr(1, theLongLong.size() - 2);
		llongs.push_back(stringToInteger<long long>(newLlong));
	});

	parseStream(theStream);
}


ullongList::ullongList(std::istream& theStream)
{
	registerRegex(integerRegex, [this](const std::string& theUnsignedLongLong, std::istream& unused) {
		ullongs.push_back(stringToInteger<unsigned long long>(theUnsignedLongLong));
	});
	registerRegex(quotedIntegerRegex, [this](const std::string& theUnsignedLongLong, std::istream& unused) {
		const auto newULlong = theUnsignedLongLong.substr(1, theUnsignedLongLong.size() - 2);
		ullongs.push_back(stringToInteger<unsigned long long>(newULlong));
	});

	parseStream(theStream);
}


singleInt::singleInt(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	theInt = stringToInteger<int>(token);
}


singleLlong::singleLlong(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	theLongLong = stringToInteger<long long>(token);
}


singleULlong::singleULlong(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	theUnsignedLongLong = stringToInteger<unsigned long long>(token);
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
	return stringToInteger<int>(value);
}


doubleList::doubleList(std::istream& theStream)
{
	registerRegex(floatRegex, [this](const std::string& theDouble, std::istream& unused) {
		doubles.push_back(stringToDouble(theDouble));
	});
	registerRegex(quotedFloatRegex, [this](const std::string& theDouble, std::istream& unused) {
		const auto newDouble = remQuotes(theDouble);
		doubles.push_back(stringToDouble(newDouble));
	});

	parseStream(theStream);
}


singleDouble::singleDouble(std::istream& theStream)
{
	getNextTokenWithoutMatching(theStream); // remove equals
	const auto token = remQuotes(*getNextTokenWithoutMatching(theStream));

	theDouble = stringToDouble(token);
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
