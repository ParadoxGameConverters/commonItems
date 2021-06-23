#include "../GameVersion.h"
#include "gtest/gtest.h"


TEST(GameVersion_Tests, GameVersionDefaultsToZeroZeroZeroZero)
{
	const GameVersion version;

	std::stringstream output;
	output << version;

	EXPECT_EQ("0.0.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetDirectly)
{
	const GameVersion version(1, 2, 3, 4);

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByFourPartString)
{
	const GameVersion version("1.2.3.4");

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByThreePartString)
{
	const GameVersion version("1.2.3");

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.2.3.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByTwoPartString)
{
	const GameVersion version("1.2");

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.2.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByOnePartString)
{
	const GameVersion version("1");

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.0.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByEmptyString)
{
	const GameVersion version("");

	std::stringstream output;
	output << version;

	EXPECT_EQ("0.0.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByStream)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tfirst = 1\n";
	input << "\tsecond = 2\n";
	input << "\tthird = 3\n";
	input << "\tforth = 4\n"; // paradox's misspelling
	input << "}";
	const GameVersion version(input);

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByFactory)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tfirst = 1\n";
	input << "\tsecond = 2\n";
	input << "\tthird = 3\n";
	input << "\tforth = 4\n"; // paradox's misspelling
	input << "}";
	const auto version = GameVersion::Factory{}.getVersion(input);

	std::stringstream output;
	output << version;

	EXPECT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, EqualityCanBeTrue)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_EQ(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromFirstPart)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(2, 2, 3, 4);

	EXPECT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromSecondPart)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(1, 3, 3, 4);

	EXPECT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromThirdPart)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(1, 2, 4, 4);

	EXPECT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromFourthPart)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(1, 2, 3, 5);

	EXPECT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromFourthPart)
{
	const GameVersion version(1, 2, 3, 5);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromThirdPart)
{
	const GameVersion version(1, 2, 4, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromSecondPart)
{
	const GameVersion version(1, 3, 2, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromFirstPart)
{
	const GameVersion version(2, 1, 2, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanOrEqualsFromGreaterThan)
{
	const GameVersion version(2, 1, 2, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_GE(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanOrEqualsFromEquals)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_GE(version, versionTwo);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromFourthPart)
{
	const GameVersion version(1, 2, 3, 5);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromThirdPart)
{
	const GameVersion version(1, 2, 4, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromSecondPart)
{
	const GameVersion version(1, 3, 2, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromFirstPart)
{
	const GameVersion version(2, 1, 2, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanOrEqualsFromGreaterThan)
{
	const GameVersion version(2, 1, 2, 3);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_LE(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanOrEqualsFromEquals)
{
	const GameVersion version(1, 2, 3, 4);
	const GameVersion versionTwo(1, 2, 3, 4);

	EXPECT_LE(versionTwo, version);
}

TEST(GameVersion_Tests, GameVersionEqualMissingFourthPartIsSameAsZero)
{
	const GameVersion version("1.3.3.0");
	const GameVersion requiredVersion("1.3.3");
	EXPECT_EQ(version, requiredVersion);
}

TEST(GameVersion_Tests, GameVersionNotEqualMissingFourthPartIsNotSameAsThirdPart)
{
	const GameVersion version("1.3.3.3");
	const GameVersion requiredVersion("1.3.3");
	EXPECT_NE(version, requiredVersion);
}

TEST(GameVersion_Tests, GameVersionFullNameReturned)
{
	const GameVersion version1("1.3.0.3");
	const GameVersion version2("1.3.0");
	const GameVersion version3("1.3");
	const GameVersion version4("1");
	const GameVersion version5("0");

	EXPECT_EQ("1.3.0.3", version1.toString());
	EXPECT_EQ("1.3.0.0", version2.toString());
	EXPECT_EQ("1.3.0.0", version3.toString());
	EXPECT_EQ("1.0.0.0", version4.toString());
	EXPECT_EQ("0.0.0.0", version5.toString());
}

TEST(GameVersion_Tests, GameVersionShortNameReturned)
{
	const GameVersion version1("1.3.0.3");
	const GameVersion version2("1.3.0");
	const GameVersion version3("1.0");
	const GameVersion version4("1.0.0.1");
	const GameVersion version5("0.0.0.0");

	EXPECT_EQ("1.3.0.3", version1.toShortString());
	EXPECT_EQ("1.3", version2.toShortString());
	EXPECT_EQ("1", version3.toShortString());
	EXPECT_EQ("1.0.0.1", version4.toShortString());
	EXPECT_EQ("0", version5.toShortString());
}

TEST(GameVersion_Tests, LargerishFalseForLarger)
{
	const GameVersion requiredVersion("2.1.1.1");
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("2.1.1.2")));
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("2.1.2.0")));
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("2.2.0.0")));
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("3.0.0.0")));
	
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("2.1.2")));
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("2.2")));
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("3")));
}

TEST(GameVersion_Tests, LargerishTrueForSmaller)
{
	const GameVersion requiredVersion("2.1.1.1");
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.0.0.0")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1.0.0")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1.1.0")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("1.0.0.0")));

	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1.0")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("1")));

	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("0.0.0.0")));
}

TEST(GameVersion_Tests, LargerishTrueForOvershootingSmallerish)
{
	// This is the main meat.

	const GameVersion requiredVersion("2.1");
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1.99.0")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1.99.99")));
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("2.1.1.1")));
}

TEST(GameVersion_Tests, LargerishFalseForLargerish)
{
	const GameVersion requiredVersion("2.1");
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("2.2")));
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("3.0")));
}

TEST(GameVersion_Tests, LargerishZeroTrueForZero)
{
	const GameVersion requiredVersion("0.0.0.0");
	EXPECT_TRUE(requiredVersion.isLargerishThan(GameVersion("0.0.0.0")));
}

TEST(GameVersion_Tests, LargerishForActualIntendedZeroWithSubversions)
{
	auto requiredVersion = GameVersion("1.0.9");
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("1.1")));

	requiredVersion = GameVersion("1.0.0.9");
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("1.0.1")));
}

TEST(GameVersion_Tests, LargerishForActualIntendedZeroWithoutSubversions)
{
	auto requiredVersion = GameVersion("1.0");
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("1.1")));

	requiredVersion = GameVersion("1.0.0");
	EXPECT_FALSE(requiredVersion.isLargerishThan(GameVersion("1.9.1")));
}
