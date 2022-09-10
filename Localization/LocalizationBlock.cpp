#include "LocalizationBlock.h"
#include <ranges>



std::optional<std::string> commonItems::LocalizationBlock::GetLocalization(const std::string& language) const
{
	if (const auto itr = languages_to_localizations_.find(language); itr != languages_to_localizations_.end())
	{
		return itr->second;
	}
	return std::nullopt;
}


void commonItems::LocalizationBlock::CopyFrom(const LocalizationBlock& other_block)
{
	for (const auto& [language, localization] : other_block.GetLocalizations())
	{
		languages_to_localizations_[language] = localization;
	}
}


void commonItems::LocalizationBlock::ModifyForEveryLanguage(const LocalizationBlock& other_block, const LocalizationLambda& modifying_function)
{
	for (auto& [language, localization]: languages_to_localizations_)
	{
		localization = modifying_function(localization, language);
	}
}


void commonItems::LocalizationBlock::ModifyForEveryLanguage(const LocalizationBlock& other_block, const TwoArgLocalizationLambda& modifying_function)
{
	for (auto& [language, localization]: languages_to_localizations_)
	{
		localization = modifying_function(localization, other_block.GetLocalization(language), language);
	}
}