#include "../ConvenientParser.h"
#include <gtest/gtest.h>


TEST(ConvenientParser_Tests, StringSetterWorks)
{
	std::stringstream input{"key = string"};
	class Test: commonItems::convenientParser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerSetter("key", member);
			parseStream(stream);
		}
		std::string member;
	};
	const auto test = Test(input);

	ASSERT_EQ("string", test.member);
}

TEST(ConvenientParser_Tests, IntSetterWorks)
{
	std::stringstream input{"key = 34"};
	class Test: commonItems::convenientParser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerSetter("key", member);
			parseStream(stream);
		}
		int member;
	};
	const auto test = Test(input);

	ASSERT_EQ(34, test.member);
}

TEST(ConvenientParser_Tests, DoubleSetterWorks)
{
	std::stringstream input{"key = 3.4"};
	class Test: commonItems::convenientParser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerSetter("key", member);
			parseStream(stream);
		}
		double member;
	};
	const auto test = Test(input);

	ASSERT_NEAR(3.4, test.member, 0.001);
}

TEST(ConvenientParser_Tests, LongLongSetterWorks)
{
	std::stringstream input{"key = -123456789012345"};
	class Test: commonItems::convenientParser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerSetter("key", member);
			parseStream(stream);
		}
		long long int member;
	};
	const auto test = Test(input);

	ASSERT_EQ(-123456789012345, test.member);
}

TEST(ConvenientParser_Tests, UnsignedLongLongSetterWorks)
{
	std::stringstream input{"key = 123456789012345"};
	class Test: commonItems::convenientParser
	{
	  public:
		explicit Test(std::istream& stream)
		{
			registerSetter("key", member);
			parseStream(stream);
		}
		unsigned long long int member;
	};
	const auto test = Test(input);

	ASSERT_EQ(123456789012345, test.member);
}