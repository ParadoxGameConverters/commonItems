#include "../Parser.h"
#include "gtest/gtest.h"
#include <sstream>
#include "../ParserHelpers.h"

TEST(Parser_Tests, AbsorbBOMAbsorbsBOM)
{
	std::stringstream input{"\xEF\xBB\xBFMore text"};
	commonItems::absorbBOM(input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ("More text", std::string{buffer});
}

TEST(Parser_Tests, AbsorbBOMDoesNotAbsorbNonBOM)
{
	std::stringstream input{"More text"};
	commonItems::absorbBOM(input);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ("More text", std::string{buffer});
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
				value = commonItems::singleString(theStream).getString();				
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);
	
	ASSERT_EQ("key", test.key);
	ASSERT_EQ("value", test.value);
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
				value = commonItems::singleString(theStream).getString();
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_EQ("\"key\"", test.key);
	ASSERT_EQ("value", test.value);
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
				value = commonItems::singleString(theStream).getString();
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_EQ("\"key\"", test.key);
	ASSERT_EQ("value", test.value);
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
				value = commonItems::singleString(theStream).getString();
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
				value = commonItems::singleString(theStream).getString();
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_EQ("key", test.key);
	ASSERT_EQ("value", test.value);
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
				value = commonItems::singleString(theStream).getString();
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
				value = commonItems::singleString(theStream).getString();
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_EQ("\"key\"", test.key);
	ASSERT_EQ("value", test.value);
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
				value = commonItems::singleString(theStream).getString();
			});
			parseStream(stream);
		}
		std::string key;
		std::string value;
	};
	const auto test = Test(input);

	ASSERT_EQ("\"key\"", test.key);
	ASSERT_EQ("value", test.value);
}
