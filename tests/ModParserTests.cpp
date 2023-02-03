#include "../ModLoader/ModParser.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"



using testing::UnorderedElementsAre;



TEST(ModParserTests, modPrimitivesDefaultToBlank)
{
	std::stringstream input;
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getPath().empty());
	EXPECT_TRUE(theMod.getDependencies().empty());
	EXPECT_TRUE(theMod.getReplacedPaths().empty());

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMod("mod/empty_mod_file.mod");

	EXPECT_TRUE(the_mod_file.getName().empty());
	EXPECT_TRUE(the_mod_file.getPath().empty());
	EXPECT_TRUE(the_mod_file.getDependencies().empty());
	EXPECT_TRUE(the_mod_file.getReplacedPaths().empty());
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
	EXPECT_EQ("modPath", theMod.getPath());
	EXPECT_THAT(theMod.getDependencies(), UnorderedElementsAre("dep1", "dep2"));
	EXPECT_THAT(theMod.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMod("mod/parseable_mod_file.mod");

	EXPECT_EQ(the_mod_file.getName(), "modName");
	EXPECT_EQ(the_mod_file.getPath(), "modPath");
	EXPECT_THAT(the_mod_file.getDependencies(), UnorderedElementsAre("dep1", "dep2"));
	EXPECT_THAT(the_mod_file.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));
}


TEST(ModParserTests, metadataPrimitivesDefaultToBlank)
{
	std::stringstream input;
	input << "{}";
	commonItems::ModParser theMod;
	theMod.parseMetadata(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getPath().empty());
	EXPECT_TRUE(theMod.getDependencies().empty());
	EXPECT_TRUE(theMod.getReplacedPaths().empty());

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMetadata("mod/empty_mod/.metadata/metadata.json");

	EXPECT_TRUE(the_mod_file.getName().empty());
	EXPECT_EQ(the_mod_file.getPath(), "mod/empty_mod"); // path is derived from the path itself, so the path is filled out even with an empty file
	EXPECT_TRUE(the_mod_file.getDependencies().empty());
	EXPECT_TRUE(the_mod_file.getReplacedPaths().empty());
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
	EXPECT_TRUE(theMod.getPath().empty());			  // path is derived from the path itself, so a stream leaves no path
	EXPECT_TRUE(theMod.getDependencies().empty()); // dependencies are unknown for now
	EXPECT_THAT(theMod.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));

	commonItems::ModParser the_mod_file;
	the_mod_file.parseMetadata("mod/parseable_metadata/.metadata/metadata.json");

	EXPECT_EQ(the_mod_file.getName(), "modName");
	EXPECT_EQ(the_mod_file.getPath(), "mod/parseable_metadata");
	EXPECT_TRUE(the_mod_file.getDependencies().empty()); // dependencies are unknown for now
	EXPECT_THAT(the_mod_file.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));
}


TEST(ModParserTests, modPathCanBeSetFromArchive)
{
	std::stringstream input;
	input << "archive=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_EQ("modPath", theMod.getPath());
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

	EXPECT_EQ(theMod.getPath(), "updated_path");
}