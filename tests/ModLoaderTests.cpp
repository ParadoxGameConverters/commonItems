#include "../ModLoader/ModLoader.h"
#include "../OSCompatibilityLayer.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>



using testing::UnorderedElementsAre;



TEST(ModLoaderTests, LoadModsLogsWhenNoMods)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	const Mods incomingMods;

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("./"), incomingMods);
	const auto& mods = modLoader.getMods();

	std::cout.rdbuf(stdOutBuf);
	std::string actualOutput = log.str();
	std::stringstream actualStream(actualOutput);

	EXPECT_TRUE(mods.empty());
	EXPECT_THAT(actualStream.str(), testing::HasSubstr(R"([INFO] No mods were detected in savegame. Skipping mod processing.)"));
#pragma warning(pop)
}



TEST(ModLoaderTests, ModsByPathCanBeLocatedUnpackedAndUpdated)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;														  // this is what comes from the save
	incomingMods.emplace_back(Mod("Some mod", "mod/themod.mod")); // mod's in fact named "The Mod" in the file.

	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path.string(), "GameDocumentsFolder/mod\\themodsfolder/");
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
#pragma warning(pop)
}


TEST(ModLoaderTests, ModsByNameCanBeLocatedUnpackedAndUpdated)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;									  // this is what comes from the save
	incomingMods.emplace_back(Mod("The Mod", "")); // No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path.string(), "GameDocumentsFolder/mod\\themodsfolder/");
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
#pragma warning(pop)
}


TEST(ModLoaderTests, ModsByNameCanBeLocatedByMetadataUnpackedAndUpdated)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;												  // this is what comes from the save
	incomingMods.emplace_back(Mod("The Metadata Mod", "")); // No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Metadata Mod");
	EXPECT_EQ(mods[0].path.string(), "GameDocumentsFolder/mod\\the_metadata_mod/");
	EXPECT_THAT(mods[0].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
#pragma warning(pop)
}


TEST(ModLoaderTests, BrokenMissingAndNonexistentModsAreDiscarded)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;
	incomingMods.emplace_back(Mod("", "mod/themod.mod"));								  // no name given but valid!
	incomingMods.emplace_back(Mod("Broken mod", "mod/brokenmod.mod"));			  // no path
	incomingMods.emplace_back(Mod("Missing mod", "mod/missingmod.mod"));			  // missing directory
	incomingMods.emplace_back(Mod("Nonexistent mod", "mod/nonexistentmod.mod")); // doesn't exist.

	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path.string(), "GameDocumentsFolder/mod\\themodsfolder/");
#pragma warning(pop)
}


TEST(ModLoaderTests, CompressedModsCanBeUnpacked)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;
	incomingMods.emplace_back(Mod("some packed mod", "mod/packedmod.mod"));

	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 1);
	EXPECT_EQ(mods[0].name, "Packed Mod");
	EXPECT_EQ(mods[0].path.string(), "mods\\packedmod/");
	EXPECT_TRUE(commonItems::DoesFolderExist(mods[0].path));
#pragma warning(pop)
}


TEST(ModLoaderTests, BrokenCompressedModsAreSkipped)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;
	incomingMods.emplace_back(Mod("broken packed mod", "mod/brokenpacked.mod"));

	commonItems::ModLoader modLoader;
	// calling std::string version because it calls the other
	modLoader.loadMods(std::string("GameDocumentsFolder"), incomingMods);
	const auto mods = modLoader.getMods();

	EXPECT_TRUE(mods.empty());
#pragma warning(pop)
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
	EXPECT_EQ(mods[0].path.string(), "GameDocumentsFolder\\mod\\themodsfolder");
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
	EXPECT_EQ(mods[1].name, "The Metadata Mod Two");
	EXPECT_EQ(mods[1].path.string(), "SteamModsFolder\\529340\\the_metadata_mod_two");
	EXPECT_THAT(mods[1].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
	EXPECT_EQ(mods[2].name, "The Metadata Mod");
	EXPECT_EQ(mods[2].path.string(), "GameDocumentsFolder\\mod\\the_metadata_mod");
	EXPECT_THAT(mods[2].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
}


TEST(ModLoaderTests, MultipleModDirectoriesCanBeLoadedStringVersion)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	Mods incomingMods;														// this is what comes from the save
	incomingMods.emplace_back(Mod("The Mod", ""));					// No path given, old-style mod inputs.
	incomingMods.emplace_back(Mod("The Metadata Mod Two", "")); // No path given, old-style mod inputs.
	incomingMods.emplace_back(Mod("The Metadata Mod", ""));		// No path given, old-style mod inputs.

	commonItems::ModLoader modLoader;
	// SteamModsFolder/529340 is Vic3's steam workshop mod folder
	modLoader.loadMods(std::vector<std::string>{"GameDocumentsFolder/mod", "SteamModsFolder/529340"}, incomingMods);
	const auto mods = modLoader.getMods();

	ASSERT_EQ(mods.size(), 3);
	EXPECT_EQ(mods[0].name, "The Mod");
	EXPECT_EQ(mods[0].path, "GameDocumentsFolder/mod/themodsfolder/");
	EXPECT_THAT(mods[0].dependencies, UnorderedElementsAre("Packed Mod", "Missing Mod"));
	EXPECT_EQ(mods[1].name, "The Metadata Mod Two");
	EXPECT_EQ(mods[1].path, "SteamModsFolder/529340/the_metadata_mod_two/");
	EXPECT_THAT(mods[1].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
	EXPECT_EQ(mods[2].name, "The Metadata Mod");
	EXPECT_EQ(mods[2].path, "GameDocumentsFolder/mod/the_metadata_mod/");
	EXPECT_THAT(mods[2].replacedFolders, testing::UnorderedElementsAre("replaced/path", "replaced/path/two"));
#pragma warning(pop)
}