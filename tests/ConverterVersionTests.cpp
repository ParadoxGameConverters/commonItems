#include "../ConverterVersion.h"
#include <gtest/gtest.h>

TEST(ConverterVersion_Tests, ItemsDefaultToEmpty)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	commonItems::ConverterVersion converterVersion;
	// Using std::string_view version because it calls the other
	converterVersion.loadVersion(std::string("emptyVersion.txt"));

	EXPECT_TRUE(converterVersion.getName().empty());
	EXPECT_TRUE(converterVersion.getVersion().empty());
	EXPECT_TRUE(converterVersion.getSource().empty());
	EXPECT_TRUE(converterVersion.getTarget().empty());
	EXPECT_EQ(GameVersion(), converterVersion.getMinSource());
	EXPECT_EQ(GameVersion(), converterVersion.getMaxSource());
	EXPECT_EQ(GameVersion(), converterVersion.getMinTarget());
	EXPECT_EQ(GameVersion(), converterVersion.getMaxTarget());
#pragma warning(pop)
}


TEST(ConverterVersion_Tests, ItemsCanBeImported)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	commonItems::ConverterVersion converterVersion;
	// Using std::string_view version because it calls the other
	converterVersion.loadVersion(std::string("version.txt"));

	EXPECT_EQ("Adams-prerelease", converterVersion.getName());
	EXPECT_EQ("0.1A", converterVersion.getVersion());
	EXPECT_EQ("EU4", converterVersion.getSource());
	EXPECT_EQ("Vic3", converterVersion.getTarget());
	EXPECT_EQ(GameVersion("1.31"), converterVersion.getMinSource());
	EXPECT_EQ(GameVersion("1.31.7"), converterVersion.getMaxSource());
	EXPECT_EQ(GameVersion("1.0"), converterVersion.getMinTarget());
	EXPECT_EQ(GameVersion("1.1"), converterVersion.getMaxTarget());
#pragma warning(pop)
}


TEST(ConverterVersion_Tests, DescriptionCanBeConstructed)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	commonItems::ConverterVersion converterVersion;
	// Using std::string_view version because it calls the other
	converterVersion.loadVersion(std::string("version.txt"));

	EXPECT_EQ("Compatible with EU4 [v1.31-v1.31.7] and Vic3 [v1.0-v1.1]", converterVersion.getDescription());
#pragma warning(pop)
}

TEST(ConverterVersion_Tests, DescriptionDoesNotDuplicateIdenticalVersions)
{
	std::stringstream input;
	input << "source = \"EU4\"\n";
	input << "target = \"Vic3\"\n";
	input << "minSource = \"1.31\"\n";
	input << "maxSource = \"1.31\"\n";
	input << "minTarget = \"1.0\"\n";
	input << "maxTarget = \"1.0\"\n";
	commonItems::ConverterVersion converterVersion;
	converterVersion.loadVersion(input);

	EXPECT_EQ("Compatible with EU4 [v1.31] and Vic3 [v1.0]", converterVersion.getDescription());
}

TEST(ConverterVersion_Tests, ConverterVersionCanBeOutput)
{
#pragma warning(push)
#pragma warning(disable : 4996)
	commonItems::ConverterVersion converterVersion;
	// Using std::string_view version because it calls the other
	converterVersion.loadVersion(std::string("version.txt"));

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	std::cout << converterVersion;
	std::cout.rdbuf(stdOutBuf);
	std::string actualOutput = log.str();
	std::stringstream actualStream(actualOutput);

	const std::string expectedOutput =
		 "\n\n"
		 "************ -= The Paradox Game Converters Group =- *****************\n"
		 "* Converter version 0.1A \"Adams-prerelease\"\n"
		 "* Compatible with EU4 [v1.31-v1.31.7] and Vic3 [v1.0-v1.1]\n"
		 "* Built on [some timestamp]\n"
		 "************************** + EU4 To Vic3 + ***************************\n";
	std::stringstream expectedStream(expectedOutput);

	std::string expectedLine;
	std::string actualLine;

	for (auto counter = 0; counter < 5; ++counter) // first 5 lines
	{
		std::getline(expectedStream, expectedLine);
		std::getline(actualStream, actualLine);
		EXPECT_EQ(expectedLine, actualLine);
	}

	std::getline(expectedStream, expectedLine);
	std::getline(actualStream, actualLine);
	EXPECT_NE(expectedLine, actualLine); // Can't match timestamps between build and tests.

	std::getline(expectedStream, expectedLine);
	std::getline(actualStream, actualLine);
	EXPECT_EQ(expectedLine, actualLine); // footer line
#pragma warning(pop)
}

TEST(ConverterVersion_Tests, ConverterVersionOutputSkipsIncompleteVersionOrName)
{
	std::stringstream input;
	input << "source = \"EU4\"\n";
	input << "target = \"Vic3\"\n";
	input << "minSource = \"1.31\"\n";
	input << "maxSource = \"1.31\"\n";
	input << "minTarget = \"1.0\"\n";
	input << "maxTarget = \"1.0\"\n";
	commonItems::ConverterVersion converterVersion;
	converterVersion.loadVersion(input);

	std::stringstream log;
	auto stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());
	std::cout << converterVersion;
	std::cout.rdbuf(stdOutBuf);
	std::string actualOutput = log.str();
	std::stringstream actualStream(actualOutput);

	const std::string expectedOutput =
		 "\n\n"
		 "************ -= The Paradox Game Converters Group =- *****************\n"
		 "* Compatible with EU4 [v1.31] and Vic3 [v1.0]\n"
		 "* Built on [some timestamp]\n"
		 "************************** + EU4 To Vic3 + ***************************\n";
	std::stringstream expectedStream(expectedOutput);

	std::string expectedLine;
	std::string actualLine;

	for (auto counter = 0; counter < 4; ++counter) // first 4 lines
	{
		std::getline(expectedStream, expectedLine);
		std::getline(actualStream, actualLine);
		EXPECT_EQ(expectedLine, actualLine);
	}

	std::getline(expectedStream, expectedLine);
	std::getline(actualStream, actualLine);
	EXPECT_NE(expectedLine, actualLine); // Can't match timestamps between build and tests.

	std::getline(expectedStream, expectedLine);
	std::getline(actualStream, actualLine);
	EXPECT_EQ(expectedLine, actualLine); // footer line
}
