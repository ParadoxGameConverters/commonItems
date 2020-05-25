#include "../ParserHelpers.h"
#include "gtest/gtest.h"
#include <sstream>


TEST(ParserHelper_Tests, IgnoreItemIgnoresSimpleText)
{
	std::stringstream input{"ignore_me More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ("More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreItemIgnoresAssignedText)
{
	std::stringstream input{"= ignore_me More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ("More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreItemIgnoresBracedItem)
{
	std::stringstream input{"{ { ignore_me } } More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreItemIgnoresAssignedBracedItem)
{
	std::stringstream input{"= { { ignore_me } } More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreObjectIgnoresNextItem)
{
	std::stringstream input{"ignore_me More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ("More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreObjectIgnoresWholeBracedItem)
{
	std::stringstream input{"{ { ignore_me } } More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreStringIgnoresNextItem)
{
	std::stringstream input{"ignore_me More text"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ("More text", std::string{buffer});
}


TEST(ParserHelper_Tests, IgnoreStringIgnoresWholeQuoation)
{
	std::stringstream input{R"("ignore_me More" text)"};
	input >> std::noskipws;
	commonItems::ignoreItem("unused", input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" text", std::string{buffer});
}


TEST(ParserHelper_Tests, SingleItemReadsNextItem)
{
	std::stringstream input{"read_me More text"};
	input >> std::noskipws;

	const auto output = commonItems::singleItem("unused", input);

	ASSERT_EQ("read_me", output);
}


TEST(ParserHelper_Tests, SingleItemStripsQuotes)
{
	std::stringstream input{R"("read_me" More text)"};
	input >> std::noskipws;

	const auto output = commonItems::singleItem("unused", input);

	ASSERT_EQ("read_me", output);
}


TEST(ParserHelper_Tests, SingleItemSkipsEquals)
{
	std::stringstream input{"= read_me More text"};
	input >> std::noskipws;

	const auto output = commonItems::singleItem("unused", input);

	ASSERT_EQ("read_me", output);
}


TEST(ParserHelper_Tests, SingleItemReadsWholeBracedBlock)
{
	std::stringstream input{"{ { read_me } } More text"};
	input >> std::noskipws;

	const auto output = commonItems::singleItem("unused", input);

	ASSERT_EQ("{{read_me }}", output);
}


TEST(ParserHelper_Tests, IntListDefaultsToEmpty)
{
	std::stringstream input;

	const commonItems::intList theIntegers(input);

	ASSERT_EQ(std::vector<int>{}, theIntegers.getInts());
}


TEST(ParserHelper_Tests, IntListAddsInts)
{
	std::stringstream input{"1 2 3"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{1, 2, 3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}


TEST(ParserHelper_Tests, IntListAddsQuotedInts)
{
	std::stringstream input{R"("1" "2" "3")"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{1, 2, 3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}


TEST(ParserHelper_Tests, IntListAddsIntsFromBracedBlock)
{
	std::stringstream input{" = {1 2 3} 4"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{1, 2, 3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}


TEST(ParserHelper_Tests, SingleIntGetsIntAfterEquals)
{
	std::stringstream input{" = 1"};

	const commonItems::singleInt theInteger(input);

	ASSERT_EQ(1, theInteger.getInt());
}


TEST(ParserHelper_Tests, SingleIntGetsQuotedIntAfterEquals)
{
	std::stringstream input{R"(= "1")"};

	const commonItems::singleInt theInteger(input);

	ASSERT_EQ(1, theInteger.getInt());
}


TEST(ParserHelper_Tests, SingleIntLogsInvalidInput)
{
	std::stringstream input{"= foo"};

	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const commonItems::singleInt theInteger(input);

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ(" [WARNING] Expected an int, but instead got foo\n", log.str());
	ASSERT_EQ(0, theInteger.getInt());
}


TEST(ParserHelper_Tests, SimpleObjectGetsKeyValuePairs)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tid = 180\n";
	input << "\ttype = 46\n";
	input << "}\n";

	const commonItems::simpleObject theObject(input);

	ASSERT_EQ("180", theObject.getValue("id"));
	ASSERT_EQ("46", theObject.getValue("type"));
}


TEST(ParserHelper_Tests, SimpleObjectGivesEmptyStringForMissingKey)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}\n";

	const commonItems::simpleObject theObject(input);

	ASSERT_EQ("", theObject.getValue("id"));
}


TEST(ParserHelper_Tests, SimpleObjectCanReturnInt)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\tid = 180\n";
	input << "\ttype = 46\n";
	input << "}\n";

	const commonItems::simpleObject theObject(input);

	ASSERT_EQ(180, theObject.getValueAsInt("id"));
	ASSERT_EQ(46, theObject.getValueAsInt("type"));
}


TEST(ParserHelper_Tests, SimpleObjectGivesZeroForMissingKey)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "}\n";

	const commonItems::simpleObject theObject(input);

	ASSERT_EQ(0, theObject.getValueAsInt("id"));
}


TEST(ParserHelper_Tests, SimpleObjectIgnoresInternalBraces)
{
	std::stringstream input;
	input << "=\n";
	input << "{\n";
	input << "\t{\n";
	input << "\t\tid = 180\n";
	input << "\t\ttype = 46\n";
	input << "\t}\n";
	input << "}\n";

	const commonItems::simpleObject theObject(input);

	ASSERT_EQ(0, theObject.getValueAsInt("id"));
	ASSERT_EQ(0, theObject.getValueAsInt("type"));
}


TEST(ParserHelper_Tests, DoubleListDefaultsToEmpty)
{
	std::stringstream input;

	const commonItems::doubleList theDoubles(input);

	ASSERT_EQ(std::vector<double>{}, theDoubles.getDoubles());
}


TEST(ParserHelper_Tests, DoubleListAddsDoubles)
{
	std::stringstream input{"1.25 2.5 3.75"};

	const commonItems::doubleList theDoubles(input);

	const auto expectedDoubles = std::vector<double>{1.25, 2.5, 3.75};
	ASSERT_EQ(expectedDoubles, theDoubles.getDoubles());
}


TEST(ParserHelper_Tests, DoubleListAddsQuotedDoubles)
{
	std::stringstream input{R"("1.25" "2.5" "3.75")"};

	const commonItems::doubleList theDoubles(input);

	const auto expectedDoubles = std::vector<double>{1.25, 2.5, 3.75};
	ASSERT_EQ(expectedDoubles, theDoubles.getDoubles());
}


TEST(ParserHelper_Tests, DoubleListAddsDoublesFromBracedBlock)
{
	std::stringstream input{" = {1.25 2.5 3.75} 4.5"};

	const commonItems::doubleList theDoubles(input);

	const auto expectedDoubles = std::vector<double>{1.25, 2.5, 3.75};
	ASSERT_EQ(expectedDoubles, theDoubles.getDoubles());
}


TEST(ParserHelper_Tests, SingleDoubleGetsDoubleAfterEquals)
{
	std::stringstream input{" = 1.25"};

	const commonItems::singleDouble theDouble(input);

	ASSERT_EQ(1.25, theDouble.getDouble());
}


TEST(ParserHelper_Tests, SingleDoubleGetsQuotedDoubleAfterEquals)
{
	std::stringstream input{R"(= "1.25")"};

	const commonItems::singleDouble theDouble(input);

	ASSERT_EQ(1.25, theDouble.getDouble());
}


TEST(ParserHelper_Tests, SingleDoubleLogsInvalidInput)
{
	std::stringstream input{"= foo"};

	const std::stringstream log;
	const auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const commonItems::singleDouble theDouble(input);

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ(" [WARNING] Expected a double, but instead got foo\n", log.str());
	ASSERT_EQ(0, theDouble.getDouble());
}


TEST(ParserHelper_Tests, StringListDefaultsToEmpty)
{
	std::stringstream input;

	const commonItems::stringList theStrings(input);

	ASSERT_EQ(std::vector<std::string>{}, theStrings.getStrings());
}


TEST(ParserHelper_Tests, StringListAddsStrings)
{
	std::stringstream input{"foo bar baz"};

	const commonItems::stringList theStrings(input);

	const auto expectedStrings = std::vector<std::string>{"foo", "bar", "baz"};
	ASSERT_EQ(expectedStrings, theStrings.getStrings());
}


TEST(ParserHelper_Tests, StringListAddsQuotedStrings)
{
	std::stringstream input{R"("foo" "bar" "baz")"};

	const commonItems::stringList theStrings(input);

	const auto expectedStrings = std::vector<std::string>{"foo", "bar", "baz"};
	ASSERT_EQ(expectedStrings, theStrings.getStrings());
}


TEST(ParserHelper_Tests, StringListAddsStringsFromBracedBlock)
{
	std::stringstream input{" = { foo bar baz } qux"};

	const commonItems::stringList theStrings(input);

	const auto expectedStrings = std::vector<std::string>{"foo", "bar", "baz"};
	ASSERT_EQ(expectedStrings, theStrings.getStrings());
}


TEST(ParserHelper_Tests, SingleStringGetsStringAfterEquals)
{
	std::stringstream input{" = foo"};

	const commonItems::singleString theString(input);

	ASSERT_EQ("foo", theString.getString());
}


TEST(ParserHelper_Tests, SingleStringGetsQuotedStringAfterEquals)
{
	std::stringstream input{R"(= "foo")"};

	const commonItems::singleString theString(input);

	ASSERT_EQ("foo", theString.getString());
}


TEST(ParserHelper_Tests, StringOfObjectConvertsBracedObjectsToStrings)
{
	std::stringstream input;
	input >> std::noskipws;
	input << "=\n";
	input << "{\n";
	input << "\t{\n";
	input << "\t\tid = 180\n";
	input << "\t\ttype = 46\n";
	input << "\t}\n";
	input << "}";

	const commonItems::stringOfObject theObject(input);

	ASSERT_EQ(input.str(), theObject.getString());
}


TEST(ParserHelper_Tests, StringOfItemConvertsBracedObjectsToStrings)
{
	std::stringstream input;
	input >> std::noskipws;
	input << "= {\n";
	input << "\t{\n";
	input << "\t\tid = 180\n";
	input << "\t\ttype = 46\n";
	input << "\t}\n";
	input << "}";

	const commonItems::stringOfItem theItem(input);

	ASSERT_EQ(input.str(), theItem.getString());
}


TEST(ParserHelper_Tests, StringOfItemGetsStringAfterEquals)
{
	std::stringstream input{" = foo"};

	const commonItems::stringOfItem theItem(input);

	ASSERT_EQ("= foo", theItem.getString());
}


TEST(ParserHelper_Tests, StringOfItemsConvertsItemsWithinBracesToStrings)
{
	std::stringstream input;
	input >> std::noskipws;
	input << "= {\n";
	input << "\tfoo = {\n";
	input << "\t\tid = 180\n";
	input << "\t\ttype = 46\n";
	input << "\t}\n";
	input << "\tbar = baz\n";
	input << "}";

	const commonItems::stringsOfItems theItems(input);

	const auto expectedStrings = std::vector<std::string>{"foo = {\n\t\tid = 180\n\t\ttype = 46\n\t}\n", "bar = baz\n"};
	ASSERT_EQ(expectedStrings, theItems.getStrings());
}


TEST(ParserHelper_Tests, StringOfItemNamesConvertsItemNamesWithinBracesToStrings)
{
	std::stringstream input;
	input >> std::noskipws;
	input << "= {\n";
	input << "\tfoo = {\n";
	input << "\t\tid = 180\n";
	input << "\t\ttype = 46\n";
	input << "\t}\n";
	input << "\tbar = baz\n";
	input << "}";

	const commonItems::stringsOfItemNames theItemNames(input);

	const auto expectedStrings = std::vector<std::string>{"foo", "bar"};
	ASSERT_EQ(expectedStrings, theItemNames.getStrings());
}


TEST(ParserHelper_Tests, AssignmentItemsWithinBracesToKeyValuePairs)
{
	std::stringstream input;
	input >> std::noskipws;
	input << "= {\n";
	input << "\tid = 180\n";
	input << "\ttype = 46\n";
	input << "}";

	const commonItems::assignments theAssignments(input);

	const auto expectedAssignments = std::map<std::string, std::string>{{"id", "180"}, {"type", "46"}};
	ASSERT_EQ(expectedAssignments, theAssignments.getAssignments());
}

TEST(ParserHelper_Tests, ParseStreamSkipsMissingKeyInBraces)
{
	class TestClass: commonItems::parser
	{
	public:
		explicit TestClass(std::istream& theStream)
		{
			registerKeyword("test", [this](const std::string& unused, std::istream& theStream) {
				const commonItems::singleString testStr(theStream);
				test = testStr.getString() == "yes";
			});
			parseStream(theStream);
		}
		bool test = false;
	};
	
	class WrapperClass: commonItems::parser
	{
	  public:
		explicit WrapperClass(std::istream& theStream)
		{
			registerRegex("[a-z]", [this](const std::string& thekey, std::istream& theStream) {
				const TestClass newtest(theStream);
				themap[thekey] = newtest.test;
			});
			parseStream(theStream);
		}
		std::map<std::string, bool> themap;
	};

	std::stringstream input;
	input >> std::noskipws;
	input << "a = { test = yes }\n";
	input << "b = { = yes }\n";
	input << "c = { test = yes }";

	auto wrapper = WrapperClass(input);
	
	ASSERT_TRUE(wrapper.themap["a"]);
	ASSERT_FALSE(wrapper.themap["b"]);
	ASSERT_TRUE(wrapper.themap["c"]);
}

TEST(ParserHelper_Tests, ParseStreamSkipsMissingKeyOutsideBraces)
{
	class WrapperClass: commonItems::parser
	{
	  public:
		explicit WrapperClass(std::istream& theStream)
		{
			registerRegex("[a-z]", [this](const std::string& thekey, std::istream& theStream) {
				const commonItems::singleString testStr(theStream);
				themap[thekey] = testStr.getString() == "yes";
			});
			parseStream(theStream);
		}
		std::map<std::string, bool> themap;
	};

	std::stringstream input;
	input >> std::noskipws;
	input << "a = yes\n";
	input << "= yes\n";
	input << "c = yes\n";
	input << "d = yes\n";

	auto wrapper = WrapperClass(input);

	ASSERT_TRUE(wrapper.themap["a"]);
	ASSERT_FALSE(wrapper.themap["b"]);
	ASSERT_TRUE(wrapper.themap["c"]);
	ASSERT_TRUE(wrapper.themap["d"]);
}
