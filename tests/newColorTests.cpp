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
	const commonItems::newColor testColor({0.1, 1.0, 0.4}, commonItems::newColor::ColorSpaces::HSV);

	auto [r, g, b] = testColor.getComponents();
	ASSERT_EQ(0.1, r);
	ASSERT_EQ(1.0, g);
	ASSERT_EQ(0.4, b);
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
	input << "= hsv { 0.1 1.0 0.4 }";
	const auto testColor = commonItems::newColor::Factory::getColor(input);

	auto [h, s, v] = testColor.getComponents();
	ASSERT_EQ(0.1, h);
	ASSERT_EQ(1.0, s);
	ASSERT_EQ(0.4, v);
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
	const commonItems::newColor testColor({0.1, 1.0, 0.4}, commonItems::newColor::ColorSpaces::HSV);

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("= hsv { 0.1 1.0 0.4 }", output.str());
}


TEST(NewColor_Tests, UnequalFromFirstComponent)
{
	const commonItems::newColor colorOne({2, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);
	const commonItems::newColor colorTwo({3, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);

	ASSERT_NE(colorOne, colorTwo);
}


TEST(NewColor_Tests, UnequalFromSecondComponent)
{
	const commonItems::newColor colorOne({2, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);
	const commonItems::newColor colorTwo({2, 5, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);

	ASSERT_NE(colorOne, colorTwo);
}


TEST(NewColor_Tests, UnequalFromThirdComponent)
{
	const commonItems::newColor colorOne({2, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);
	const commonItems::newColor colorTwo({2, 4, 9}, commonItems::newColor::ColorSpaces::UNSPECIFIED);

	ASSERT_NE(colorOne, colorTwo);
}


TEST(NewColor_Tests, UnequalFromColorspace)
{
	const commonItems::newColor colorOne({2, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);
	const commonItems::newColor colorTwo({2, 4, 8}, commonItems::newColor::ColorSpaces::RGB);

	ASSERT_NE(colorOne, colorTwo);
}


TEST(NewColor_Tests, Equality)
{
	const commonItems::newColor colorOne({2, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);
	const commonItems::newColor colorTwo({2, 4, 8}, commonItems::newColor::ColorSpaces::UNSPECIFIED);

	ASSERT_EQ(colorOne, colorTwo);
}


// RandomlyFluctuate() isn't easily testable, so skipped