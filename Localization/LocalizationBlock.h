#ifndef LOCALIZATION_LOCALIZATION_BLOCK_H
#define LOCALIZATION_LOCALIZATION_BLOCK_H



#include "LocalizationLambdas.h"
#include <map>
#include <optional>
#include <string>



namespace commonItems
{

class LocalizationBlock
{
  public:
	LocalizationBlock(std::string key, std::string base_language): key_(std::move(key)), base_language_(std::move(base_language)) {}
	LocalizationBlock(std::string key, const LocalizationBlock& other_block):
		 key_(std::move(key)), languages_to_localizations_(other_block.languages_to_localizations_), base_language_(other_block.base_language_)
	{
	}
	void CopyFrom(const LocalizationBlock& other_block);

	[[nodiscard]] const std::string& GetKey() const { return key_; }
	[[nodiscard]] const std::string& GetBaseLanguage() const { return base_language_; }
	[[nodiscard]] const std::map<std::string, std::string>& GetLocalizations() const { return languages_to_localizations_; }

	[[nodiscard]] std::string GetLocalization(const std::string& language) const;

	void ModifyLocalization(const std::string& language, const std::string& localization) { languages_to_localizations_[language] = localization; }

	// ModifyForEveryLanguage helps remove boilerplate by applying modifying_function to every language in the block
	// For example:
	//  name_block["english"] = name_block["english"].Replace("$ADJ$", base_adjective_block["english"]);
	//  name_block["french"] = name_block["french"].Replace("$ADJ$", base_adjective_block["french"]);
	//  name_block["german"] = name_block["german"].Replace("$ADJ$", base_adjective_block["german"]);
	//  name_block["russian"] = name_block["russian"].Replace("$ADJ$", base_adjective_block["russian"]);
	//  name_block["simp_chinese"] = name_block["simp_chinese"].Replace("$ADJ$", base_adjective_block["simp_chinese"]);
	//  name_block["spanish"] = name_block["spanish"].Replace("$ADJ$", base_adjective_block["spanish"]);
	//
	// Can be replaced by:
	//  name_block.ModifyForEveryLanguage(base_adjective_block, [this](const std::string& base_localization, const std::optional<std::string>&
	//  modifying_localization, const std::string& language){
	// 		return base_localization.Replace("$ADJ$", *modifying_localization);
	//  });
	void ModifyForEveryLanguage(const LocalizationLambda& modifying_function);
	void ModifyForEveryLanguage(const LocalizationBlock& other_block, const TwoArgLocalizationLambda& modifying_function);

  private:
	std::string key_;
	std::map<std::string, std::string> languages_to_localizations_;
	std::string base_language_;
};

} // namespace commonItems



#endif // LOCALIZATION_LOCALIZATION_BLOCK_H