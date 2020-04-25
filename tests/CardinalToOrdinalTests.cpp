#include "../CardinalToOrdinal.h"
#include "gtest/gtest.h"



TEST(CardinalToOrdinal_Tests, LastDigitOneGivesSt)
{
	ASSERT_EQ("st", CardinalToOrdinal(1));
}


TEST(CardinalToOrdinal_Tests, LastDigitTwoGivesNd)
{
	ASSERT_EQ("nd", CardinalToOrdinal(2));
}


TEST(CardinalToOrdinal_Tests, LastDigitThreeGivesRd)
{
	ASSERT_EQ("rd", CardinalToOrdinal(3));
}


TEST(CardinalToOrdinal_Tests, RemainingDigitsGiveTh)
{
	ASSERT_EQ("th", CardinalToOrdinal(4));
	ASSERT_EQ("th", CardinalToOrdinal(5));
	ASSERT_EQ("th", CardinalToOrdinal(6));
	ASSERT_EQ("th", CardinalToOrdinal(7));
	ASSERT_EQ("th", CardinalToOrdinal(8));
	ASSERT_EQ("th", CardinalToOrdinal(9));
	ASSERT_EQ("th", CardinalToOrdinal(0));
}


TEST(CardinalToOrdinal_Tests, TeensGiveTh)
{
	ASSERT_EQ("th", CardinalToOrdinal(10));
	ASSERT_EQ("th", CardinalToOrdinal(11));
	ASSERT_EQ("th", CardinalToOrdinal(12));
	ASSERT_EQ("th", CardinalToOrdinal(13));
}