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

TEST(ParserHelper_Tests, IntListAddsNegativeInts)
{
	std::stringstream input{"-1 -2 -3"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{-1, -2, -3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}


TEST(ParserHelper_Tests, IntListAddsQuotedInts)
{
	std::stringstream input{R"("1" "2" "3")"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{1, 2, 3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}

TEST(ParserHelper_Tests, IntListAddsQuotedNegativeInts)
{
	std::stringstream input{R"("-1" "-2" "-3")"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{-1, -2, -3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}

TEST(ParserHelper_Tests, IntListAddsIntsFromBracedBlock)
{
	std::stringstream input{" = {1 2 3} 4"};

	const commonItems::intList theIntegers(input);

	const auto expectedIntegers = std::vector<int>{1, 2, 3};
	ASSERT_EQ(expectedIntegers, theIntegers.getInts());
}


TEST(ParserHelper_Tests, LlongListDefaultsToEmpty)
{
	std::stringstream input;

	const commonItems::llongList theLlongs(input);

	ASSERT_EQ(std::vector<long long>{}, theLlongs.getLlongs());
}

TEST(ParserHelper_Tests, ULlongListDefaultsToEmpty)
{
	std::stringstream input;

	const commonItems::ullongList theULlongs(input);

	ASSERT_EQ(std::vector<unsigned long long>{}, theULlongs.getULlongs());
}


TEST(ParserHelper_Tests, LlongListAddsLlongs)
{
	std::stringstream input{"123456789012345 234567890123456 345678901234567"};

	const commonItems::llongList theLlongs(input);

	const auto expectedLlongs = std::vector<long long>{123456789012345, 234567890123456, 345678901234567};
	ASSERT_EQ(expectedLlongs, theLlongs.getLlongs());
}

TEST(ParserHelper_Tests, LlongListAddsNegativeLlongs)
{
	std::stringstream input{"-123456789012345 -234567890123456 -345678901234567"};

	const commonItems::llongList theLlongs(input);

	const auto expectedLlongs = std::vector<long long>{-123456789012345, -234567890123456, -345678901234567};
	ASSERT_EQ(expectedLlongs, theLlongs.getLlongs());
}

TEST(ParserHelper_Tests, ULlongListAddsLlongs)
{
	std::stringstream input{"299792458000000000 299792458000000304 256792458000000304"};

	const commonItems::ullongList theULlongs(input);

	const auto expectedULlongs =
		 std::vector<unsigned long long>{299792458000000000, 299792458000000304, 256792458000000304};
	ASSERT_EQ(expectedULlongs, theULlongs.getULlongs());
}


TEST(ParserHelper_Tests, LlongListAddsQuotedLLongs)
{
	std::stringstream input{R"("123456789012345" "234567890123456" "345678901234567")"};

	const commonItems::llongList theLlongs(input);

	const auto expectedLlongs = std::vector<long long>{123456789012345, 234567890123456, 345678901234567};
	ASSERT_EQ(expectedLlongs, theLlongs.getLlongs());
}

TEST(ParserHelper_Tests, LlongListAddsQuotedNegativeLLongs)
{
	std::stringstream input{R"("-123456789012345" "-234567890123456" "-345678901234567")"};

	const commonItems::llongList theLlongs(input);

	const auto expectedLlongs = std::vector<long long>{-123456789012345, -234567890123456, -345678901234567};
	ASSERT_EQ(expectedLlongs, theLlongs.getLlongs());
}

TEST(ParserHelper_Tests, ULlongListAddsQuotedLLongs)
{
	std::stringstream input{R"("299792458000000000" "299792458000000304" "256792458000000304")"};

	const commonItems::ullongList theULlongs(input);

	const auto expectedULlongs =
		 std::vector<unsigned long long>{299792458000000000, 299792458000000304, 256792458000000304};
	ASSERT_EQ(expectedULlongs, theULlongs.getULlongs());
}


TEST(ParserHelper_Tests, LlongListAddsLlongsFromBracedBlock)
{
	std::stringstream input{" = {123456789012345 234567890123456 345678901234567} 456789012345678"};

	const commonItems::llongList theLlongs(input);

	const auto expectedLlongs = std::vector<long long>{123456789012345, 234567890123456, 345678901234567};
	ASSERT_EQ(expectedLlongs, theLlongs.getLlongs());
}

TEST(ParserHelper_Tests, ULlongListAddsULlongsFromBracedBlock)
{
	std::stringstream input{" = {299792458000000000 299792458000000304 256792458000000304} 256796558000000304"};

	const commonItems::ullongList theULlongs(input);

	const auto expectedULlongs =
		 std::vector<unsigned long long>{299792458000000000, 299792458000000304, 256792458000000304};
	ASSERT_EQ(expectedULlongs, theULlongs.getULlongs());
}

TEST(ParserHelper_Tests, SingleIntGetsIntAfterEquals)
{
	std::stringstream input{" = 1"};

	const commonItems::singleInt theInteger(input);

	ASSERT_EQ(1, theInteger.getInt());
}

TEST(ParserHelper_Tests, SingleIntGetsNegativeIntAfterEquals)
{
	std::stringstream input{" = -1"};

	const commonItems::singleInt theInteger(input);

	ASSERT_EQ(-1, theInteger.getInt());
}


TEST(ParserHelper_Tests, SingleIntGetsQuotedIntAfterEquals)
{
	std::stringstream input{R"(= "1")"};

	const commonItems::singleInt theInteger(input);

	ASSERT_EQ(1, theInteger.getInt());
}


TEST(ParserHelper_Tests, SingleIntGetsQuotedNegativeIntAfterEquals)
{
	std::stringstream input{R"(= "-1")"};

	const commonItems::singleInt theInteger(input);

	ASSERT_EQ(-1, theInteger.getInt());
}

TEST(ParserHelper_Tests, SingleIntLogsInvalidInput)
{
	std::stringstream input{"= foo"};

	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const commonItems::singleInt theInteger(input);

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ(" [WARNING] Expected an int, but instead got foo\n", log.str());
	ASSERT_EQ(0, theInteger.getInt());
}

TEST(ParserHelper_Tests, SingleLlongGetsLlongAfterEquals)
{
	std::stringstream input{" = 123456789012345"};

	const commonItems::singleLlong theLlong(input);

	ASSERT_EQ(123456789012345, theLlong.getLlong());
}

TEST(ParserHelper_Tests, SingleLlongGetsNegativeLlongAfterEquals)
{
	std::stringstream input{" = -123456789012345"};

	const commonItems::singleLlong theLlong(input);

	ASSERT_EQ(-123456789012345, theLlong.getLlong());
}


TEST(ParserHelper_Tests, SingleULlongGetsIntAfterEquals)
{
	std::stringstream input{" = 299792458000000000"};

	const commonItems::singleULlong theULlong(input);

	ASSERT_EQ(299792458000000000, theULlong.getULlong());
}


TEST(ParserHelper_Tests, SingleLlongGetsQuotedLlongAfterEquals)
{
	std::stringstream input{R"(= "123456789012345")"};

	const commonItems::singleLlong theLlong(input);

	ASSERT_EQ(123456789012345, theLlong.getLlong());
}

TEST(ParserHelper_Tests, SingleLlongGetsQuotedNegativeLlongAfterEquals)
{
	std::stringstream input{R"(= "-123456789012345")"};

	const commonItems::singleLlong theLlong(input);

	ASSERT_EQ(-123456789012345, theLlong.getLlong());
}

TEST(ParserHelper_Tests, SingleULlongGetsQuotedIntAfterEquals)
{
	std::stringstream input{R"(= "123456789012345")"};

	const commonItems::singleULlong theULlong(input);

	ASSERT_EQ(123456789012345, theULlong.getULlong());
}

TEST(ParserHelper_Tests, SingleLlongLogsInvalidInput)
{
	std::stringstream input{"= foo"};

	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const commonItems::singleLlong theLlong(input);

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ(" [WARNING] Expected a long long, but instead got foo\n", log.str());
	ASSERT_EQ(0, theLlong.getLlong());
}

TEST(ParserHelper_Tests, SingleULlongLogsInvalidInput)
{
	std::stringstream input{"= foo"};

	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const commonItems::singleULlong theULlong(input);

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ(" [WARNING] Expected an unsigned long long, but instead got foo\n", log.str());
	ASSERT_EQ(0, theULlong.getULlong());
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
	auto* const stdOutBuf = std::cout.rdbuf();
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

TEST(ParserHelper_Tests, IgnoreItemIgnoresSimpleColorWithColorSpace)
{
	std::stringstream input{"rgb {6 7 15} More text"};
	std::stringstream input2{"hsv {0.1 1.0 0.6} More text"};
	input >> std::noskipws;
	input2 >> std::noskipws;
	commonItems::ignoreItem("unused", input);
	commonItems::ignoreItem("unused", input2);

	char buffer[256];
	char buffer2[256];
	input.getline(buffer, sizeof buffer);
	input2.getline(buffer2, sizeof buffer2);
	ASSERT_EQ(" More text", std::string{buffer});
	ASSERT_EQ(" More text", std::string{buffer2});
}


TEST(ParserHelper_Tests, IgnoreItemIgnoresAssignedColorWithColorSpace)
{
	std::stringstream input{"= rgb {6 7 15} More text"};
	std::stringstream input2{"= hsv {0.1 1.0 0.6} More text"};
	input >> std::noskipws;
	input2 >> std::noskipws;
	commonItems::ignoreItem("unused", input);
	commonItems::ignoreItem("unused", input2);

	char buffer[256];
	char buffer2[256];
	input.getline(buffer, sizeof buffer);
	input2.getline(buffer2, sizeof buffer2);
	ASSERT_EQ(" More text", std::string{buffer});
	ASSERT_EQ(" More text", std::string{buffer2});
}


TEST(ParserHelper_Tests, IgnoreItemIgnoresRgbAndHsvStringsWithoutBreakingParsing)
{
	std::stringstream input{"= rgb next_parameter = 69 More text"};
	std::stringstream input2{"= hsv next_parameter = 420 More text"};
	input >> std::noskipws;
	input2 >> std::noskipws;
	commonItems::ignoreItem("unused", input);
	commonItems::ignoreItem("unused", input2);

	char buffer[256];
	char buffer2[256];
	input.getline(buffer, sizeof buffer);
	input2.getline(buffer2, sizeof buffer2);
	ASSERT_EQ("next_parameter = 69 More text", std::string{buffer});
	ASSERT_EQ("next_parameter = 420 More text", std::string{buffer2});
}


TEST(ParserHelper_Tests, IgnoreItemIgnoresQuotedRgbAndHsvStringsWithoutBreakingParsing)
{
	std::stringstream input{"= \"rgb\" next_parameter = 69 More text"};
	std::stringstream input2{"= \"hsv\" next_parameter = 420 More text"};
	input >> std::noskipws;
	input2 >> std::noskipws;
	commonItems::ignoreItem("unused", input);
	commonItems::ignoreItem("unused", input2);

	char buffer[256];
	char buffer2[256];
	input.getline(buffer, sizeof buffer);
	input2.getline(buffer2, sizeof buffer2);
	ASSERT_EQ(" next_parameter = 69 More text", std::string{buffer});
	ASSERT_EQ(" next_parameter = 420 More text", std::string{buffer2});
}

TEST(ParserHelper_Tests, BlobListDefaultsToEmpty)
{
	std::stringstream input;

	const commonItems::blobList theBlobs(input);

	ASSERT_TRUE(theBlobs.getBlobs().empty());
}

TEST(ParserHelper_Tests, BlobListAddsBlobs)
{
	std::stringstream input{"= { {foo} {bar} {baz} }"};

	const commonItems::blobList theBlobs(input);
	input >> std::noskipws;

	const auto expectedBlobs = std::vector<std::string>{"foo", "bar", "baz"};
	ASSERT_EQ(expectedBlobs, theBlobs.getBlobs());
}

TEST(ParserHelper_Tests, BlobListAddsComplicatedBlobs)
{
	std::stringstream input{"= { {foo=bar bar=baz} {bar=baz baz=foo} {baz=foo foo=bar} }"};
	input >> std::noskipws;

	const commonItems::blobList theBlobs(input);

	const auto expectedBlobs = std::vector<std::string>{"foo=bar bar=baz", "bar=baz baz=foo", "baz=foo foo=bar"};
	ASSERT_EQ(expectedBlobs, theBlobs.getBlobs());
}

TEST(ParserHelper_Tests, BlobListPreservesEverythingWithinBlobs)
{
	std::stringstream input{
		 "= { {foo\t=\nbar\n \n{bar\t=\tbaz\n\n}} {BROKEN\t\t\tbar\n=\nbaz\n \t\tbaz\t=\nfoo\t} {\t\nbaz\n\t=\t\n\tfoo\n "
		 "{} \n\tfoo\t=\tbar\t} }"};
	input >> std::noskipws;

	const commonItems::blobList theBlobs(input);

	const auto expectedBlobs = std::vector<std::string>{"foo\t=\nbar\n \n{bar\t=\tbaz\n\n}",
		 "BROKEN\t\t\tbar\n=\nbaz\n \t\tbaz\t=\nfoo\t",
		 "\t\nbaz\n\t=\t\n\tfoo\n {} \n\tfoo\t=\tbar\t"};
	ASSERT_EQ(expectedBlobs, theBlobs.getBlobs());
}

TEST(ParserHelper_Tests, BlobListIgnoresEverythingOutsideBlobs)
{
	std::stringstream input{"= {\n\n\t\t{foo}\nkey=value\n\t {bar}\t\nsome=value\t\n{baz}\t\n  randomLooseText   }"};
	input >> std::noskipws;

	const commonItems::blobList theBlobs(input);

	const auto expectedBlobs = std::vector<std::string>{"foo", "bar", "baz"};
	ASSERT_EQ(expectedBlobs, theBlobs.getBlobs());
}

TEST(ParserHelper_Tests, BlobListIsEmptyOnTrivialWrongUsage)
{
	std::stringstream input{"= value\n"};
	input >> std::noskipws;

	const commonItems::blobList theBlobs(input);

	ASSERT_TRUE(theBlobs.getBlobs().empty());
}

TEST(ParserHelper_Tests, BlobListIsEmptyOnSimpleWrongUsage)
{
	std::stringstream input{"= { key=value\n key2=value2 }"};
	input >> std::noskipws;

	const commonItems::blobList theBlobs(input);

	ASSERT_TRUE(theBlobs.getBlobs().empty());
}

TEST(ParserHelper_Tests, BlobListIsNotAtFaultYouAreOnComplexWrongUsage)
{
	std::stringstream input{"= { key=value\n key2={ key3 = value2 }}"};
	input >> std::noskipws;

	const commonItems::blobList theBlobs(input);

	const auto expectedBlobs = std::vector<std::string>{" key3 = value2 "};
	ASSERT_EQ(expectedBlobs, theBlobs.getBlobs());
}
