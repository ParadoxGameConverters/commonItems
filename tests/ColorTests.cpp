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


TEST(Color_Tests, RgbConversion_ExcessiveHueIsDiscarded)
{
	const commonItems::Color testColor(std::array<float, 3>{1.0f, 1.0f, 1.0f});
	const commonItems::Color testColor2(std::array<float, 3>{1.1f, 1.0f, 1.0f});

	auto [r, g, b] = testColor.getRgbComponents();
	auto [r2, g2, b2] = testColor2.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
	ASSERT_NEAR(255, r2, 1);
	ASSERT_NEAR(0, g2, 1);
	ASSERT_NEAR(0, b2, 1);
}


TEST(Color_Tests, ColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 64 128 128 }";
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}

TEST(Color_Tests, ColorHSVCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 0.9 0.9 0.9 }";
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(229, r);
	ASSERT_EQ(22, g);
	ASSERT_EQ(146, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.9f, h, 0.01);
	ASSERT_NEAR(0.9f, s, 0.01);
	ASSERT_NEAR(0.9f, v, 0.01);
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "64" "128" "128" })";
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

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
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

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


TEST(Color_Tests, ColorCanBeInitializedFromStreamInHex)
{
	std::stringstream input;
	input << "= hex { 408080 }";
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorInitializationRequiresSixDigitsWhenHex)
{
	std::stringstream input;
	input << "= hex { 12345 }";

	ASSERT_THROW(commonItems::Color::Factory{}.getColor(input), std::runtime_error);
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamInHsv)
{
	std::stringstream input;
	input << "= hsv { 0.5 0.5 0.5 }";
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

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


TEST(Color_Tests, ColorCanBeInitializedFromStreamInHsv360)
{
	std::stringstream input;
	input << "= hsv360 { 180 50 50 }";
	const auto testColor = commonItems::Color::Factory{}.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorInitializationRequiresThreeComponentsWhenHsv360)
{
	std::stringstream input;
	input << "= hsv360 { 120 50 }";

	ASSERT_THROW(commonItems::Color::Factory{}.getColor(input), std::runtime_error);
}


TEST(Color_Tests, ColorCanBeInitializedFromStreamWithName)
{
	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColor("dark_moderate_cyan", commonItems::Color(std::array<int, 3>{64, 128, 128}));
	
	std::stringstream input;
	input << "= dark_moderate_cyan";
	const auto testColor = colorFactory.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}

TEST(Color_Tests, ColorCanBeInitializedFromQuotedStreamWithName)
{
	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColor("dark_moderate_cyan", commonItems::Color(std::array<int, 3>{64, 128, 128}));

	std::stringstream input;
	input << "= \"dark_moderate_cyan\"";
	const auto testColor = colorFactory.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorInitializingRequiresCachedColorWhenUsingName)
{
	const auto colorFactory = commonItems::Color::Factory();
	
	std::stringstream input;
	input << "= dark_moderate_cyan";
	ASSERT_THROW(colorFactory.getColor(input), std::runtime_error);
}


TEST(Color_Tests, ColorCanBeCachedFromStream)
{
	auto colorFactory = commonItems::Color::Factory();

	std::stringstream cacheInput;
	cacheInput << "= rgb { 64 128 128 }";
	colorFactory.addNamedColor("dark_moderate_cyan", cacheInput);

	std::stringstream input;
	input << "= dark_moderate_cyan";
	const auto testColor = colorFactory.getColor(input);

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(Color_Tests, ColorCanBeInitializedWithName)
{
	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColor("dark_moderate_cyan", commonItems::Color(std::array<int, 3>{64, 128, 128}));

	const auto testColor = colorFactory.getColor("dark_moderate_cyan");

	auto [r, g, b] = testColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


class foo: commonItems::parser
{
  public:
	explicit foo(std::istream& theStream)
	{
		registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
			color = commonItems::Color::Factory{}.getColor(theStream);
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


TEST(Color_Tests, ColorCanBeOutputInHexColorSpace)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testColor.outputHex();

	ASSERT_EQ("= hex { 408080 }", output.str());
}

TEST(Color_Tests, ColorCanBeOutputInHexColorSpacePreservingZeroes)
{
	const commonItems::Color testColor(std::array<int, 3>{0, 0, 0});

	std::stringstream output;
	output << testColor.outputHex();

	ASSERT_EQ("= hex { 000000 }", output.str());
}

TEST(Color_Tests, ColorCanBeOutputInHsvColorSpace)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testColor.outputHsv();

	ASSERT_EQ("= hsv { 0.5 0.5 0.5 }", output.str());
}


TEST(Color_Tests, ColorCanBeOutputInHsv360ColorSpace)
{
	const commonItems::Color testColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testColor.outputHsv360();

	ASSERT_EQ("= hsv360 { 180 50 50 }", output.str());
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

TEST(Color_Tests, ColorPaletteCanBeInitializedByMap)
{
	std::map<std::string, commonItems::Color> theMap;
	theMap.insert(std::pair("white", commonItems::Color(std::array<int, 3>{255, 255, 255})));
	theMap.insert(std::pair("gray", commonItems::Color(std::array<int, 3>{50, 50, 50})));
	
	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColorMap(theMap);

	std::stringstream input;
	input << "= white";
	const auto white = colorFactory.getColor(input);
	std::stringstream input2;
	input2 << "= gray";
	const auto gray = colorFactory.getColor(input2);

	auto [r, g, b] = white.getRgbComponents();
	ASSERT_EQ(255, r);
	ASSERT_EQ(255, g);
	ASSERT_EQ(255, b);

	auto [h, s, v] = white.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
	ASSERT_NEAR(0.0f, s, 0.01);
	ASSERT_NEAR(1.0f, v, 0.01);

	auto [r2, g2, b2] = gray.getRgbComponents();
	ASSERT_EQ(50, r2);
	ASSERT_EQ(50, g2);
	ASSERT_EQ(50, b2);

	auto [h2, s2, v2] = gray.getHsvComponents();
	ASSERT_NEAR(0.0f, h2, 0.01);
	ASSERT_NEAR(0.0f, s2, 0.01);
	ASSERT_NEAR(0.196f, v2, 0.01);
}

TEST(Color_Tests, ColorPaletteCanBeAlteredByStream)
{
	auto colorFactory = commonItems::Color::Factory();
	std::stringstream input;
	input << "= { 255 0 0 }";
	colorFactory.addNamedColor("gold", input);
	
	std::stringstream input2;
	input2 << "= hex { FFD700 }";
	colorFactory.addNamedColor("gold", input2);
	
	const auto gold = colorFactory.getColor("gold");

	auto [r, g, b] = gold.getRgbComponents();
	ASSERT_EQ(255, r);
	ASSERT_EQ(215, g);
	ASSERT_EQ(0, b);

	auto [h, s, v] = gold.getHsvComponents();
	ASSERT_NEAR(0.142f, h, 0.01);
	ASSERT_NEAR(1.0f, s, 0.01);
	ASSERT_NEAR(1.0f, v, 0.01);
}

TEST(Color_Tests, ColorPaletteCanBeAlteredDirectly)
{
	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColor("gold", commonItems::Color(std::array<int, 3>{255, 0, 0}));
	colorFactory.addNamedColor("gold", commonItems::Color(std::array<int, 3>{255, 215, 0}));

	const auto gray = colorFactory.getColor("gold");

	auto [r, g, b] = gray.getRgbComponents();
	ASSERT_EQ(255, r);
	ASSERT_EQ(215, g);
	ASSERT_EQ(0, b);

	auto [h, s, v] = gray.getHsvComponents();
	ASSERT_NEAR(0.142f, h, 0.01);
	ASSERT_NEAR(1.0f, s, 0.01);
	ASSERT_NEAR(1.0f, v, 0.01);
}

TEST(Color_Tests, ColorPaletteCanBeAlteredByMap)
{
	std::map<std::string, commonItems::Color> wrongMap;
	wrongMap.insert(std::pair("white", commonItems::Color(std::array<int, 3>{0, 0, 0})));
	wrongMap.insert(std::pair("red", commonItems::Color(std::array<int, 3>{255, 255, 19})));

	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColorMap(wrongMap);

	std::map<std::string, commonItems::Color> correctMap;
	correctMap.insert(std::pair("white", commonItems::Color(std::array<int, 3>{255, 255, 255})));
	correctMap.insert(std::pair("red", commonItems::Color(std::array<int, 3>{255, 0, 0})));
	colorFactory.addNamedColorMap(correctMap);

	std::stringstream input;
	input << "= white";
	const auto white = colorFactory.getColor(input);
	std::stringstream input2;
	input2 << "= red";
	const auto red = colorFactory.getColor(input2);

	auto [r, g, b] = white.getRgbComponents();
	ASSERT_EQ(255, r);
	ASSERT_EQ(255, g);
	ASSERT_EQ(255, b);

	auto [h, s, v] = white.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
	ASSERT_NEAR(0.0f, s, 0.01);
	ASSERT_NEAR(1.0f, v, 0.01);

	auto [r2, g2, b2] = red.getRgbComponents();
	ASSERT_EQ(255, r2);
	ASSERT_EQ(0, g2);
	ASSERT_EQ(0, b2);

	auto [h2, s2, v2] = red.getHsvComponents();
	ASSERT_NEAR(0.0f, h2, 0.01);
	ASSERT_NEAR(1.0f, s2, 0.01);
	ASSERT_NEAR(1.0f, v2, 0.01);
}

TEST(Color_Tests, ColorPaletteCanBeCleared)
{
	std::map<std::string, commonItems::Color> colorMap;
	colorMap.insert(std::pair("white", commonItems::Color(std::array<int, 3>{0, 0, 0})));
	colorMap.insert(std::pair("red", commonItems::Color(std::array<int, 3>{255, 255, 19})));
	
	auto colorFactory = commonItems::Color::Factory();
	colorFactory.addNamedColorMap(colorMap);

	ASSERT_EQ(2, colorFactory.getRegisteredColors().size());

	colorFactory.clear();

	ASSERT_TRUE(colorFactory.getRegisteredColors().empty());
}


// RandomlyFluctuate() isn't easily testable, so skipped