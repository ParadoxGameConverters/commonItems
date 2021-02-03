#include "../ConverterVersion.h"
#include "gtest/gtest.h"



TEST(ConverterVersion_Tests, ItemsDefaultToEmpty)
{
	const auto converterVersion = commonItems::ConverterVersionParser().importVersion("emptyVersion.txt");

	ASSERT_TRUE(converterVersion.name.empty());
	ASSERT_TRUE(converterVersion.version.empty());
	ASSERT_TRUE(converterVersion.descriptionLine.empty());
}


TEST(ConverterVersion_Tests, ItemsCanBeImported)
{
	const auto converterVersion = commonItems::ConverterVersionParser().importVersion("version.txt");

	ASSERT_EQ("test-name", converterVersion.name);
	ASSERT_EQ("test-version", converterVersion.version);
	ASSERT_EQ("test description", converterVersion.descriptionLine);
}