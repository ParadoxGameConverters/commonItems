#include "../newColor.h"
#include "gtest/gtest.h"



TEST(newColor_Tests, newColorDefaultsToBlack)
{
	const commonItems::newColor testnewColor;

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(0, r);
	ASSERT_EQ(0, g);
	ASSERT_EQ(0, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
	ASSERT_NEAR(0.0f, s, 0.01);
	ASSERT_NEAR(0.0f, v, 0.01);
}


TEST(newColor_Tests, newColorCanBeInitializedWithRgbComponents)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{64, 128, 128});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, newColorCanBeInitializedWithHsvComponents)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.5f, 0.5f, 0.5f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, HsvConversion_GreyHasZeroHue)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 128, 128});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_RedHasHueOfZero)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 0, 0});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.0f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_YellowHasHueOfOneSixth)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 128, 64});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.167f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_GreenHasHueOfOneThird)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{0, 128, 0});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.333f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_CyanHasHueOfOneHalf)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{64, 128, 128});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_BlueHasHueOfTwoThirds)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{0, 0, 128});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.667f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_MagentaHasHueOfFiveSixths)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 64, 128});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.833f, h, 0.01);
}


TEST(newColor_Tests, HsvConversion_BlackHasZeroSaturation)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{0, 0, 0});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.0f, s, 0.01);
}


TEST(newColor_Tests, HsvConversion_GreyHasZeroSaturation)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 128, 128});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.0f, s, 0.01);
}


TEST(newColor_Tests, HsvConversion_newColorHasSaturation)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 128, 64});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, s, 0.01);
}


TEST(newColor_Tests, HsvConversion_BlackHasZeroValue)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{0, 0, 0});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.0f, v, 0.01);
}


TEST(newColor_Tests, HsvConversion_newColorHasValue)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{128, 64, 64});

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, RgbConversion_ZeroHueGivesRed)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.0f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(newColor_Tests, RgbConversion_OneSixthHueGivesYellow)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.167f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(254, r, 1);
	ASSERT_NEAR(254, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(newColor_Tests, RgbConversion_OneThirdHueGivesGreen)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.333f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(255, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(newColor_Tests, RgbConversion_OneHalfHueGivesCyan)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.5f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(255, g, 1);
	ASSERT_NEAR(255, b, 1);
}


TEST(newColor_Tests, RgbConversion_TwoThirdHueGivesBlue)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.667f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(255, b, 1);
}


TEST(newColor_Tests, RgbConversion_FiveSixthsHueGivesMagenta)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.833f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(254, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(254, b, 1);
}


TEST(newColor_Tests, RgbConversion_ZeroSaturationIsGreyscale)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.0f, 0.0f, 0.5f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(127, r, 1);
	ASSERT_NEAR(127, g, 1);
	ASSERT_NEAR(127, b, 1);
}


TEST(newColor_Tests, RgbConversion_FullSaturationisPurenewColor)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.0f, 1.0f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(newColor_Tests, RgbConversion_SaturationWhitensnewColor)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.0f, 0.5f, 1.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(255, r, 1);
	ASSERT_NEAR(127, g, 1);
	ASSERT_NEAR(127, b, 1);
}


TEST(newColor_Tests, RgbConversion_ZeroValueIsBlack)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.0f, 1.0f, 0.0f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(0, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(newColor_Tests, RgbConversion_ValueDarkensnewColor)
{
	const commonItems::newColor testnewColor(std::array<float, 3>{0.0f, 1.0f, 0.5f});

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_NEAR(127, r, 1);
	ASSERT_NEAR(0, g, 1);
	ASSERT_NEAR(0, b, 1);
}


TEST(newColor_Tests, newColorCanBeInitializedFromStream)
{
	std::stringstream input;
	input << "= { 64 128 128 }";
	const auto testnewColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, newColorCanBeInitializedFromStreamWithQuotes)
{
	std::stringstream input;
	input << R"(= { "64" "128" "128" })";
	const auto testnewColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, newColorInitializationRequiresThreeComponentsWhenUnspecified)
{
	std::stringstream input;
	input << "= { 64 128 }";

	ASSERT_THROW(commonItems::newColor::Factory{}.getColor(input), std::runtime_error);
}


TEST(newColor_Tests, newColorCanBeInitializedFromStreamInRgb)
{
	std::stringstream input;
	input << "= rgb { 64 128 128 }";
	const auto testnewColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, newColorInitializationRequiresThreeComponentsWhenRgb)
{
	std::stringstream input;
	input << "= rgb { 64 128 }";

	ASSERT_THROW(commonItems::newColor::Factory{}.getColor(input), std::runtime_error);
}


TEST(newColor_Tests, newColorCanBeInitializedFromStreamInHsv)
{
	std::stringstream input;
	input << "= hsv { 0.5 0.5 0.5 }";
	const auto testnewColor = commonItems::newColor::Factory::getColor(input);

	auto [r, g, b] = testnewColor.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = testnewColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);
}


TEST(newColor_Tests, newColorInitializationRequiresThreeComponentsWhenHsv)
{
	std::stringstream input;
	input << "= hsv { 0.333 0.5 }";

	ASSERT_THROW(commonItems::newColor::Factory{}.getColor(input), std::runtime_error);
}


class bax: commonItems::parser
{
  public:
	explicit bax(std::istream& theStream)
	{
		registerKeyword("color", [this](const std::string& unused, std::istream& theStream) {
			newColor = commonItems::newColor::Factory::getColor(theStream);
		});
		parseStream(theStream);
	}

	commonItems::newColor newColor;
};

TEST(newColor_Tests, newColorCanBeInitializedFromLongerStream)
{
	std::stringstream input;
	input << "= { color = { 64 128 128 } } more text";
	const bax quz(input);

	auto [r, g, b] = quz.newColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = quz.newColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" more text", std::string{buffer});
}


TEST(newColor_Tests, newColorCanBeInitializedInRgbFromLongerStream)
{
	std::stringstream input;
	input << "= { color = rgb { 64 128 128 } } more text";
	const bax quz(input);

	auto [r, g, b] = quz.newColor.getRgbComponents();
	ASSERT_EQ(64, r);
	ASSERT_EQ(128, g);
	ASSERT_EQ(128, b);

	auto [h, s, v] = quz.newColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" more text", std::string{buffer});
}


TEST(newColor_Tests, newColorCanBeInitializedInHsvFromLongerStream)
{
	std::stringstream input;
	input << "= { color = hsv { 0.5 0.5 0.5 } } more text";
	const bax quz(input);

	auto [r, g, b] = quz.newColor.getRgbComponents();
	ASSERT_EQ(63, r);
	ASSERT_EQ(127, g);
	ASSERT_EQ(127, b);

	auto [h, s, v] = quz.newColor.getHsvComponents();
	ASSERT_NEAR(0.5f, h, 0.01);
	ASSERT_NEAR(0.5f, s, 0.01);
	ASSERT_NEAR(0.5f, v, 0.01);

	char buffer[256];
	input.getline(buffer, sizeof buffer);
	ASSERT_EQ(" more text", std::string{buffer});
}


TEST(newColor_Tests, newColorCanBeOutputInUnspecifiednewColorSpace)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testnewColor;

	ASSERT_EQ("= { 64 128 128 }", output.str());
}


TEST(newColor_Tests, newColorCanBeOutputInRgbnewColorSpace)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testnewColor.outputRgb();

	ASSERT_EQ("= rgb { 64 128 128 }", output.str());
}


TEST(newColor_Tests, newColorCanBeOutputInHsvnewColorSpace)
{
	const commonItems::newColor testnewColor(std::array<int, 3>{64, 128, 128});

	std::stringstream output;
	output << testnewColor.outputHsv();

	ASSERT_EQ("= hsv { 0.5 0.5 0.5 }", output.str());
}


TEST(newColor_Tests, UnequalFromDifferentRgb)
{
	const commonItems::newColor newColorOne(std::array<int, 3>{2, 4, 8});
	const commonItems::newColor newColorTwo(std::array<int, 3>{3, 4, 8});

	ASSERT_NE(newColorOne, newColorTwo);
}


TEST(newColor_Tests, UnequalFromDifferentHsv)
{
	const commonItems::newColor newColorOne(std::array<float, 3>{0.333f, 0.50f, 0.50f});
	const commonItems::newColor newColorTwo(std::array<float, 3>{0.333f, 0.75f, 0.75f});

	ASSERT_NE(newColorOne, newColorTwo);
}


TEST(newColor_Tests, Equality)
{
	const commonItems::newColor newColorOne(std::array<int, 3>{2, 4, 8});
	const commonItems::newColor newColorTwo(std::array<int, 3>{2, 4, 8});

	ASSERT_EQ(newColorOne, newColorTwo);
}


// RandomlyFluctuate() isn't easily testable, so skipped