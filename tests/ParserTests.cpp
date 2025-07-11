#include "../CommonRegexes.h"
#include "../Parser.h"
#include "../ParserHelpers.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <sstream>



TEST(Parser_Tests, AbsorbBOMAbsorbsBOM)
{
	std::stringstream input{"\xEF\xBB\xBFMore text"};
	commonItems::absorbBOM(input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	EXPECT_EQ("More text", std::string{buffer});
}

TEST(Parser_Tests, AbsorbBOMDoesNotAbsorbNonBOM)
{
	std::stringstream input{"More text"};
	commonItems::absorbBOM(input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	EXPECT_EQ("More text", std::string{buffer});
}

TEST(Parser_Tests, KeywordsAreMatched)
{
	std::stringstream input{"key = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, KeywordsAreMatchedForExistsEquals)
{
	std::stringstream input{"key ?= value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, KeywordsAreMatchedForExistsEqualsWithoutPrecedingWhitespace)
{
	std::stringstream input{"key?= value"}; // no whitespace between key and ?=
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, FunctionObjectEquivalentToParse)
{
	std::stringstream input{"key = value"};
	std::string value;
	commonItems::parser parser{};
	parser.registerKeyword("key", [&value](std::istream& theStream) {
		value = commonItems::getString(theStream);
	});
	parser(input);
	EXPECT_EQ("value", value);
}

TEST(Parser_Tests, QuotedKeywordsAreMatched)
{
	std::stringstream input{"\"key\" = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"key\"", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, QuotedKeywordsAreQuotedlyMatched)
{
	std::stringstream input{"\"key\" = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("\"key\"", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"key\"", test.key);
	EXPECT_EQ("value", test.value);
}


TEST(Parser_Tests, QuotedValuesAreParsed)
{
	std::stringstream input{R"(key = "value quote")"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value quote", test.value);
}


TEST(Parser_Tests, QuotedValuesWithEscapedQuotesAreParsed)
{
	std::stringstream input{R"(key = "value \"quote\" string")"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ(R"(value \"quote\" string)", test.value);
}


TEST(Parser_Tests, StringLiteralsAreParsed)
{
	std::stringstream input{"key = R\"(value \"quote\" string)\""};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ(R"(value "quote" string)", test.value);
}


TEST(Parser_Tests, WrongKeywordsAreIgnored)
{
	std::stringstream input{"wrongkey = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerKeyword("key", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_TRUE(test.key.empty());
	ASSERT_TRUE(test.value.empty());
}


TEST(Parser_Tests, RegexesAreMatched)
{
	std::stringstream input{"key = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex("[key]+", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, RegexesAreMatchedOnExistsEquals)
{
	std::stringstream input1{"key ?= value"};
	std::stringstream input2{"key?= value"}; // no whitespace between key and ?=
	std::stringstream input3{"key ?=value"}; // no whitespace between ?= and value
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex("[key]+", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};

	auto test = Test(input1);
	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);

   test = Test(input2);
	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);

	test = Test(input3);
	EXPECT_EQ("key", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, WrongRegexesAreIgnored)
{
	std::stringstream input{"nonsense = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex("[key]+", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_TRUE(test.key.empty());
	ASSERT_TRUE(test.value.empty());
}

TEST(Parser_Tests, QuotedRegexesAreMatched)
{
	std::stringstream input{"\"key\" = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex("[key]+", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"key\"", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, QuotedRegexesAreQuotedlyMatched)
{
	std::stringstream input{"\"key\" = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex("[k\"ey]+", [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"key\"", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, CatchAllCatchesQuotedKeys)
{
	std::stringstream input{"\"key\" = value"};
	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex(commonItems::catchallRegex, [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"key\"", test.key);
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, CatchAllCatchesQuotedKeysWithWhitespaceInside)
{
	std::stringstream input;
	input << std::noskipws;
	input << "\"this\tis a\nkey\n\" = value";

	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex(commonItems::catchallRegex, [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"this\tis a key \"", test.key); // newlines have been replaced with spaces. This is WAD.
	EXPECT_EQ("value", test.value);
}

TEST(Parser_Tests, CatchAllCatchesQuotedKeysWithFigurativeCrapInside)
{
	std::stringstream input;
	input << std::noskipws;
	input << "\"this = is a silly { key\t} \" = value";

	class Test: commonItems::parser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerRegex(commonItems::catchallRegex, [this](const std::string& keyword, std::istream& theStream) {
				key = keyword;
				value = commonItems::getString(theStream);
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	EXPECT_EQ("\"this = is a silly { key\t} \"", test.key);
	EXPECT_EQ("value", test.value);
}


TEST(Parser_Tests, IgnoreUnregisteredItemsIgnoresUnregisteredItems)
{
	std::stringstream input;
	input << std::noskipws;
	input << "key = value\n";
	input << "key_two = { nested_item}\n";

	commonItems::parser parser;
	parser.IgnoreUnregisteredItems();


	std::stringstream log;
	auto std_out_buf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	parser.parseStream(input);

	std::cout.rdbuf(std_out_buf);
	std::string actual_output = log.str();

	EXPECT_TRUE(actual_output.empty());
}


TEST(Parser_Tests, IgnoreAndLogUnregisteredItemsIgnoresAndLogsUnregisteredItems)
{
	std::stringstream input;
	input << std::noskipws;
	input << "key = value\n";
	input << "key_two = { nested_item}\n";

	commonItems::parser parser;
	parser.IgnoreAndLogUnregisteredItems();


	std::stringstream log;
	auto std_out_buf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	parser.parseStream(input);

	std::cout.rdbuf(std_out_buf);
	std::string actual_output = log.str();

	EXPECT_THAT(actual_output, testing::HasSubstr(R"([DEBUG]     Ignoring keyword: key)"));
	EXPECT_THAT(actual_output, testing::HasSubstr(R"([DEBUG]     Ignoring keyword: key_two)"));
}

TEST(Parser_Tests, IgnoreAndLogUnregisteredItemsIgnoresAndLogsUnregisteredItemsOnExistsEquals)
{
	std::stringstream input;
	input << std::noskipws;
	input << "key ?= value1\n";
	input << "key_two ?= { nested_item}\n";
	input << "key_three?= value3\n"; // no space before the ExistsEquals
	input << "key_four ?=value4\n";	// no space after the ExistsEquals

	commonItems::parser parser;
	parser.IgnoreAndLogUnregisteredItems();


	std::stringstream log;
	auto std_out_buf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	parser.parseStream(input);

	std::cout.rdbuf(std_out_buf);
	std::string actual_output = log.str();

	EXPECT_THAT(actual_output, testing::HasSubstr(R"([DEBUG]     Ignoring keyword: key)"));
	EXPECT_THAT(actual_output, testing::HasSubstr(R"([DEBUG]     Ignoring keyword: key_two)"));
	EXPECT_THAT(actual_output, testing::HasSubstr(R"([DEBUG]     Ignoring keyword: key_three)"));
	EXPECT_THAT(actual_output, testing::HasSubstr(R"([DEBUG]     Ignoring keyword: key_four)"));
}


TEST(Parser_Tests, IgnoreAndStoreUnregisteredItemsIgnoresUnregisteredItems)
{
	std::stringstream input;
	input << std::noskipws;
	input << "key = value\n";
	input << "key_two = { nested_item}\n";

	std::set<std::string> ignored_items;
	commonItems::parser parser;
	parser.IgnoreAndStoreUnregisteredItems(ignored_items);


	parser.parseStream(input);

	EXPECT_THAT(ignored_items, testing::UnorderedElementsAre("key", "key_two"));
}

TEST(Parser_Tests, IgnoreAndStoreUnregisteredItemsIgnoresUnregisteredItemsOnExistsEquals)
{
	std::stringstream input;
	input << std::noskipws;
	input << "key ?= value1\n";
	input << "key_two ?= { nested_item}\n";
	input << "key_three?= value3\n"; // no space before the ExistsEquals
	input << "key_four ?=value4\n";  // no space after the ExistsEquals

	std::set<std::string> ignored_items;
	commonItems::parser parser;
	parser.IgnoreAndStoreUnregisteredItems(ignored_items);


	parser.parseStream(input);

	EXPECT_THAT(ignored_items, testing::UnorderedElementsAre("key", "key_two", "key_three", "key_four"));
}