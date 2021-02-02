#include "../ConverterVersion.h"
#include "gtest/gtest.h"



TEST(ConverterVersion_Tests, ItemsDefaultToEmpty)
{
	const commonItems::ConverterVersion converterVersion;

	ASSERT_TRUE(converterVersion.name.empty());
	ASSERT_TRUE(converterVersion.version.empty());
	ASSERT_TRUE(converterVersion.descriptionLine.empty());
}


TEST(ConverterVersion_Tests, ItemsCanBeImported)
{
	const auto converterVersion = commonItems::ConverterVersionParser().importVersion();

	ASSERT_EQ(converterVersion.name, "test-name");
	ASSERT_EQ(converterVersion.version, "test-version");
	ASSERT_EQ(converterVersion.descriptionLine, "test description");
}