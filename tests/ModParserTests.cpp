#include "../ModLoader/ModParser.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>



using std::filesystem::path;
using testing::UnorderedElementsAre;



TEST(ModParserTests, modPrimitivesDefaultToBlank)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	std::stringstream input;
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getPath().empty());
	EXPECT_TRUE(theMod.getDependencies().empty());
	EXPECT_TRUE(theMod.getReplacedPaths().empty());

	commonItems::ModParser the_mod_file;
	// test using the string version of the function, since that ends up calling the path version
	the_mod_file.parseMod(std::string("some_nonexistent_path/nonsense")); // loading non-existent mod

	EXPECT_TRUE(the_mod_file.getName().empty());
	EXPECT_TRUE(the_mod_file.getPath().empty());
	EXPECT_TRUE(the_mod_file.getFilesystemPath().empty());
	EXPECT_TRUE(the_mod_file.getDependencies().empty());
	EXPECT_TRUE(the_mod_file.getReplacedPaths().empty());
	EXPECT_TRUE(the_mod_file.getFilesystemReplacedPaths().empty());
#pragma warning(pop)
}


TEST(ModParserTests, modPrimitivesCanBeSet)
{
#pragma warning(push)
#pragma warning(disable : 4996)
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
	EXPECT_EQ(path("modPath"), theMod.getFilesystemPath());
	EXPECT_THAT(theMod.getDependencies(), UnorderedElementsAre("dep1", "dep2"));
#ifdef WINDOWS
	EXPECT_THAT(theMod.getReplacedPaths(), UnorderedElementsAre("replaced\\path", "replaced\\path\\two"));
#else
	EXPECT_THAT(theMod.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));
#endif
	EXPECT_THAT(theMod.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));

	commonItems::ModParser the_mod_file;
	// test using the string version of the function, since that ends up calling the path version
	the_mod_file.parseMod(std::string("GameDocumentsFolder/mod/parseable_mod_file.mod"));

	EXPECT_EQ(the_mod_file.getName(), "modName");
	EXPECT_EQ(the_mod_file.getPath(), "modPath");
	EXPECT_EQ(the_mod_file.getFilesystemPath(), path("modPath"));
	EXPECT_THAT(the_mod_file.getDependencies(), UnorderedElementsAre("dep1", "dep2"));
#ifdef WINDOWS
	EXPECT_THAT(the_mod_file.getReplacedPaths(), UnorderedElementsAre("replaced\\path", "replaced\\path\\two"));
#else
	EXPECT_THAT(the_mod_file.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));
#endif
	EXPECT_THAT(the_mod_file.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));
#pragma warning(pop)
}


TEST(ModParserTests, metadataPrimitivesDefaultToBlank)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	std::stringstream input;
	input << "{}";
	commonItems::ModParser theMod;
	theMod.parseMetadata(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getPath().empty());
	EXPECT_TRUE(theMod.getFilesystemPath().empty());
	EXPECT_TRUE(theMod.getDependencies().empty());
	EXPECT_TRUE(theMod.getReplacedPaths().empty());
	EXPECT_TRUE(theMod.getFilesystemReplacedPaths().empty());

	commonItems::ModParser the_mod_file;
	// test using the string version of the function, since that ends up calling the path version
	the_mod_file.parseMetadata(std::string("some_nonexistent_path/empty_mod/metadata/.metadata.json")); // doesn't exist

	EXPECT_TRUE(the_mod_file.getName().empty());
#ifdef WINDOWS
	// path is derived from the path itself, so the path is filled out even with an empty/missing file
	EXPECT_EQ(the_mod_file.getPath(), "some_nonexistent_path\\empty_mod");
#else
	// path is derived from the path itself, so the path is filled out even with an empty/missing file
	EXPECT_EQ(the_mod_file.getPath(), "some_nonexistent_path/empty_mod");
#endif
	// path is derived from the path itself, so the path is filled out even with an empty/missing file
	EXPECT_EQ(the_mod_file.getFilesystemPath(), path("some_nonexistent_path/empty_mod"));
	EXPECT_TRUE(the_mod_file.getDependencies().empty());
	EXPECT_TRUE(the_mod_file.getReplacedPaths().empty());
	EXPECT_TRUE(the_mod_file.getFilesystemReplacedPaths().empty());
#pragma warning(pop)
}


TEST(ModParserTests, metadataPrimitivesCanBeSet)
{
#pragma warning(push)
#pragma warning(disable : 4996)
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
	EXPECT_TRUE(theMod.getPath().empty());				 // path is derived from the path itself, so a stream leaves no path
	EXPECT_TRUE(theMod.getFilesystemPath().empty()); // path is derived from the path itself, so a stream leaves no path
	EXPECT_TRUE(theMod.getDependencies().empty());	 // dependencies are unknown for now
#ifdef WINDOWS
	EXPECT_THAT(theMod.getReplacedPaths(), UnorderedElementsAre("replaced\\path", "replaced\\path\\two"));
#else
	EXPECT_THAT(theMod.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));
#endif
	EXPECT_THAT(theMod.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));

	commonItems::ModParser the_mod_file;
	// test using the string version of the function, since that ends up calling the path version
	the_mod_file.parseMetadata(std::string("GameDocumentsFolder/mod/parseable_metadata/.metadata/metadata.json"));

	EXPECT_EQ(the_mod_file.getName(), "modName");
#ifdef WINDOWS
	EXPECT_EQ(the_mod_file.getPath(), "mod\\parseable_metadata");
#else
	EXPECT_EQ(the_mod_file.getPath(), "mod/parseable_metadata");
#endif
	EXPECT_EQ(the_mod_file.getFilesystemPath(), path("mod/parseable_metadata"));
	EXPECT_TRUE(the_mod_file.getDependencies().empty()); // dependencies are unknown for now
#ifdef WINDOWS
	EXPECT_THAT(the_mod_file.getReplacedPaths(), UnorderedElementsAre("replaced\\path", "replaced\\path\\two"));
#else
	EXPECT_THAT(the_mod_file.getReplacedPaths(), UnorderedElementsAre("replaced/path", "replaced/path/two"));
#endif
	EXPECT_THAT(the_mod_file.getFilesystemReplacedPaths(), UnorderedElementsAre(path("replaced/path"), path("replaced/path/two")));
#pragma warning(pop)
}


TEST(ModParserTests, modPathCanBeSetFromArchive)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	std::stringstream input;
	input << "archive=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_EQ("modPath", theMod.getPath());
	EXPECT_EQ(path("modPath"), theMod.getFilesystemPath());
#pragma warning(pop)
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
#pragma warning(push)
#pragma warning(disable : 4996)
	std::stringstream input;
	input << "name=modName\n";
	input << "path=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);
	// use string version, since that calls the path version
	theMod.setPath(std::string("updated_path"));

	EXPECT_EQ(theMod.getPath(), "updated_path");
#pragma warning(pop)
}