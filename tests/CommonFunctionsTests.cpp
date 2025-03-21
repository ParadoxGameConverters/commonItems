#include "../CommonFunctions.h"
#include <gtest/gtest.h>



TEST(CardinalToOrdinal_Tests, LastDigitOneGivesSt)
{
	ASSERT_EQ("st", cardinalToOrdinal(1));
}


TEST(CardinalToOrdinal_Tests, LastDigitTwoGivesNd)
{
	ASSERT_EQ("nd", cardinalToOrdinal(2));
}


TEST(CardinalToOrdinal_Tests, LastDigitThreeGivesRd)
{
	ASSERT_EQ("rd", cardinalToOrdinal(3));
}


TEST(CardinalToOrdinal_Tests, RemainingDigitsGiveTh)
{
	ASSERT_EQ("th", cardinalToOrdinal(4));
	ASSERT_EQ("th", cardinalToOrdinal(5));
	ASSERT_EQ("th", cardinalToOrdinal(6));
	ASSERT_EQ("th", cardinalToOrdinal(7));
	ASSERT_EQ("th", cardinalToOrdinal(8));
	ASSERT_EQ("th", cardinalToOrdinal(9));
	ASSERT_EQ("th", cardinalToOrdinal(0));
}


TEST(CardinalToOrdinal_Tests, TeensGiveTh)
{
	ASSERT_EQ("th", cardinalToOrdinal(10));
	ASSERT_EQ("th", cardinalToOrdinal(11));
	ASSERT_EQ("th", cardinalToOrdinal(12));
	ASSERT_EQ("th", cardinalToOrdinal(13));
}

TEST(TrimPath_Tests, TrimPathTrimsSlashes)
{
	const std::string input = R"(/this/is/a/path.txt)";
#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimPath(input), "path.txt");
#pragma warning(pop)
}

TEST(TrimPath_Tests, TrimPathTrimsBackslashes)
{
	const std::string input = R"(c:\this\is\a\path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimPath(input), "path.txt");
#pragma warning(pop)
}

TEST(TrimPath_Tests, TrimPathTrimsMixedSlashes)
{
	const std::string input = R"(c:\this\is/a/path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimPath(input), "path.txt");
#pragma warning(pop)
}

TEST(TrimPath_Tests, TrimPathTrimsReversedMixedSlashes)
{
	const std::string input = R"(/this/is\a\path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimPath(input), "path.txt");
#pragma warning(pop)
}

TEST(TrimPath_Tests, TrimPathDoesNotAffectRawFiles)
{
	const std::string input = R"(path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimPath(input), "path.txt");
#pragma warning(pop)
}

TEST(GetPath_Tests, GetPathGetsSlashedPath)
{
	const std::string input = R"(/this/is/a/path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ("/this/is/a/", getPath(input));
#pragma warning(pop)
}

TEST(GetPath_Tests, GetPathGetsBackslashedPath)
{
	const std::string input = R"(c:\this\is\a\path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(R"(c:\this\is\a\)", getPath(input));
#pragma warning(pop)
}

TEST(GetPath_Tests, GetPathGetsMixedSlashedPath)
{
	const std::string input = R"(c:\this\is/a/path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(R"(c:\this\is/a/)", getPath(input));
#pragma warning(pop)
}

TEST(GetPath_Tests, GetPathGetsReversedMixedSlashedPath)
{
	const std::string input = R"(/this/is\a\path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(R"(/this/is\a\)", getPath(input));
#pragma warning(pop)
}

TEST(GetPath_Tests, GetPathReturnsBlankStringForRawFiles)
{
	const std::string input = R"(path.txt)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_TRUE(getPath(input).empty());
#pragma warning(pop)
}

TEST(TrimExtension_Tests, TrimExtensionTrimsDot)
{
	const std::string input = R"(file.extension)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimExtension(input), "file");
#pragma warning(pop)
}

TEST(TrimExtension_Tests, TrimExtensionTrimsLastDot)
{
	const std::string input = R"(file.name.with.extension)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimExtension(input), "file.name.with");
#pragma warning(pop)
}

TEST(TrimExtension_Tests, TrimExtensionDoesNotAffectDirectories)
{
	const std::string input = R"(/path/with.extension/filename)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ(trimExtension(input), "/path/with.extension/filename");
#pragma warning(pop)
}

TEST(GetExtension_Tests, GetExtensionGetsPostDot)
{
	const std::string input = R"(file.extension)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ("extension", getExtension(input));
#pragma warning(pop)
}

TEST(GetExtension_Tests, GetExtensionGetsPostLastDot)
{
	const std::string input = R"(file.name.with.extension)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ("extension", getExtension(input));
#pragma warning(pop)
}

TEST(GetExtension_Tests, GetExtensionReturnsEmptyStringForNoExtension)
{
	const std::string input = R"(filename)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_TRUE(getExtension(input).empty());
#pragma warning(pop)
}

TEST(GetExtension_Tests, GetExtensionDoesNotAffectDirectories)
{
	const std::string input = R"(/path/with.extension/directoryname)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_TRUE(getExtension(input).empty());
#pragma warning(pop)
}

TEST(GetExtension_Tests, GetExtensionWorksOnAbsolutePaths)
{
	const std::string input = R"(c:\path/with.extension/filename.mod)";

#pragma warning(push)
#pragma warning(disable : 4996)
	ASSERT_EQ("mod", getExtension(input));
#pragma warning(pop)
}

TEST(ReplaceCharacter_Tests, ReplaceCharacterCanReplaceSpaces)
{
	const std::string input = R"(a file name.eu4)";

	ASSERT_EQ(replaceCharacter(input, ' '), "a_file_name.eu4");
}

TEST(ReplaceCharacter_Tests, ReplaceCharacterCanReplaceMinuses)
{
	const std::string input = R"(a file-with-name.eu4)";

	ASSERT_EQ(replaceCharacter(input, '-'), "a file_with_name.eu4");
}

TEST(CardinalToRoman_Tests, NumbersCanBeConverted)
{
	EXPECT_EQ("", cardinalToRoman(-15));
	EXPECT_EQ("", cardinalToRoman(0));
	EXPECT_EQ("I", cardinalToRoman(1));
	EXPECT_EQ("II", cardinalToRoman(2));
	EXPECT_EQ("VII", cardinalToRoman(7));
	EXPECT_EQ("IX", cardinalToRoman(9));
	EXPECT_EQ("XI", cardinalToRoman(11));
	EXPECT_EQ("XXXI", cardinalToRoman(31));
	EXPECT_EQ("LI", cardinalToRoman(51));
	EXPECT_EQ("CI", cardinalToRoman(101));
	EXPECT_EQ("DI", cardinalToRoman(501));
	EXPECT_EQ("MI", cardinalToRoman(1001));
}
