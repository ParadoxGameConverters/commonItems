#include "../ModLoader/ModLoader.h"
#include "../OSCompatibilityLayer.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"



using testing::UnorderedElementsAre;



TEST(ModLoaderTests, LoadModsLogsWhenNoMods)
{
	const Mods incomingMods;

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto mods = modLoader.getMods();

	std::cout.rdbuf(stdOutBuf);
	std::string actualOutput = log.str();
	std::stringstream actualStream(actualOutput);

	EXPECT_TRUE(mods.empty());
	EXPECT_THAT(actualStream.str(), testing::HasSubstr(R"([INFO] No mods were detected in savegame. Skipping mod processing.)"));
}



TEST(ModLoaderTests, ModsByPathCanBeLocatedUnpackedAndUpdated)
{
	Mods incomingMods;														  // this is what comes from the save
	incomingMods.emplace_back(Mod("Some mod", "mod/themod.mod")); // mod's in fact named "The Mod" in the file.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, "mod/themod/");
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
}


TEST(ModLoaderTests, ModsByNameCanBeLocatedUnpackedAndUpdated)
{
	Mods incomingMods;									  // this is what comes from the save
	incomingMods.emplace_back(Mod("The Mod", "")); // No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, "mod/themod/");
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
}


TEST(ModLoaderTests, BrokenMissingAndNonexistentModsAreDiscarded)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("", "mod/themod.mod"));								  // no name given
	incomingMods.emplace_back(Mod("Broken mod", "mod/brokenmod.mod"));			  // no path
	incomingMods.emplace_back(Mod("Missing mod", "mod/missingmod.mod"));			  // missing directory
	incomingMods.emplace_back(Mod("Nonexistent mod", "mod/nonexistentmod.mod")); // doesn't exist.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, "mod/themod/");
}


TEST(ModLoaderTests, CompressedModsCanBeUnpacked)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("some packed mod", "mod/packedmod.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "Packed Mod");
	EXPECT_EQ(mods[0].path, "mods/packedmod/");
	EXPECT_TRUE(commonItems::DoesFolderExist("mods/packedmod/"));
}


TEST(ModLoaderTests, BrokenCompressedModsAreNotSkippedEvenThoughTheyShouldBe)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("broken packed mod", "mod/brokenpacked.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "Broken Packed Mod");
	EXPECT_EQ(mods[0].path, "mods/brokenpacked/");
	EXPECT_TRUE(commonItems::DoesFolderExist("mods/brokenpacked/"));
}
