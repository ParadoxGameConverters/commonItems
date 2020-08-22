#include "../Color.h"
#include "gtest/gtest.h"



TEST(Color_Tests, ColorDefaultsToUninitialized)
{
	const commonItems::Color testColor;

	ASSERT_FALSE(commonItems::Color{});
	ASSERT_EQ(0, testColor.r());
	ASSERT_EQ(0, testColor.g());
	ASSERT_EQ(0, testColor.b());
}


TEST(Color_Tests, ColorCanBeInitializedWithRGB)
{
	const commonItems::Color testColor(2, 4, 8);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(Color_Tests, ColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const commonItems::Color testColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const commonItems::Color testColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(Color_Tests, ColorCanBeFactoryInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const auto testColor = commonItems::Color::Factory::getColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(Color_Tests, ColorCanBeFactoryInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const auto testColor = commonItems::Color::Factory::getColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(Color_Tests, ColorCanBeOnlyInitializedFromStreamWithAtLeastThreeColors)
{
	std::stringstream input;
	input << "= { 2 4 }";
	const commonItems::Color testColor(input);

	ASSERT_FALSE(testColor);
}


TEST(Color_Tests, RGBCanBeExtracted)
{
	const commonItems::Color testColor(2, 4, 8);

	auto r = 1;
	auto g = 1;
	auto b = 1;
	testColor.GetRGB(r, g, b);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(Color_Tests, ColorCanBeOutput)
{
	const commonItems::Color testColor(2, 4, 8);

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("2 4 8", output.str());
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamWithColorSpacePrefix)
{
	std::stringstream input;
	input << "= rgb { 2 4 8 }";
	const commonItems::Color testColor(input, true);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}

TEST(Color_Tests, ColorCanBeInitializedFromStreamWithQuotesWithColorSpacePrefix)
{
	std::stringstream input;
	input << R"(= rgb { "2" "4" "8" })";
	const commonItems::Color testColor(input, true);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(Color_Tests, ColorCanBeOnlyInitializedFromStreamWithAtLeastThreeColorsWithColorSpacePrefix)
{
	std::stringstream input;
	input << "= rgb { 2 4 }";
	const commonItems::Color testColor(input, true);

	ASSERT_FALSE(testColor);
}

// RandomlyFluctuate() isn't easily testable, so skipped