#include "../ModLoader/ModLoader.h"
#include "../OSCompatibilityLayer.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

TEST(ModLoaderTests, ModsCanBeLocatedUnpackedAndUpdated)
{
	Mods incomingMods;														  // this is what comes from the save
	incomingMods.emplace_back(Mod("Some mod", "mod/themod.mod")); // mod's in fact named "The Mod" in the file.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_THAT(mods, UnorderedElementsAre(Mod("The Mod", "TestFiles/mod/themod/")));
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
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_THAT(mods, UnorderedElementsAre(Mod("The Mod", "TestFiles/mod/themod/")));
}

TEST(ModLoaderTests, CompressedModsCanBeUnpacked)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("some packed mod", "mod/packedmod.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_THAT(mods, UnorderedElementsAre(Mod("Packed Mod", "mods/packedmod/")));
	EXPECT_TRUE(commonItems::DoesFolderExist("mods/packedmod/"));
}

TEST(ModLoaderTests, BrokenCompressedModsAreNotSkippedEvenThoughTheyShouldBe)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("broken packed mod", "mod/brokenpacked.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_THAT(mods, UnorderedElementsAre(Mod("Broken Packed Mod", "mods/brokenpacked/")));
	EXPECT_TRUE(commonItems::DoesFolderExist("mods/brokenpacked/"));
}
