#include "../../Localization/LocalizationDatabase.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"



TEST(Localization_LocalizationDatabase_Tests, NewLocalizationDatabaseIsEmpty)
{
	const commonItems::LocalizationDatabase database("base language", {});

	EXPECT_EQ(database.size(), 0);
}


TEST(Localization_LocalizationDatabase_Tests, LocalizationsCanBeAddedFromStream)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	std::stringstream base_input;
	base_input << "l_base_language:\n";
	base_input << R"( KEY:0 "value")" << "\n";
	base_input << R"( KEY_TWO:0 "value_two")";
	const auto base_language_lines = database.ScrapeStream(base_input);
	EXPECT_EQ(base_language_lines, 2);

	std::stringstream second_input;
	second_input << "l_second_language:\n";
	second_input << R"( KEY:0 "second_value")";
	const auto second_language_lines = database.ScrapeStream(second_input);
	EXPECT_EQ(second_language_lines, 1);

	EXPECT_EQ(database.size(), 2);
	EXPECT_TRUE(database.HasLocalization("KEY"));
	const auto test_block = database.GetLocalizationBlock("KEY");
	ASSERT_TRUE(test_block.has_value());
	EXPECT_EQ(test_block->GetBaseLanguage(), "base_language");
	EXPECT_EQ(test_block->GetKey(), "KEY");
	EXPECT_THAT(test_block->GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("base_language", "value"), testing::Pair("second_language", "second_value")));
	const auto test_block_two = database.GetLocalizationBlock("KEY_TWO");
	ASSERT_TRUE(test_block_two.has_value());
	EXPECT_EQ(test_block_two->GetBaseLanguage(), "base_language");
	EXPECT_EQ(test_block_two->GetKey(), "KEY_TWO");
	EXPECT_THAT(test_block_two->GetLocalizations(), testing::UnorderedElementsAre(testing::Pair("base_language", "value_two")));
}