#include "../ModLoader/ModLoader.h"
#include "../OSCompatibilityLayer.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::Pair;
using testing::UnorderedElementsAre;

TEST(ModLoaderTests, ModsCanBeLocatedUnpackedAndUpdated)
{
	IncomingMods incomingMods;														  // this is what comes from the save
	incomingMods.emplace_back(std::pair("Some mod", "mod/themod.mod")); // mod's in fact named "The Mod" in the file.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_THAT(mods, UnorderedElementsAre(Pair("The Mod", "TestFiles/mod/themod/")));
}

TEST(ModLoaderTests, BrokenMissingAndNonexistentModsAreDiscarded)
{
	IncomingMods incomingMods;
	incomingMods.emplace_back(std::pair("", "mod/themod.mod"));								  // no name given
	incomingMods.emplace_back(std::pair("Broken mod", "mod/brokenmod.mod"));			  // no path
	incomingMods.emplace_back(std::pair("Missing mod", "mod/missingmod.mod"));			  // missing directory
	incomingMods.emplace_back(std::pair("Nonexistent mod", "mod/nonexistentmod.mod")); // doesn't exist.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_THAT(mods, UnorderedElementsAre(Pair("The Mod", "TestFiles/mod/themod/")));
}

TEST(ModLoaderTests, CompressedModsCanBeUnpacked)
{
	IncomingMods incomingMods;
	incomingMods.emplace_back(std::pair("some packed mod", "mod/packedmod.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("TestFiles", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_THAT(mods, UnorderedElementsAre(Pair("Packed Mod", "mods/packedmod/")));
	EXPECT_TRUE(commonItems::DoesFolderExist("mods/packedmod/"));
}
