#include "../ModLoader/ModParser.h"
#include "gtest/gtest.h"

TEST(ModParserTests, primitivesDefaultToBlank)
{
	std::stringstream input;
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_TRUE(theMod.getName().empty());
	EXPECT_TRUE(theMod.getPath().empty());
}

TEST(ModParserTests, primitivesCanBeSet)
{
	std::stringstream input;
	input << "name=modName\n";
	input << "path=modPath\n";
	commonItems::ModParser theMod;
	theMod.parseMod(input);

	EXPECT_EQ("modName", theMod.getName());
	EXPECT_EQ("modPath", theMod.getPath());
}

TEST(ModParserTests, pathCanBeSetFromArchive)
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