#include "../StringUtils.h"
#include "gtest/gtest.h"



TEST(StringUtils_Tests, RemQuotesRemovesQuotes)
{
	const std::string quotedString{R"("Quoted string")"};
	const auto unquotedString = stringutils::remQuotes(quotedString);

	ASSERT_EQ("Quoted string", unquotedString);
}


TEST(StringUtils_Tests, RemQuotesRequiresStartingQuotes)
{
	const std::string quotedString{R"(Quoted string")"};
	const auto unquotedString = stringutils::remQuotes(quotedString);

	ASSERT_EQ(R"(Quoted string")", unquotedString);
}


TEST(StringUtils_Tests, RemQuotesRequiresEndingQuotes)
{
	const std::string quotedString{R"("Quoted string)"};
	const auto unquotedString = stringutils::remQuotes(quotedString);

	ASSERT_EQ(R"("Quoted string)", unquotedString);
}


TEST(StringUtils_Tests, RemQuotesLeavesSingleQuote)
{
	const std::string quotedString{R"(")"};
	const auto unquotedString = stringutils::remQuotes(quotedString);

	ASSERT_EQ(R"(")", unquotedString);
}


TEST(StringUtils_Tests, AddQuotesTurnsEmptyStringIntoQuoted)
{
	const std::string unquotedString;
	const auto quotedString = stringutils::addQuotes(unquotedString);

	ASSERT_EQ(R"("")", quotedString);
}


TEST(StringUtils_Tests, AddQuotesLeavesAlreadyQuotedStringAlone)
{
	const std::string alreadyQuotedString{R"("already quoted")"};
	const auto quotedString = stringutils::addQuotes(alreadyQuotedString);

	ASSERT_EQ(R"("already quoted")", quotedString);
}


TEST(StringUtils_Tests, AddQuotesAddsQuotesToUnquotedString)
{
	const std::string unqQuotedString{R"(not quoted)"};
	const auto quotedString = stringutils::addQuotes(unqQuotedString);

	ASSERT_EQ(R"("not quoted")", quotedString);
}