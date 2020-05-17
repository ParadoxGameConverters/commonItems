#include "../CommonFunctions.h"
#include "gtest/gtest.h"



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

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(TrimPath_Tests, TrimPathTrimsBackslashes)
{
	const std::string input = R"(c:\this\is\a\path.txt)";

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(TrimPath_Tests, TrimPathTrimsMixedSlashes)
{
	const std::string input = R"(c:\this\is/a/path.txt)";

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(TrimPath_Tests, TrimPathTrimsReversedMixedSlashes)
{
	const std::string input = R"(/this/is\a\path.txt)";

	ASSERT_EQ(trimPath(input), "path.txt");
}

TEST(TrimExtension_Tests, TrimExtensionTrimsDot)
{
	const std::string input = R"(file.extension)";

	ASSERT_EQ(trimExtension(input), "file");
}

TEST(TrimExtension_Tests, TrimExtensionTrimsLastDot)
{
	const std::string input = R"(file.name.with.extension)";

	ASSERT_EQ(trimExtension(input), "file.name.with");
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
