#include "../ModLoader/ModLoader.h"
#include "../OSCompatibilityLayer.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>



using testing::UnorderedElementsAre;



TEST(ModLoaderTests, LoadModsLogsWhenNoMods)
{
	const Mods incomingMods;

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	commonItems::ModLoader modLoader;
	modLoader.loadMods("./", incomingMods);
	const auto& mods = modLoader.getMods();

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
	modLoader.loadMods("GameDocumentsFolder", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, Path("GameDocumentsFolder/mod/themodsfolder"));
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
}


TEST(ModLoaderTests, ModsByNameCanBeLocatedUnpackedAndUpdated)
{
	Mods incomingMods;									  // this is what comes from the save
	incomingMods.emplace_back(Mod("The Mod", "")); // No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("GameDocumentsFolder", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, Path("GameDocumentsFolder/mod/themodsfolder"));
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
}


TEST(ModLoaderTests, ModsByNameCanBeLocatedByMetadataUnpackedAndUpdated)
{
	Mods incomingMods;												  // this is what comes from the save
	incomingMods.emplace_back(Mod("The Metadata Mod", "")); // No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("GameDocumentsFolder", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Metadata Mod");
	EXPECT_EQ(mods[0].path, Path("GameDocumentsFolder/mod/the_metadata_mod"));
	EXPECT_THAT(mods[0].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
}


TEST(ModLoaderTests, BrokenMissingAndNonexistentModsAreDiscarded)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("", "mod/themod.mod"));								  // no name given but valid!
	incomingMods.emplace_back(Mod("Broken mod", "mod/brokenmod.mod"));			  // no path
	incomingMods.emplace_back(Mod("Missing mod", "mod/missingmod.mod"));			  // missing directory
	incomingMods.emplace_back(Mod("Nonexistent mod", "mod/nonexistentmod.mod")); // doesn't exist.

	commonItems::ModLoader modLoader;
	modLoader.loadMods("GameDocumentsFolder", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, Path("GameDocumentsFolder/mod/themodsfolder"));
}


TEST(ModLoaderTests, CompressedModsCanBeUnpacked)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("some packed mod", "mod/packedmod.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("GameDocumentsFolder", incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "Packed Mod");
	EXPECT_EQ(mods[0].path, Path("mods/packedmod"));
	EXPECT_TRUE(commonItems::DoesFolderExist(mods[0].path));
}


TEST(ModLoaderTests, BrokenCompressedModsAreSkipped)
{
	Mods incomingMods;
	incomingMods.emplace_back(Mod("broken packed mod", "mod/brokenpacked.mod"));

	commonItems::ModLoader modLoader;
	modLoader.loadMods("GameDocumentsFolder", incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_TRUE(mods.empty());
}


TEST(ModLoaderTests, MultipleModDirectoriesCanBeLoaded)
{
	Mods incomingMods;														// this is what comes from the save
	incomingMods.emplace_back(Mod("The Mod", ""));					// No path given, old-style mod inputs.
	incomingMods.emplace_back(Mod("The Metadata Mod Two", "")); // No path given, old-style mod inputs.
	incomingMods.emplace_back(Mod("The Metadata Mod", ""));		// No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	// SteamModsFolder/529340 is Vic3's steam workshop mod folder
	modLoader.loadMods(std::vector<std::filesystem::path>{"GameDocumentsFolder/mod", "SteamModsFolder/529340"}, incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 3);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, Path("GameDocumentsFolder/mod/themodsfolder"));
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
	EXPECT_EQ(mods[1].name, "The Metadata Mod Two");
	EXPECT_EQ(mods[1].path, Path("SteamModsFolder/529340/the_metadata_mod_two"));
	EXPECT_THAT(mods[1].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
	EXPECT_EQ(mods[2].name, "The Metadata Mod");
	EXPECT_EQ(mods[2].path, Path("GameDocumentsFolder/mod/the_metadata_mod"));
	EXPECT_THAT(mods[2].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
}