#include "../CommonFunctions.h"
#include <gtest/gtest.h>



TEST(CardinalToOrdinal_Tests, LastDigitOneGivesSt)
{
	ASSERT_EQ("st", cardinalToOrdinal(1));
}


TEST(CardinalToOrdinal_Tests, LastDigitTwoGivesNd)
{
	ASSERT_EQ("nd", cardinalToOrdinal(2));
}


TEST(CardinalToOrdinal_Tests, LastDigitThreeGivesRd)
{
	ASSERT_EQ("rd", cardinalToOrdinal(3));
}


TEST(CardinalToOrdinal_Tests, RemainingDigitsGiveTh)
{
	ASSERT_EQ("th", cardinalToOrdinal(4));
	ASSERT_EQ("th", cardinalToOrdinal(5));
	ASSERT_EQ("th", cardinalToOrdinal(6));
	ASSERT_EQ("th", cardinalToOrdinal(7));
	ASSERT_EQ("th", cardinalToOrdinal(8));
	ASSERT_EQ("th", cardinalToOrdinal(9));
	ASSERT_EQ("th", cardinalToOrdinal(0));
}


TEST(CardinalToOrdinal_Tests, TeensGiveTh)
{
	ASSERT_EQ("th", cardinalToOrdinal(10));
	ASSERT_EQ("th", cardinalToOrdinal(11));
	ASSERT_EQ("th", cardinalToOrdinal(12));
	ASSERT_EQ("th", cardinalToOrdinal(13));
}

TEST(ReplaceCharacter_Tests, ReplaceCharacterCanReplaceSpaces)
{
	const std::string input = R"(a file name.eu4)";

	ASSERT_EQ(replaceCharacter(input, ' '), "a_file_name.eu4");
}

TEST(ReplaceCharacter_Tests, ReplaceCharacterCanReplaceMinuses)
{
	const std::string input = R"(a file-with-name.eu4)";

	ASSERT_EQ(replaceCharacter(input, '-'), "a file_with_name.eu4");
}

TEST(CardinalToRoman_Tests, NumbersCanBeConverted)
{
	EXPECT_EQ("", cardinalToRoman(-15));
	EXPECT_EQ("", cardinalToRoman(0));
	EXPECT_EQ("I", cardinalToRoman(1));
	EXPECT_EQ("II", cardinalToRoman(2));
	EXPECT_EQ("VII", cardinalToRoman(7));
	EXPECT_EQ("IX", cardinalToRoman(9));
	EXPECT_EQ("XI", cardinalToRoman(11));
	EXPECT_EQ("XXXI", cardinalToRoman(31));
	EXPECT_EQ("LI", cardinalToRoman(51));
	EXPECT_EQ("CI", cardinalToRoman(101));
	EXPECT_EQ("DI", cardinalToRoman(501));
	EXPECT_EQ("MI", cardinalToRoman(1001));
}
