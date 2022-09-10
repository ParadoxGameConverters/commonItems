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

	void ScrapeLocalizations(const ModFilesystem& mod_filesystem);
	[[nodiscard]] int ScrapeStream(std::istream& stream);
	void AddLocalizationBlock(const std::string& key, const LocalizationBlock& new_block);

	[[nodiscard]] std::optional<LocalizationBlock> GetLocalizationBlock(const std::string& key) const;

  private:
	std::pair<std::string, std::string> DetermineKeyLocalizationPair(const std::string& line, std::string& current_language) const;
	int ScrapeFile(const std::string& file_path);

	std::string base_language_;
	std::vector<std::string> other_languages_;
	std::map<std::string, LocalizationBlock> localizations_;
};

} // namespace commonItems



#endif // LOCALIZATION_LOCALIZATION_DATABASE_H