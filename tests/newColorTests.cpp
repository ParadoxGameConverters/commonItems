#include "../newColor.h"
#include "gtest/gtest.h"



TEST(NewColor_Tests, ColorDefaultsToBlack)
{
	const commonItems::newColor testColor;

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(0, r);
	ASSERT_EQ(0, g);
	ASSERT_EQ(0, b);
}


TEST(NewColor_Tests, ColorDefaultsToUnspecifiedColorspace)
{
	const commonItems::newColor testColor;

	ASSERT_EQ(commonItems::newColor::ColorSpaces::UNSPECIFIED, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeInitializedWithComponents)
{
	const commonItems::newColor testColor({2, 4, 8});

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::UNSPECIFIED, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeInitializedWithComponentsInRgb)
{
	const commonItems::newColor testColor({2, 4, 8}, commonItems::newColor::ColorSpaces::RGB);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::RGB, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeInitializedWithComponentsInHsv)
{
	const commonItems::newColor testColor({2, 4, 8}, commonItems::newColor::ColorSpaces::HSV);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::HSV, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 2 4 8 }";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::UNSPECIFIED, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "2" "4" "8" })";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::UNSPECIFIED, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorInitializationRequiresThreeComponents)
{
	std::stringstream input;
	input << "= { 2 4 }";

	ASSERT_THROW(commonItems::newColor::Factory{}.getColor(input), std::runtime_error);
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStreamInRgb)
{
	std::stringstream input;
	input << "= rgb { 2 4 8 }";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::RGB, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeInitializedFromStreamInHsv)
{
	std::stringstream input;
	input << "= hsv { 2 4 8 }";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::HSV, testColor.getColorSpace());
}


TEST(NewColor_Tests, InitializationIgnoresTrailingText)
{
	std::stringstream input;
	input << "= { 2 4 8 } moretext";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(2, r);
	ASSERT_EQ(4, g);
	ASSERT_EQ(8, b);
	ASSERT_EQ(commonItems::newColor::ColorSpaces::UNSPECIFIED, testColor.getColorSpace());
}


TEST(NewColor_Tests, ColorCanBeOutputInUnspecifiedColorSpace)
{
	const commonItems::newColor testColor({2, 4, 8});

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("= { 2 4 8 }", output.str());
}


TEST(NewColor_Tests, ColorCanBeOutputInRgbColorSpace)
{
	const commonItems::newColor testColor({2, 4, 8}, commonItems::newColor::ColorSpaces::RGB);

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("= rgb { 2 4 8 }", output.str());
}


TEST(NewColor_Tests, ColorCanBeOutputInHsvColorSpace)
{
	const commonItems::newColor testColor({2, 4, 8}, commonItems::newColor::ColorSpaces::HSV);

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("= hsv { 2 4 8 }", output.str());
}


// RandomlyFluctuate() isn't easily testable, so skipped