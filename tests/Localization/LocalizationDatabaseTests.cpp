#include "../../Localization/LocalizationDatabase.h"
#include "../../ModLoader/Mod.h"
#include "../../ModLoader/ModLoader.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

TEST(Localization_LocalizationDatabase_Tests, NewLocalizationDatabaseIsEmpty)
{
	const commonItems::LocalizationDatabase database("base_language", {});

	EXPECT_EQ(database.size(), 0);
}


TEST(Localization_LocalizationDatabase_Tests, GettingMissingLocalizationBlockReturnsNullopt)
{
	const commonItems::LocalizationDatabase database("base_language", {});

	EXPECT_EQ(database.GetLocalizationBlock("missing"), std::nullopt);
}


TEST(Localization_LocalizationDatabase_Tests, LocalizationBlockCanBeAdded)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	commonItems::LocalizationBlock block("KEY", "base_language");
	block.ModifyLocalization("base_language", "base_value");
	block.ModifyLocalization("second_language", "second_value");
	database.AddOrModifyLocalizationBlock("KEY", block);

	EXPECT_EQ(database.size(), 1);
	EXPECT_TRUE(database.HasLocalization("KEY"));
	const auto test_block = database.GetLocalizationBlock("KEY");
	ASSERT_TRUE(test_block.has_value());
	EXPECT_EQ(test_block->GetBaseLanguage(), "base_language");
	EXPECT_EQ(test_block->GetKey(), "KEY");
	EXPECT_THAT(test_block->GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("base_language", "base_value"), testing::Pair("second_language", "second_value")));
}


TEST(Localization_LocalizationDatabase_Tests, LocalizationBlockCanBeUpdated)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	commonItems::LocalizationBlock block("KEY", "base_language");
	block.ModifyLocalization("base_language", "base_value");
	block.ModifyLocalization("second_language", "second_value");
	database.AddOrModifyLocalizationBlock("KEY", block);

	commonItems::LocalizationBlock second_block("KEY", "base_language");
	second_block.ModifyLocalization("base_language", "updated_base_value");
	second_block.ModifyLocalization("second_language", "updated_second_value");
	database.AddOrModifyLocalizationBlock("KEY", second_block);

	EXPECT_EQ(database.size(), 1);
	EXPECT_TRUE(database.HasLocalization("KEY"));
	const auto test_block = database.GetLocalizationBlock("KEY");
	ASSERT_TRUE(test_block.has_value());
	EXPECT_EQ(test_block->GetBaseLanguage(), "base_language");
	EXPECT_EQ(test_block->GetKey(), "KEY");
	EXPECT_THAT(test_block->GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("base_language", "updated_base_value"), testing::Pair("second_language", "updated_second_value")));
}


TEST(Localization_LocalizationDatabase_Tests, LocalizationsCanBeAddedFromStream)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	std::stringstream base_input;
	base_input << "l_base_language:\n";
	base_input << " KEY:0 \"value\"\n";
	base_input << " # comment - next two lines are blank and skipped\n";
	base_input << " \n";
	base_input << "    \n";
	base_input << " no_separator_skip_this_line\n";
	base_input << R"( KEY_TWO:0 "value_two")";
	base_input << R"( EMPTY_KEY:0)";
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


TEST(Localization_LocalizationDatabase_Tests, KeylessLocalizationsAreNotAdded)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	std::stringstream base_input;
	base_input << "l_base_language:\n";
	base_input << R"( :0 "empty value")";
	const auto base_language_lines = database.ScrapeStream(base_input);
	EXPECT_EQ(base_language_lines, 0);

	EXPECT_EQ(database.size(), 0);
}


TEST(Localization_LocalizationDatabase_Tests, ValuelessLocalizationsAreNotAdded)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	std::stringstream base_input;
	base_input << "l_base_language:\n";
	base_input << R"( EMPTY_KEY:0)";
	const auto base_language_lines = database.ScrapeStream(base_input);
	EXPECT_EQ(base_language_lines, 0);

	EXPECT_EQ(database.size(), 0);
	EXPECT_FALSE(database.HasLocalization("EMPTY_KEY"));
}


TEST(Localization_LocalizationDatabase_Tests, LocalizationsFromUnsupportedLanguagesWarnsAndAreNotAdded)
{
	commonItems::LocalizationDatabase database("base_language", {"second_language"});

	std::stringstream base_input;
	base_input << "l_third_language:\n";
	base_input << " KEY:0 \"value\"";

	std::stringstream log;
	auto stdout_buf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	const auto base_language_lines = database.ScrapeStream(base_input);
	std::cout.rdbuf(stdout_buf);
	std::string actual_output = log.str();

	EXPECT_EQ(base_language_lines, 0);
	EXPECT_THAT(actual_output, testing::HasSubstr(R"([WARNING] Scraping loc line [ KEY:0 "value"] without language specified!)"));

	EXPECT_EQ(database.size(), 0);
	EXPECT_FALSE(database.HasLocalization("KEY"));
}


TEST(Localization_LocalizationDatabase_Tests, LocalizationsCanBeReadFromFilesystem)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), {Mod{"The Mod", "mod/themod.mod"}});
	const auto mods = modLoader.getMods();

	// calling std::string_view version since that also calls the std::filesystem::path version
	commonItems::ModFilesystem filesystem(std::string_view("game"), mods);

	commonItems::LocalizationDatabase database("english", {"french", "spanish"});

	std::stringstream log;
	auto stdout_buf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	// calling std::string version since that also calls the std::filesystem::path version
	database.ScrapeLocalizations(filesystem, std::string("localization"));
	std::cout.rdbuf(stdout_buf);
	std::string actual_output = log.str();

	EXPECT_THAT(actual_output, testing::HasSubstr("[WARNING] Scraping loc line [ NO_LANGUAGE_KEY1:0 \"loc w/o language\"] without language specified!"));

	EXPECT_EQ(database.size(), 3);
	EXPECT_TRUE(database.HasLocalization("KEY1"));
	const auto test_block = database.GetLocalizationBlock("KEY1");
	ASSERT_TRUE(test_block.has_value());
	EXPECT_EQ(test_block->GetBaseLanguage(), "english");
	EXPECT_EQ(test_block->GetKey(), "KEY1");
	EXPECT_THAT(test_block->GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("english", "value1 modded"), testing::Pair("french", "valeur1"), testing::Pair("spanish", "valor")));

	const auto test_block_two = database.GetLocalizationBlock("KEY2");
	ASSERT_TRUE(test_block_two.has_value());
	EXPECT_EQ(test_block_two->GetBaseLanguage(), "english");
	EXPECT_EQ(test_block_two->GetKey(), "KEY2");
	EXPECT_THAT(test_block_two->GetLocalizations(), testing::UnorderedElementsAre(testing::Pair("english", "value2"), testing::Pair("french", "valeur2")));

	const auto test_block_three = database.GetLocalizationBlock("MOD_KEY1");
	ASSERT_TRUE(test_block_three.has_value());
	EXPECT_EQ(test_block_three->GetBaseLanguage(), "english");
	EXPECT_EQ(test_block_three->GetKey(), "MOD_KEY1");
	EXPECT_THAT(test_block_three->GetLocalizations(), testing::UnorderedElementsAre(testing::Pair("english", "mod loc")));

	const auto non_loc_block = database.GetLocalizationBlock("NON_LOC_KEY");
	EXPECT_FALSE(non_loc_block.has_value());
#pragma warning(pop)
#pragma GCC diagnostic pop
}

TEST(Localization_LocalizationDatabase_Tests, LocalizationsCanBeReadFromMetaModFilesystem)
{
#pragma warning(push)
#pragma GCC diagnostic push
#pragma warning(disable : 4996)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), {Mod{"The Metadata Mod", ""}});
	const auto mods = modLoader.getMods();

	// calling std::string_view version since that also calls the std::filesystem::path version
	commonItems::ModFilesystem filesystem(std::string_view("game"), mods);

	commonItems::LocalizationDatabase database("english", {"french", "spanish"});

	std::stringstream log;
	auto stdout_buf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	// calling std::string version since that also calls the std::filesystem::path version
	database.ScrapeLocalizations(filesystem, std::string("localization"));
	std::cout.rdbuf(stdout_buf);
	std::string actual_output = log.str();

	EXPECT_THAT(actual_output, testing::HasSubstr("[WARNING] Scraping loc line [ NO_LANGUAGE_KEY1:0 \"loc w/o language\"] without language specified!"));

	EXPECT_EQ(database.size(), 3);
	EXPECT_TRUE(database.HasLocalization("KEY1"));
	const auto test_block = database.GetLocalizationBlock("KEY1");
	ASSERT_TRUE(test_block.has_value());
	EXPECT_EQ(test_block->GetBaseLanguage(), "english");
	EXPECT_EQ(test_block->GetKey(), "KEY1");
	EXPECT_THAT(test_block->GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("english", "value1 modded"), testing::Pair("french", "valeur1"), testing::Pair("spanish", "valor")));

	const auto test_block_two = database.GetLocalizationBlock("KEY2");
	ASSERT_TRUE(test_block_two.has_value());
	EXPECT_EQ(test_block_two->GetBaseLanguage(), "english");
	EXPECT_EQ(test_block_two->GetKey(), "KEY2");
	EXPECT_THAT(test_block_two->GetLocalizations(), testing::UnorderedElementsAre(testing::Pair("english", "value2"), testing::Pair("french", "valeur2")));

	const auto test_block_three = database.GetLocalizationBlock("MOD_KEY1");
	ASSERT_TRUE(test_block_three.has_value());
	EXPECT_EQ(test_block_three->GetBaseLanguage(), "english");
	EXPECT_EQ(test_block_three->GetKey(), "MOD_KEY1");
	EXPECT_THAT(test_block_three->GetLocalizations(), testing::UnorderedElementsAre(testing::Pair("english", "mod loc")));

	const auto non_loc_block = database.GetLocalizationBlock("NON_LOC_KEY");
	EXPECT_FALSE(non_loc_block.has_value());
#pragma warning(pop)
#pragma GCC diagnostic pop
}