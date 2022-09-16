#include "../Linguistics/StringExtensions.h"
#include "gtest/gtest.h"



TEST(StringExtensions_Tests, LinguisticRulesCanBeEmbeddedIntoFile)
{
	const auto adjective_rules = commonItems::GetAdjectiveRules();

	EXPECT_FALSE(adjective_rules.empty());
}