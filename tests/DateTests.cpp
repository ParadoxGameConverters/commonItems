#include "../Date.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"



TEST(Date_Tests, DaysInMonthCorrectForJanuary)
{
	EXPECT_EQ(CommonItems::DaysInMonth(1), 31);
}


TEST(Date_Tests, DaysInMonthCorrectForFebruary)
{
	EXPECT_EQ(CommonItems::DaysInMonth(2), 28);
}


TEST(Date_Tests, DaysInMonthCorrectForMarch)
{
	EXPECT_EQ(CommonItems::DaysInMonth(3), 31);
}


TEST(Date_Tests, DaysInMonthCorrectForApril)
{
	EXPECT_EQ(CommonItems::DaysInMonth(4), 30);
}


TEST(Date_Tests, DaysInMonthCorrectForMay)
{
	EXPECT_EQ(CommonItems::DaysInMonth(5), 31);
}


TEST(Date_Tests, DaysInMonthCorrectForJune)
{
	EXPECT_EQ(CommonItems::DaysInMonth(6), 30);
}


TEST(Date_Tests, DaysInMonthCorrectForJuly)
{
	EXPECT_EQ(CommonItems::DaysInMonth(7), 31);
}


TEST(Date_Tests, DaysInMonthCorrectForAugust)
{
	EXPECT_EQ(CommonItems::DaysInMonth(8), 31);
}


TEST(Date_Tests, DaysInMonthCorrectForSeptember)
{
	EXPECT_EQ(CommonItems::DaysInMonth(9), 30);
}


TEST(Date_Tests, DaysInMonthCorrectForOctober)
{
	EXPECT_EQ(CommonItems::DaysInMonth(10), 31);
}


TEST(Date_Tests, DaysInMonthCorrectForNovember)
{
	EXPECT_EQ(CommonItems::DaysInMonth(11), 30);
}


TEST(Date_Tests, DaysInMonthCorrectForDecember)
{
	EXPECT_EQ(CommonItems::DaysInMonth(12), 31);
}

TEST(Date_Tests, DefaultDateIsNotSet)
{
	const date testDate;

	ASSERT_FALSE(testDate.isSet());
}


TEST(Date_Tests, DefaultDateEqualsOneJanuaryFirst)
{
	const date testDate;

	ASSERT_EQ("1.1.1", testDate.toString());
}


TEST(Date_Tests, DateCanBeInput)
{
	const date testDate(2020, 4, 25);

	ASSERT_EQ("2020.4.25", testDate.toString());
}


TEST(Date_Tests, DateCanBeInputFromString)
{
	const date testDate("2020.4.25");

	ASSERT_EQ("2020.4.25", testDate.toString());
}


TEST(Date_Tests, DateInitializedFromEmptyStringIsDefault)
{
	const date testDate("");

	ASSERT_EQ("1.1.1", testDate.toString());
}


TEST(Date_Tests, DateLogsBadInitialization)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const date testDate("2020.4");

	std::cout.rdbuf(stdOutBuf);

	EXPECT_THAT(log.str(), testing::HasSubstr("[WARNING] Problem inputting date:"));
}


TEST(Date_Tests, DateIsNotSetOnBadInitialization)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const date testDate("2020.4");

	std::cout.rdbuf(stdOutBuf);

	ASSERT_FALSE(testDate.isSet());
}


TEST(Date_Tests, DateIsOneJanuaryFirstOnBadInitialization)
{
	const std::stringstream log;
	auto* const stdOutBuf = std::cout.rdbuf();
	std::cout.rdbuf(log.rdbuf());

	const date testDate("2020.4");

	std::cout.rdbuf(stdOutBuf);

	ASSERT_EQ("1.1.1", testDate.toString());
}


TEST(Date_Tests, DaysCanBeIncreased)
{
	date testDate(2020, 1, 25);
	testDate.ChangeByDays(4);

	ASSERT_EQ("2020.1.29", testDate.toString());
}


TEST(Date_Tests, DaysCanBeIncreasedAndWrapsMonths)
{
	date testDate(2020, 4, 25);
	testDate.ChangeByDays(14);

	ASSERT_EQ("2020.5.9", testDate.toString());
}


TEST(Date_Tests, DaysCanBeIncreasedAndWrapsYears)
{
	date testDate(2020, 4, 25);
	testDate.ChangeByDays(314);

	ASSERT_EQ("2021.3.5", testDate.toString());
}


TEST(Date_Tests, DaysCanBeDecreased)
{
	date testDate(2020, 4, 25);
	testDate.ChangeByDays(-4);

	ASSERT_EQ("2020.4.21", testDate.toString());
}


TEST(Date_Tests, DaysCanBeDecreasedAndWrapsMonths)
{
	date testDate(2020, 2, 25);
	testDate.ChangeByDays(-34);

	ASSERT_EQ("2020.1.22", testDate.toString());
}


TEST(Date_Tests, DaysCanBeDecreasedAndWrapsYears)
{
	date testDate(2020, 4, 25);
	testDate.ChangeByDays(-679);

	ASSERT_EQ("2018.6.15", testDate.toString());
}


TEST(Date_Tests, MonthsCanBeIncreased)
{
	date testDate(2020, 4, 25);
	testDate.increaseByMonths(4);

	ASSERT_EQ("2020.8.25", testDate.toString());
}


TEST(Date_Tests, MonthsCanBeIncreasedAndWrapAround)
{
	date testDate(2020, 4, 25);
	testDate.increaseByMonths(9);

	ASSERT_EQ("2021.1.25", testDate.toString());
}


TEST(Date_Tests, MonthsCanBeDecreased)
{
	date testDate(2020, 8, 25);
	testDate.ChangeByMonths(-4);

	ASSERT_EQ("2020.4.25", testDate.toString());
}


TEST(Date_Tests, MonthsCanBeDecreasedAndWrapAround)
{
	date testDate(2020, 4, 25);
	testDate.ChangeByMonths(-9);

	ASSERT_EQ("2019.7.25", testDate.toString());
}


TEST(Date_Tests, YearsCanBeIncreased)
{
	date testDate(2020, 4, 25);
	testDate.addYears(4);

	ASSERT_EQ("2024.4.25", testDate.toString());
}

TEST(Date_Tests, YearsCanBeDecreased)
{
	date testDate(2020, 4, 25);
	testDate.subtractYears(4);

	ASSERT_EQ("2016.4.25", testDate.toString());
}


TEST(Date_Tests, DatesCanBeEqual)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2020, 4, 25);

	ASSERT_EQ(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeUnequalFromDay)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2020, 4, 24);

	ASSERT_NE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeUnequalFromMonth)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2020, 3, 25);

	ASSERT_NE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeUnequalFromYear)
{
	const date testDate(2019, 4, 25);
	const date testDateTwo(2020, 4, 25);

	ASSERT_NE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeLessThanFromDay)
{
	const date testDate(2020, 4, 24);
	const date testDateTwo(2020, 4, 25);

	ASSERT_LT(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeLessThanFromMonth)
{
	const date testDate(2020, 3, 26);
	const date testDateTwo(2020, 4, 25);

	ASSERT_LT(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeLessThanFromYear)
{
	const date testDate(2019, 5, 26);
	const date testDateTwo(2020, 4, 25);

	ASSERT_LT(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeGreaterThanFromDay)
{
	const date testDate(2020, 4, 26);
	const date testDateTwo(2020, 4, 25);

	ASSERT_GT(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeGreaterThanFromMonth)
{
	const date testDate(2020, 5, 24);
	const date testDateTwo(2020, 4, 25);

	ASSERT_GT(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeGreaterThanFromYear)
{
	const date testDate(2021, 3, 26);
	const date testDateTwo(2020, 4, 25);

	ASSERT_GT(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeLessThanOrEqualsFromLessThan)
{
	const date testDate(2020, 4, 24);
	const date testDateTwo(2020, 4, 25);

	ASSERT_LE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeLessThanOrEqualsFromEqualsThan)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2020, 4, 25);

	ASSERT_LE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeGreaterThanOrEqualsFromGreaterThan)
{
	const date testDate(2020, 4, 26);
	const date testDateTwo(2020, 4, 25);

	ASSERT_GE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeGreaterThanOrEqualsFromEquals)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2020, 4, 25);

	ASSERT_GE(testDate, testDateTwo);
}


TEST(Date_Tests, DatesCanBeOutput)
{
	const date testDate(2020, 4, 25);

	std::stringstream output;
	output << testDate;

	ASSERT_EQ("2020.4.25", output.str());
}


TEST(Date_Tests, DiffInYearsHandlesExactYears)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2019, 4, 25);

	ASSERT_EQ(1.0f, testDate.diffInYears(testDateTwo));
}


TEST(Date_Tests, DiffInYearsHandlesPartialYears)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2020, 1, 25);

	ASSERT_NEAR(0.246575f, testDate.diffInYears(testDateTwo), 0.0001);
}


TEST(Date_Tests, DiffInYearsHandlesWrapAround)
{
	const date testDate(2020, 4, 25);
	const date testDateTwo(2019, 8, 25);

	ASSERT_NEAR(0.665753f, testDate.diffInYears(testDateTwo), 0.0001);
}


TEST(Date_Tests, DiffInYearsHandlesNegative)
{
	const date testDate(2020, 1, 25);
	const date testDateTwo(2020, 4, 25);

	ASSERT_NEAR(-0.246575f, testDate.diffInYears(testDateTwo), 0.0001);
}


TEST(Date_Tests, AUCcanBeConvertedToAD)
{
	const date testDate(450, 10, 1, true);
	const date testDate2(1306, 3, 1, true);
	const date testDate3("450.10.1", true);
	const date testDate4("1306.3.1", true);

	ASSERT_EQ("-304.10.1", testDate.toString());
	ASSERT_EQ("553.3.1", testDate2.toString());
	ASSERT_EQ("-304.10.1", testDate3.toString());
	ASSERT_EQ("553.3.1", testDate4.toString());
}

TEST(Date_Tests, separateComponentsCanBeGotten)
{
	const date testDate("450.10.7");

	ASSERT_EQ(450, testDate.getYear());
	ASSERT_EQ(10, testDate.getMonth());
	ASSERT_EQ(7, testDate.getDay());
}
TEST(Date_Tests, negativeYearComponentsCanBeGotten)
{
	const date testDate("-450.10.7");

	ASSERT_EQ(-450, testDate.getYear());
	ASSERT_EQ(10, testDate.getMonth());
	ASSERT_EQ(7, testDate.getDay());
}