#include "../Version.h"
#include "gtest/gtest.h"
#include <sstream>



TEST(Version_Tests, VersionDefaultsToZeroZeroZeroZero)
{
	const Version version;

	std::stringstream output;
	output << version;

	ASSERT_EQ("0.0.0.0", output.str());
}


TEST(Version_Tests, VersionCanBeSetDirectly)
{
	const Version version(1, 2, 3, 4);

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(Version_Tests, VersionCanBeSetByFourPartString)
{
	const Version version("1.2.3.4");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(Version_Tests, VersionCanBeSetByThreePartString)
{
	const Version version("1.2.3");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.0", output.str());
}


TEST(Version_Tests, VersionCanBeSetByTwoPartString)
{
	const Version version("1.2");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.0.0", output.str());
}


TEST(Version_Tests, VersionCanBeSetByOnePartString)
{
	const Version version("1");

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.0.0.0", output.str());
}


TEST(Version_Tests, VersionCanBeSetByEmptyString)
{
	const Version version("");

	std::stringstream output;
	output << version;

	ASSERT_EQ("0.0.0.0", output.str());
}


TEST(Version_Tests, VersionCanBeSetByStream)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tfirst = 1\n";
	input << "\tsecond = 2\n";
	input << "\tthird = 3\n";
	input << "\tforth = 4\n"; // paradox's misspelling
	input << "}";
	const Version version(input);

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(Version_Tests, VersionCanBeSetByFactory)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tfirst = 1\n";
	input << "\tsecond = 2\n";
	input << "\tthird = 3\n";
	input << "\tforth = 4\n"; // paradox's misspelling
	input << "}";
	const auto version = Version::Factory{}.getVersion(input);

	std::stringstream output;
	output << version;

	ASSERT_EQ("1.2.3.4", output.str());
}


TEST(Version_Tests, EqualityCanBeTrue)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_EQ(version, versionTwo);
}


TEST(Version_Tests, InequalityCanBeTrueFromFirstPart)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(2, 2, 3, 4);

	ASSERT_NE(version, versionTwo);
}


TEST(Version_Tests, InequalityCanBeTrueFromSecondPart)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(1, 3, 3, 4);

	ASSERT_NE(version, versionTwo);
}


TEST(Version_Tests, InequalityCanBeTrueFromThirdPart)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(1, 2, 4, 4);

	ASSERT_NE(version, versionTwo);
}


TEST(Version_Tests, InequalityCanBeTrueFromFourthPart)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(1, 2, 3, 5);

	ASSERT_NE(version, versionTwo);
}


TEST(Version_Tests, GreaterThanCanBeSetFromFourthPart)
{
	const Version version(1, 2, 3, 5);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(Version_Tests, GreaterThanCanBeSetFromThirdPart)
{
	const Version version(1, 2, 4, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(Version_Tests, GreaterThanCanBeSetFromSecondPart)
{
	const Version version(1, 3, 2, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(Version_Tests, GreaterThanCanBeSetFromFirstPart)
{
	const Version version(2, 1, 2, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_GT(version, versionTwo);
}


TEST(Version_Tests, GreaterThanOrEqualsFromGreaterThan)
{
	const Version version(2, 1, 2, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_GE(version, versionTwo);
}


TEST(Version_Tests, GreaterThanOrEqualsFromEquals)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_GE(version, versionTwo);
}


TEST(Version_Tests, LessThanCanBeSetFromFourthPart)
{
	const Version version(1, 2, 3, 5);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(Version_Tests, LessThanCanBeSetFromThirdPart)
{
	const Version version(1, 2, 4, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(Version_Tests, LessThanCanBeSetFromSecondPart)
{
	const Version version(1, 3, 2, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(Version_Tests, LessThanCanBeSetFromFirstPart)
{
	const Version version(2, 1, 2, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_LT(versionTwo, version);
}


TEST(Version_Tests, LessThanOrEqualsFromGreaterThan)
{
	const Version version(2, 1, 2, 3);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_LE(versionTwo, version);
}


TEST(Version_Tests, LessThanOrEqualsFromEquals)
{
	const Version version(1, 2, 3, 4);
	const Version versionTwo(1, 2, 3, 4);

	ASSERT_LE(versionTwo, version);
}