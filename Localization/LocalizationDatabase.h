#ifndef LOCALIZATION_LOCALIZATION_DATABASE_H
#define LOCALIZATION_LOCALIZATION_DATABASE_H



#include "../Log.h"
#include "../ModLoader/ModFilesystem.h"
#include "LocalizationBlock.h"
#include <fstream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>



namespace commonItems
{

class LocalizationDatabase
{
  public:
	LocalizationDatabase(std::string base_language, std::vector<std::string> other_languages):
		 base_language_(std::move(base_language)), other_languages_(std::move(other_languages))
	{
	}

	void ScrapeLocalizations(const ModFilesystem& mod_filesystem, const std::string& localization_folder);
	[[nodiscard]] int ScrapeStream(std::istream& stream);
	void AddOrModifyLocalizationBlock(const std::string& key, const LocalizationBlock& new_block);

	void ClearLocalizations() { localizations_.clear(); }

	[[nodiscard]] std::optional<LocalizationBlock> GetLocalizationBlock(const std::string& key) const;
	[[nodiscard]] auto size() const { return localizations_.size(); }
	[[nodiscard]] bool HasLocalization(const std::string& key) const { return localizations_.contains(key); }

	using iterator = std::map<std::string, LocalizationBlock>::iterator;
	using const_iterator = std::map<std::string, LocalizationBlock>::const_iterator;
	[[nodiscard]] iterator begin() { return localizations_.begin(); }
	[[nodiscard]] const_iterator begin() const { return localizations_.begin(); }
	[[nodiscard]] const_iterator cbegin() const { return localizations_.cbegin(); }
	[[nodiscard]] iterator end() { return localizations_.end(); }
	[[nodiscard]] const_iterator end() const { return localizations_.end(); }
	[[nodiscard]] const_iterator cend() const { return localizations_.cend(); }
	using reverse_iterator = std::map<std::string, LocalizationBlock>::reverse_iterator;
	using const_reverse_iterator = std::map<std::string, LocalizationBlock>::const_reverse_iterator;
	[[nodiscard]] reverse_iterator rbegin() { return localizations_.rbegin(); }
	[[nodiscard]] const_reverse_iterator rbegin() const { return localizations_.rbegin(); }
	[[nodiscard]] const_reverse_iterator crbegin() const { return localizations_.crbegin(); }
	[[nodiscard]] reverse_iterator rend() { return localizations_.rend(); }
	[[nodiscard]] const_reverse_iterator rend() const { return localizations_.rend(); }
	[[nodiscard]] const_reverse_iterator crend() const { return localizations_.crend(); }

  private:
	std::pair<std::string, std::string> DetermineKeyLocalizationPair(const std::string& line, std::string& current_language) const;
	int ScrapeFile(const std::string& file_path);

	std::string base_language_;
	std::vector<std::string> other_languages_;
	std::map<std::string, LocalizationBlock> localizations_;
};

} // namespace commonItems



#endif // LOCALIZATION_LOCALIZATION_DATABASE_H
