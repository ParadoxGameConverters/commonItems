#include "../Color.h"
#include "gtest/gtest.h"



TEST(Color_Tests, ColorDefaultsToBlack)
{
	const commonItems::Color testColor;

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(0, r);
	ASSERT_EQ(0, g);
	ASSERT_EQ(0, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
	ASSERT_NEAR(0.0f, s, 0.01);
	ASSERT_NEAR(0.0f, v, 0.01);
}


TEST(Color_Tests, ColorCanBeInitializedWithRgbComponents)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorCanBeInitializedWithHsvComponents)
{
	const commonItems::Color testColor(std::array<float, 3>{0.5f, 0.5f, 0.5f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, HsvConversion_GreyHasZeroHue)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 128, 128});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_RedHasHueOfZero)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 0, 0});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_YellowHasHueOfOneSixth)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 128, 64});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.167f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_GreenHasHueOfOneThird)
{
	const commonItems::Color testColor(std::array<int, 3>{0, 128, 0});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.333f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_CyanHasHueOfOneHalf)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_BlueHasHueOfTwoThirds)
{
	const commonItems::Color testColor(std::array<int, 3>{0, 0, 128});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.667f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_MagentaHasHueOfFiveSixths)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 64, 128});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.833f, h, 0.01);
}


TEST(Color_Tests, HsvConversion_BlackHasZeroSaturation)
{
	const commonItems::Color testColor(std::array<int, 3>{0, 0, 0});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.0f, s, 0.01);
}


TEST(Color_Tests, HsvConversion_GreyHasZeroSaturation)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 128, 128});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.0f, s, 0.01);
}


TEST(Color_Tests, HsvConversion_ColorHasSaturation)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 128, 64});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, s, 0.01);
}


TEST(Color_Tests, HsvConversion_BlackHasZeroValue)
{
	const commonItems::Color testColor(std::array<int, 3>{0, 0, 0});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.0f, v, 0.01);
}


TEST(Color_Tests, HsvConversion_ColorHasValue)
{
	const commonItems::Color testColor(std::array<int, 3>{128, 64, 64});

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, RgbConversion_ZeroHueGivesRed)
{
	const commonItems::Color testColor(std::array<float, 3>{0.0f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(Color_Tests, RgbConversion_OneSixthHueGivesYellow)
{
	const commonItems::Color testColor(std::array<float, 3>{0.167f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(254, r, 1);
	ASSERT_NEAR(254, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(Color_Tests, RgbConversion_OneThirdHueGivesGreen)
{
	const commonItems::Color testColor(std::array<float, 3>{0.333f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(255, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(Color_Tests, RgbConversion_OneHalfHueGivesCyan)
{
	const commonItems::Color testColor(std::array<float, 3>{0.5f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(255, g, 1);
	ASSERT_NEAR(255, b, 1);
}


TEST(Color_Tests, RgbConversion_TwoThirdHueGivesBlue)
{
	const commonItems::Color testColor(std::array<float, 3>{0.667f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(255, b, 1);
}


TEST(Color_Tests, RgbConversion_FiveSixthsHueGivesMagenta)
{
	const commonItems::Color testColor(std::array<float, 3>{0.833f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(254, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(254, b, 1);
}


TEST(Color_Tests, RgbConversion_ZeroSaturationIsGreyscale)
{
	const commonItems::Color testColor(std::array<float, 3>{0.0f, 0.0f, 0.5f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(127, r, 1);
	ASSERT_NEAR(127, g, 1);
	ASSERT_NEAR(127, b, 1);
}


TEST(Color_Tests, RgbConversion_FullSaturationisPureColor)
{
	const commonItems::Color testColor(std::array<float, 3>{0.0f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(Color_Tests, RgbConversion_SaturationWhitensColor)
{
	const commonItems::Color testColor(std::array<float, 3>{0.0f, 0.5f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(127, g, 1);
	ASSERT_NEAR(127, b, 1);
}


TEST(Color_Tests, RgbConversion_ZeroValueIsBlack)
{
	const commonItems::Color testColor(std::array<float, 3>{0.0f, 1.0f, 0.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(Color_Tests, RgbConversion_ValueDarkensColor)
{
	const commonItems::Color testColor(std::array<float, 3>{0.0f, 1.0f, 0.5f});

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_NEAR(127, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(Color_Tests, ColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 64 128 128 }";
	const auto testColor = commonItems::Color::Factory::getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "64" "128" "128" })";
	const auto testColor = commonItems::Color::Factory::getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorInitializationRequiresThreeComponentsWhenUnspecified)
{
	std::stringstream input;
	input << "= { 64 128 }";

	ASSERT_THROW(commonItems::Color::Factory{}.getColor(input), std::runtime_error);
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamInRgb)
{
	std::stringstream input;
	input << "= rgb { 64 128 128 }";
	const auto testColor = commonItems::Color::Factory::getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorInitializationRequiresThreeComponentsWhenRgb)
{
	std::stringstream input;
	input << "= rgb { 64 128 }";

	ASSERT_THROW(commonItems::Color::Factory{}.getColor(input), std::runtime_error);
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamInHsv)
{
	std::stringstream input;
	input << "= hsv { 0.5 0.5 0.5 }";
	const auto testColor = commonItems::Color::Factory::getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorInitializationRequiresThreeComponentsWhenHsv)
{
	std::stringstream input;
	input << "= hsv { 0.333 0.5 }";

	ASSERT_THROW(commonItems::Color::Factory{}.getColor(input), std::runtime_error);
}


class foo: commonItems::parser
{
  public:
	explicit foo(std::istream& theStream)
	{
		registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
			color = commonItems::Color::Factory::getColor(theStream);
		});
		parseStream(theStream);
	}

	commonItems::Color color;
};

TEST(Color_Tests, ColorCanBeInitializedFromLongerStream)
{
	std::stringstream input;
	input << "= { color = { 64 128 128 } } more text";
	const foo bar(input);

	auto [r, g, b] = bar.color.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = bar.color.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" more text", std::string{buffer});
}


TEST(Color_Tests, ColorCanBeInitializedInRgbFromLongerStream)
{
	std::stringstream input;
	input << "= { color = rgb { 64 128 128 } } more text";
	const foo bar(input);

	auto [r, g, b] = bar.color.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = bar.color.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" more text", std::string{buffer});
}


TEST(Color_Tests, ColorCanBeInitializedInHsvFromLongerStream)
{
	std::stringstream input;
	input << "= { color = hsv { 0.5 0.5 0.5 } } more text";
	const foo bar(input);

	auto [r, g, b] = bar.color.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = bar.color.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" more text", std::string{buffer});
}


TEST(Color_Tests, ColorCanBeOutputInUnspecifiedColorSpace)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testColor;

	ASSERT_EQ("= { 64 128 128 }", output.str());
}


TEST(Color_Tests, ColorCanBeOutputInRgbColorSpace)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testColor.outputRgb();

	ASSERT_EQ("= rgb { 64 128 128 }", output.str());
}


TEST(Color_Tests, ColorCanBeOutputInHsvColorSpace)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testColor.outputHsv();

	ASSERT_EQ("= hsv { 0.5 0.5 0.5 }", output.str());
}


TEST(Color_Tests, UnequalFromDifferentRgb)
{
	const commonItems::Color colorOne(std::array<int, 3>{2, 4, 8});
	const commonItems::Color colorTwo(std::array<int, 3>{3, 4, 8});

	ASSERT_NE(colorOne, colorTwo);
}


TEST(Color_Tests, UnequalFromDifferentHsv)
{
	const commonItems::Color colorOne(std::array<float, 3>{0.333f, 0.50f, 0.50f});
	const commonItems::Color colorTwo(std::array<float, 3>{0.333f, 0.75f, 0.75f});

	ASSERT_NE(colorOne, colorTwo);
}


TEST(Color_Tests, Equality)
{
	const commonItems::Color colorOne(std::array<int, 3>{2, 4, 8});
	const commonItems::Color colorTwo(std::array<int, 3>{2, 4, 8});

	ASSERT_EQ(colorOne, colorTwo);
}


// RandomlyFluctuate() isn't easily testable, so skipped