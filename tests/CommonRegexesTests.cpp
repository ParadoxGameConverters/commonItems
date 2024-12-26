#include <regex>

#include "../CommonRegexes.h"
#include <gtest/gtest.h>



TEST(CommonRegexes_Tests, CatchallRegexMatchesStrings)
{
	std::smatch match;
	const std::string test_string("1234-abcd");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexMatchesQuotedStrings)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexDoesntMatchCurlyBrackets)
{
	std::smatch match;
	const std::string test_string("1234-abcd{");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
	const std::string test_string_two("1234-abcd}");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexMatchesQuotedCurlyBrackets)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd{")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
	const std::string test_string_two(R"("1234-abcd}")");
	EXPECT_TRUE(std::regex_match(test_string_two, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexMatchesInternalQuotes)
{
	std::smatch match;
	const std::string test_string(R"(1234-"abcd)");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexMatchesQuotedInternalQuotes)
{
	std::smatch match;
	const std::string test_string(R"("1234-"abcd")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexDoesntMatchEquals)
{
	std::smatch match;
	const std::string test_string("1234-abcd=");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, CatchallRegexMatchesQuotedEquals)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd=")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::catchallRegex)));
}


TEST(CommonRegexes_Tests, IntegerRegexMatchesIntegers)
{
	std::smatch match;
	const std::string test_string("123456");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::integerRegex)));
}


TEST(CommonRegexes_Tests, IntegerRegexMatchesNegativeIntegers)
{
	std::smatch match;
	const std::string test_string("-123456");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::integerRegex)));
}


TEST(CommonRegexes_Tests, IntegerRegexDoesntMatchQuotedIntegers)
{
	std::smatch match;
	const std::string test_string(R"("123456")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::integerRegex)));
}


TEST(CommonRegexes_Tests, IntegerRegexDoesntMatchFloats)
{
	std::smatch match;
	const std::string test_string("123.456");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::integerRegex)));
}


TEST(CommonRegexes_Tests, IntegerRegexDoesntMatchStrings)
{
	std::smatch match;
	const std::string test_string("a123456");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::integerRegex)));
	const std::string test_string_two("123456a");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::integerRegex)));
}


TEST(CommonRegexes_Tests, QuotedIntegerRegexMatchesIntegers)
{
	std::smatch match;
	const std::string test_string(R"("123456")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::quotedIntegerRegex)));
}


TEST(CommonRegexes_Tests, QuotedIntegerRegexMatchesNegativeIntegers)
{
	std::smatch match;
	const std::string test_string(R"("-123456")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::quotedIntegerRegex)));
}


TEST(CommonRegexes_Tests, QuotedIntegerRegexDoesntMatchUnquotedIntegers)
{
	std::smatch match;
	const std::string test_string("123456");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::quotedIntegerRegex)));
}


TEST(CommonRegexes_Tests, QuotedIntegerRegexesDoesntMatchFloats)
{
	std::smatch match;
	const std::string test_string(R"("123.456")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::quotedIntegerRegex)));
}


TEST(CommonRegexes_Tests, QuotedIntegerRegexDoesntMatchStrings)
{
	std::smatch match;
	const std::string test_string(R"("a123456")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::quotedIntegerRegex)));
	const std::string test_string_two(R"("123456a")");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::quotedIntegerRegex)));
}


TEST(CommonRegexes_Tests, FloatRegexMatchesFloats)
{
	std::smatch match;
	const std::string test_string("123.456");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::floatRegex)));
}


TEST(CommonRegexes_Tests, FloatRegexMatchesNegativeFloats)
{
	std::smatch match;
	const std::string test_string("-123.456");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::floatRegex)));
}


TEST(CommonRegexes_Tests, FloatRegexMatchesIntegers)
{
	std::smatch match;
	const std::string test_string("123456");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::floatRegex)));
}


TEST(CommonRegexes_Tests, FloatRegexDoesntMatchQuotedFloats)
{
	std::smatch match;
	const std::string test_string(R"("123456")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::floatRegex)));
}


TEST(CommonRegexes_Tests, FloatRegexDoesntMatchStrings)
{
	std::smatch match;
	const std::string test_string("a123456");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::floatRegex)));
	const std::string test_string_two("123456a");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::floatRegex)));
}


TEST(CommonRegexes_Tests, StringRegexMatchesStrings)
{
	std::smatch match;
	const std::string test_string("1234-abcd");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::stringRegex)));
}


TEST(CommonRegexes_Tests, StringRegexDoesntMatchQuotedStrings)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::stringRegex)));
}


TEST(CommonRegexes_Tests, StringRegexDoesntMatchCurlyBrackets)
{
	std::smatch match;
	const std::string test_string("1234-abcd{");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::stringRegex)));
	const std::string test_string_two("1234-abcd}");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::stringRegex)));
}


TEST(CommonRegexes_Tests, StringRegexDoesntMatchBrackets)
{
	std::smatch match;
	const std::string test_string("1234-abcd[");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::stringRegex)));
	const std::string test_string_two("1234-abcd]");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::stringRegex)));
}


TEST(CommonRegexes_Tests, StringRegexDoesntMatchInternalQuotes)
{
	std::smatch match;
	const std::string test_string(R"(1234-abcd")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::stringRegex)));
}


TEST(CommonRegexes_Tests, StringRegexDoesntMatchEquals)
{
	std::smatch match;
	const std::string test_string("1234-abcd=");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::stringRegex)));
}


TEST(CommonRegexes_Tests, QuotedStringRegexMatchesQuotedStrings)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::quotedStringRegex)));
}


TEST(CommonRegexes_Tests, QuotedStringRegexMatchesQuotedStringContainingCurlyBrackets)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd{")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::quotedStringRegex)));
	const std::string test_string_two("1234-abcd}");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::quotedStringRegex)));
}


TEST(CommonRegexes_Tests, QuotedStringRegexMatchesQuotedStringContainingBrackets)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd[")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::quotedStringRegex)));
	const std::string test_string_two("1234-abcd]");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::quotedStringRegex)));
}


TEST(CommonRegexes_Tests, QuotedStringRegexDoesntMatchInternalQuotes)
{
	std::smatch match;
	const std::string test_string(R"(1234-abcd")");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::quotedStringRegex)));
}


TEST(CommonRegexes_Tests, QuotedStringRegexMatchesQuotedStringContainingEquals)
{
	std::smatch match;
	const std::string test_string(R"("1234-abcd=")");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::quotedStringRegex)));
}


TEST(CommonRegexes_Tests, DateRegexMatchesDates)
{
	std::smatch match;
	const std::string test_string("1918.11.11");
	EXPECT_TRUE(std::regex_match(test_string, match, std::regex(commonItems::dateRegex)));
}


TEST(CommonRegexes_Tests, DateRegexRequiresThreeSections)
{
	std::smatch match;
	const std::string test_string("1918.11");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::dateRegex)));
	const std::string test_string_two("1918");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::dateRegex)));
	const std::string test_string_three;
	EXPECT_FALSE(std::regex_match(test_string_three, match, std::regex(commonItems::dateRegex)));
}


TEST(CommonRegexes_Tests, DateRegexDoesntMatchDatesWithCharacters)
{
	std::smatch match;
	const std::string test_string("1918a.11.11");
	EXPECT_FALSE(std::regex_match(test_string, match, std::regex(commonItems::dateRegex)));
	const std::string test_string_two("1918.11a.11");
	EXPECT_FALSE(std::regex_match(test_string_two, match, std::regex(commonItems::dateRegex)));
	const std::string test_string_three("1918.11.11a");
	EXPECT_FALSE(std::regex_match(test_string_three, match, std::regex(commonItems::dateRegex)));
}