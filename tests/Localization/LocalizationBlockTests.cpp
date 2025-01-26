#include "../../Localization/LocalizationBlock.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>



TEST(Localization_LocalizationBlock_Tests, LocalizationBlockCanBeCreated)
{
	const commonItems::LocalizationBlock block("key", "base language");

	EXPECT_EQ(block.GetKey(), "key");
	EXPECT_EQ(block.GetBaseLanguage(), "base language");
}


TEST(Localization_LocalizationBlock_Tests, LocalizationsCanBeAddedToBlock)
{
	commonItems::LocalizationBlock block("key", "base language");
	block.ModifyLocalization("base language", "base localization");
	block.ModifyLocalization("second language", "second localization");

	EXPECT_THAT(block.GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("base language", "base localization"), testing::Pair("second language", "second localization")));
}


TEST(Localization_LocalizationBlock_Tests, LocalizationsCanBeConstructedFromOtherBlock)
{
	commonItems::LocalizationBlock block("key", "base language");
	block.ModifyLocalization("base language", "base localization");
	block.ModifyLocalization("second language", "second localization");

	const commonItems::LocalizationBlock block_two("key_two", block);

	EXPECT_EQ(block_two.GetKey(), "key_two");
	EXPECT_EQ(block_two.GetBaseLanguage(), "base language");
	EXPECT_THAT(block_two.GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("base language", "base localization"), testing::Pair("second language", "second localization")));
}


TEST(Localization_LocalizationBlock_Tests, LocalizationsCanBeCopiedFromOtherBlock)
{
	commonItems::LocalizationBlock block("key", "base language");
	block.ModifyLocalization("base language", "base localization");
	block.ModifyLocalization("second language", "second localization");

	commonItems::LocalizationBlock block_two("key_two", "different base language");
	block_two.ModifyLocalization("different base language", "different base localization");
	block_two.ModifyLocalization("base language", "modified base localization");
	block_two.ModifyLocalization("third language", "third localization");

	block.CopyFrom(block_two);

	EXPECT_EQ(block.GetKey(), "key");
	EXPECT_EQ(block.GetBaseLanguage(), "base language");
	EXPECT_THAT(block.GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("base language", "modified base localization"),
			  testing::Pair("second language", "second localization"),
			  testing::Pair("different base language", "different base localization"),
			  testing::Pair("third language", "third localization")));
}


TEST(Localization_LocalizationBlock_Tests, LocalizationCanBeRetrieved)
{
	commonItems::LocalizationBlock block("key", "base language");
	block.ModifyLocalization("base language", "base localization");
	block.ModifyLocalization("second language", "second localization");

	EXPECT_EQ(block.GetLocalization("second language"), "second localization");
}


TEST(Localization_LocalizationBlock_Tests, BaseLanguageIsUsedForMissingLanguage)
{
	commonItems::LocalizationBlock block("key", "base language");
	block.ModifyLocalization("base language", "base localization");
	block.ModifyLocalization("second language", "second localization");

	EXPECT_EQ(block.GetLocalization("third language"), "base localization");
}


TEST(Localization_LocalizationBlock_Tests, KeyIsUsedForMissingBaseLanguage)
{
	commonItems::LocalizationBlock block("key", "base language");
	block.ModifyLocalization("second language", "second localization");

	EXPECT_EQ(block.GetLocalization("third language"), "key");
	EXPECT_EQ(block.GetLocalization("base language"), "key");
}


TEST(Localization_LocalizationBlock_Tests, LocalizationBlockCanBeModifiedForEveryLanguageWithoutOtherBlock)
{
	commonItems::LocalizationBlock block("key1", "english");
	block.ModifyLocalization("english", "$NUM$ Revolt");
	block.ModifyLocalization("french", "$NUM$ révolte");
	block.ModifyLocalization("german", "$NUM$ Revolte");
	block.ModifyLocalization("russian", "$NUM$ бунт");
	block.ModifyLocalization("simp_chinese", "$NUM$ 反叛");
	block.ModifyLocalization("spanish", "$NUM$ revuelta");

	const std::string number = "2";
	block.ModifyForEveryLanguage([number](const std::string& base_localization, [[maybe_unused]] const std::string& language) {
		const std::string placeholder = "$NUM$";
		std::string updated_localization = base_localization;
		return updated_localization.replace(updated_localization.find(placeholder), placeholder.size(), number);
	});

	EXPECT_THAT(block.GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("english", "2 Revolt"),
			  testing::Pair("french", "2 révolte"),
			  testing::Pair("german", "2 Revolte"),
			  testing::Pair("russian", "2 бунт"),
			  testing::Pair("simp_chinese", "2 反叛"),
			  testing::Pair("spanish", "2 revuelta")));
}


TEST(Localization_LocalizationBlock_Tests, LocalizationBlockCanBeModifiedForEveryLanguageWithOtherBlock)
{
	commonItems::LocalizationBlock block("key1", "english");
	block.ModifyLocalization("english", "$ADJ$ Revolt");
	block.ModifyLocalization("french", "$ADJ$ révolte");
	block.ModifyLocalization("german", "$ADJ$ Revolte");
	block.ModifyLocalization("russian", "$ADJ$ бунт");
	block.ModifyLocalization("simp_chinese", "$ADJ$ 反叛");
	block.ModifyLocalization("spanish", "$ADJ$ revuelta");

	commonItems::LocalizationBlock adjective_block("key2", "english");
	adjective_block.ModifyLocalization("english", "Roman");
	adjective_block.ModifyLocalization("french", "Romain");
	adjective_block.ModifyLocalization("german", "römisch");
	adjective_block.ModifyLocalization("russian", "Роман");
	adjective_block.ModifyLocalization("simp_chinese", "罗马");
	adjective_block.ModifyLocalization("spanish", "Romana");

	block.ModifyForEveryLanguage(adjective_block,
		 [](const std::string& base_localization, const std::string& modifying_localization, [[maybe_unused]] const std::string& language) {
			 const std::string placeholder = "$ADJ$";
			 std::string updated_localization = base_localization;
			 return updated_localization.replace(updated_localization.find(placeholder), placeholder.size(), modifying_localization);
		 });

	EXPECT_THAT(block.GetLocalizations(),
		 testing::UnorderedElementsAre(testing::Pair("english", "Roman Revolt"),
			  testing::Pair("french", "Romain révolte"),
			  testing::Pair("german", "römisch Revolte"),
			  testing::Pair("russian", "Роман бунт"),
			  testing::Pair("simp_chinese", "罗马 反叛"),
			  testing::Pair("spanish", "Romana revuelta")));
}