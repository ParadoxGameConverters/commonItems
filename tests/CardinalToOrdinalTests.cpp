#include "../CardinalToOrdinal.h"
#include "gtest/gtest.h"



TEST(CardinalToOrdinal_Tests, LastDigitOneGivesSt)
{
	ASSERT_EQ("st", CardinalToOrdinal(1));
}