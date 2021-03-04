#include "../GameVersion.h"
#include "gtest/gtest.h"
#include <sstream>



TEST(GameVersion_Tests, GameVersionDefaultsToZeroZeroZeroZero)
{
	const commonItems::GameVersion version;

	std::stringstream output;
	output << version;

	ASSERT_EQ("0.0.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetDirectly)
{
	const commonItems::GameVersion version(1, 2, 3, 4);

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByFourPartString)
{
	const commonItems::GameVersion version("1.2.3.4");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByThreePartString)
{
	const commonItems::GameVersion version("1.2.3");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByTwoPartString)
{
	const commonItems::GameVersion version("1.2");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByOnePartString)
{
	const commonItems::GameVersion version("1");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.0.0.0", output.str());
}


TEST(GameVersion_Tests, GameVersionCanBeSetByEmptyString)
{
	const commonItems::GameVersion version("");

	std::stringstream output;
	output << version;

	ASSERT_EQ("0.0.0.0", output.str());
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
	const commonItems::GameVersion version(input);

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
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

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(GameVersion_Tests, EqualityCanBeTrue)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_EQ(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromFirstPart)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(2, 2, 3, 4);

	ASSERT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromSecondPart)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(1, 3, 3, 4);

	ASSERT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromThirdPart)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(1, 2, 4, 4);

	ASSERT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, InequalityCanBeTrueFromFourthPart)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(1, 2, 3, 5);

	ASSERT_NE(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromFourthPart)
{
	const commonItems::GameVersion version(1, 2, 3, 5);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromThirdPart)
{
	const commonItems::GameVersion version(1, 2, 4, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromSecondPart)
{
	const commonItems::GameVersion version(1, 3, 2, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanCanBeSetFromFirstPart)
{
	const commonItems::GameVersion version(2, 1, 2, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanOrEqualsFromGreaterThan)
{
	const commonItems::GameVersion version(2, 1, 2, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_GE(version, versionTwo);
}


TEST(GameVersion_Tests, GreaterThanOrEqualsFromEquals)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_GE(version, versionTwo);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromFourthPart)
{
	const commonItems::GameVersion version(1, 2, 3, 5);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromThirdPart)
{
	const commonItems::GameVersion version(1, 2, 4, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromSecondPart)
{
	const commonItems::GameVersion version(1, 3, 2, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanCanBeSetFromFirstPart)
{
	const commonItems::GameVersion version(2, 1, 2, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanOrEqualsFromGreaterThan)
{
	const commonItems::GameVersion version(2, 1, 2, 3);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_LE(versionTwo, version);
}


TEST(GameVersion_Tests, LessThanOrEqualsFromEquals)
{
	const commonItems::GameVersion version(1, 2, 3, 4);
	const commonItems::GameVersion versionTwo(1, 2, 3, 4);

	ASSERT_LE(versionTwo, version);
}

TEST(GameVersion_Tests, GameVersionEqualMissingFourthPartIsSameAsZero)
{
	const commonItems::GameVersion version("1.3.3.0");
	const commonItems::GameVersion requiredVersion("1.3.3");
	ASSERT_EQ(version, requiredVersion);
}

TEST(GameVersion_Tests, GameVersionNotEqualMissingFourthPartIsNotSameAsThirdPart)
{
	const commonItems::GameVersion version("1.3.3.3");
	const commonItems::GameVersion requiredVersion("1.3.3");
	ASSERT_NE(version, requiredVersion);
}