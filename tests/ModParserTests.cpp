#include "../ModLoader/ModParser.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>



using std::filesystem::path;
using testing::UnorderedElementsAre;



TEST(ModParserTests, modPrimitivesDefaultToBlank)
{
	std::stringstream input;
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getFilesystemPath().empty());
	EXPECT_TRUE(theMod.getDependencies().empty());
	EXPECT_TRUE(theMod.getFilesystemReplacedPaths().empty());

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMod("some_nonexistent_path/nonsense"); // loading non-existent mod

	EXPECT_TRUE(the_mod_file.getName().empty());
	EXPECT_TRUE(the_mod_file.getFilesystemPath().empty());
	EXPECT_TRUE(the_mod_file.getDependencies().empty());
	EXPECT_TRUE(the_mod_file.getFilesystemReplacedPaths().empty());
}


TEST(ModParserTests, modPrimitivesCanBeSet)
{
	std::stringstream input;
	input << "name=modName\n";
	input << "path=modPath\n";
	input << "dependencies = { dep1 dep2 }\n";
	input << "replace_path=\"replaced/path\"\n";
	input << "replace_path=\"replaced/path/two\"\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_EQ("modName", theMod.getName());
	EXPECT_EQ(path("modPath"), theMod.getFilesystemPath());
	EXPECT_THAT(theMod.getDependencies(), UnorderedElementsAre("dep1", "dep2"));
	EXPECT_THAT(theMod.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMod("GameDocumentsFolder/mod/parseable_mod_file.mod");

	EXPECT_EQ(the_mod_file.getName(), "modName");
	EXPECT_EQ(the_mod_file.getFilesystemPath(), path("modPath"));
	EXPECT_THAT(the_mod_file.getDependencies(), UnorderedElementsAre("dep1", "dep2"));
	EXPECT_THAT(the_mod_file.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));
}


TEST(ModParserTests, metadataPrimitivesDefaultToBlank)
{
	std::stringstream input;
	input << "{}";
	commonItems::ModParser theMod;
	theMod.parseMetadata(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getFilesystemPath().empty());
	EXPECT_TRUE(theMod.getDependencies().empty());
	EXPECT_TRUE(theMod.getFilesystemReplacedPaths().empty());

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMetadata("some_nonexistent_path/empty_mod/metadata/.metadata.json"); // doesn't exist

	EXPECT_TRUE(the_mod_file.getName().empty());
	// path is derived from the path itself, so the path is filled out even with an empty/missing file
	EXPECT_EQ(the_mod_file.getFilesystemPath(), path("some_nonexistent_path/empty_mod"));
	EXPECT_TRUE(the_mod_file.getDependencies().empty());
	EXPECT_TRUE(the_mod_file.getFilesystemReplacedPaths().empty());
}


TEST(ModParserTests, metadataPrimitivesCanBeSet)
{
	std::stringstream input;
	input << "{\n";
	input << "  \"name\" : \"modName\",\n";
	input << "  \"game_custom_data\" : {\n";
	input << "    \"replace_paths\" : [\n";
	input << "\t\t\t\"replaced/path\",\n";
	input << "\t\t\t\"replaced/path/two\"\n";
	input << "\t\t]\n";
	input << " \t}\n";
	input << "}";
	commonItems::ModParser theMod;
	theMod.parseMetadata(input);

	EXPECT_EQ(theMod.getName(), "modName");
	EXPECT_TRUE(theMod.getFilesystemPath().empty()); // path is derived from the path itself, so a stream leaves no path
	EXPECT_TRUE(theMod.getDependencies().empty());	 // dependencies are unknown for now
	EXPECT_THAT(theMod.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMetadata("GameDocumentsFolder/mod/parseable_metadata/.metadata/metadata.json");

	EXPECT_EQ(the_mod_file.getName(), "modName");
	EXPECT_EQ(the_mod_file.getFilesystemPath(), path("mod/parseable_metadata"));
	EXPECT_TRUE(the_mod_file.getDependencies().empty()); // dependencies are unknown for now
	EXPECT_THAT(the_mod_file.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));
}


TEST(ModParserTests, modPathCanBeSetFromArchive)
{
	std::stringstream input;
	input << "archive=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_EQ(path("modPath"), theMod.getFilesystemPath());
}


TEST(ModParserTests, modIsInvalidIfPathOrNameUnSet)
{
	std::stringstream input;
	commonItems::ModParser theMod;
	theMod.parseMod(input);
	EXPECT_FALSE(theMod.isValid());

	std::stringstream input2;
	input2 << "name=modName\n";
	commonItems::ModParser theMod2;
	theMod2.parseMod(input2);
	EXPECT_FALSE(theMod2.isValid());

	std::stringstream input3;
	input3 << "path=modPath\n";
	commonItems::ModParser theMod3;
	theMod3.parseMod(input3);
	EXPECT_FALSE(theMod3.isValid());
}


TEST(ModParserTests, modIsValidIfNameAndPathSet)
{
	std::stringstream input;
	input << "name=modName\n";
	input << "path=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_TRUE(theMod.isValid());
}


TEST(ModParserTests, modIsCompressedForZipAndBinPaths)
{
	std::stringstream input;
	input << "path=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);
	EXPECT_FALSE(theMod.isCompressed());

	std::stringstream input2;
	input2 << "path=modPath.zip\n";
	commonItems::ModParser theMod2;
	theMod2.parseMod(input2);
	EXPECT_TRUE(theMod2.isCompressed());

	std::stringstream input3;
	input3 << "path=modPath.bin\n";
	commonItems::ModParser theMod3;
	theMod3.parseMod(input3);
	EXPECT_TRUE(theMod3.isCompressed());
}


TEST(ModParserTests, PathCanBeUpdated)
{
	std::stringstream input;
	input << "name=modName\n";
	input << "path=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);
	theMod.setPath("updated_path");

	EXPECT_EQ(theMod.getFilesystemPath(), path("updated_path"));
}