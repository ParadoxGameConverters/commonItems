#include "../newColor.h"
#include "gtest/gtest.h"



TEST(NewColor_Tests, ColorDefaultsToUninitialized)
{
	const commonItems::newColor testColor;

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(0, r);
	ASSERT_EQ(0, g);
	ASSERT_EQ(0, b);
}


TEST(NewColor_Tests, ColorCanBeInitializedWithRGB)
{
	const commonItems::newColor testColor(2, 4, 8);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const commonItems::newColor testColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const commonItems::newColor testColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(NewColor_Tests, ColorCanBeFactoryInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(NewColor_Tests, ColorCanBeFactoryInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
}


TEST(NewColor_Tests, ColorCanBeOnlyInitializedFromStreamWithAtLeastThreeColors)
{
	std::stringstream input;
	input << "= { 2 4 }";
	const commonItems::newColor testColor(input);

	ASSERT_FALSE(true);
}


TEST(NewColor_Tests, ColorCanBeOutput)
{
	const commonItems::newColor testColor(2, 4, 8);

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("2 4 8", output.str());
}


// RandomlyFluctuate() isn't easily testable, so skipped