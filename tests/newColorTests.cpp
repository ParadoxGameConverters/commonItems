#include "../newColor.h"
#include "gtest/gtest.h"



TEST(NewColor_Tests, ColorDefaultsToUninitialized)
{
	const commonItems::newColor testColor;

	ASSERT_FALSE(commonItems::newColor{});
	ASSERT_EQ(0, testColor.r());
	ASSERT_EQ(0, testColor.g());
	ASSERT_EQ(0, testColor.b());
}


TEST(NewColor_Tests, ColorCanBeInitializedWithRGB)
{
	const commonItems::newColor testColor(2, 4, 8);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const commonItems::newColor testColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const commonItems::newColor testColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(NewColor_Tests, ColorCanBeFactoryInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(NewColor_Tests, ColorCanBeFactoryInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, testColor.r());
	ASSERT_EQ(4, testColor.g());
	ASSERT_EQ(8, testColor.b());
}


TEST(NewColor_Tests, ColorCanBeOnlyInitializedFromStreamWithAtLeastThreeColors)
{
	std::stringstream input;
	input << "= { 2 4 }";
	const commonItems::newColor testColor(input);

	ASSERT_FALSE(testColor);
}


TEST(NewColor_Tests, RGBCanBeExtracted)
{
	const commonItems::newColor testColor(2, 4, 8);

	auto r = 1;
	auto g = 1;
	auto b = 1;
	testColor.GetRGB(r, g, b);

	ASSERT_TRUE(testColor);
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(NewColor_Tests, ColorCanBeOutput)
{
	const commonItems::newColor testColor(2, 4, 8);

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("2 4 8", output.str());
}


// RandomlyFluctuate() isn't easily testable, so skipped