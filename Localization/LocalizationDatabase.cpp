#include "LocalizationDatabase.h"
#include "../CommonFunctions.h"
#include "../Log.h"
#include "../Parser.h"
#include <fstream>



void commonItems::LocalizationDatabase::ScrapeLocalizations(const ModFilesystem& mod_filesystem, const std::string& localization_folder)
{
	Log(LogLevel::Info) << "Reading Localization...";

	const auto localization_files = mod_filesystem.GetAllFilesInFolderRecursive(localization_folder);
	int lines_count = 0;
	for (const auto& file: localization_files)
	{
		if (getExtension(file) == "yml")
		{
			lines_count += ScrapeFile(file);
		}
	}

	Log(LogLevel::Info) << lines_count << " localization lines read.";
}


int commonItems::LocalizationDatabase::ScrapeStream(std::istream& stream)
{
	absorbBOM(stream);
	int lines_read = 0;
	std::string current_language;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		const auto [key, localization] = DetermineKeyLocalizationPair(line, current_language);
		if (current_language.empty())
		{
			continue;
		}
		if (key.empty() || localization.empty())
		{
			continue;
		}

		if (auto itr = localizations_.find(key); itr != localizations_.end())
		{
			if (!itr->second.GetLocalizations().contains(current_language))
				itr->second.ModifyLocalization(current_language, localization);
		}
		else
		{
			LocalizationBlock new_block(key, base_language_);
			new_block.ModifyLocalization(current_language, localization);
			localizations_.emplace(key, new_block);
		}
		++lines_read;
	}

	return lines_read;
}


std::optional<commonItems::LocalizationBlock> commonItems::LocalizationDatabase::GetLocalizationBlock(const std::string& key) const
{
	if (const auto itr = localizations_.find(key); itr != localizations_.end())
	{
		return itr->second;
	}
	return std::nullopt;
}


void commonItems::LocalizationDatabase::AddOrModifyLocalizationBlock(const std::string& key, const LocalizationBlock& new_block)
{
	{
		if (auto [itr, success] = localizations_.emplace(key, new_block); !success)
		{
			itr->second = new_block;
		}
	}
}


std::pair<std::string, std::string> commonItems::LocalizationDatabase::DetermineKeyLocalizationPair(const std::string& line,
	 std::string& current_language) const
{
	if (line.length() < 4)
	{
		return {};
	}
	const auto isspace = [](char x) {return std::isspace(x);});
	const auto first_non_space = std::ranges::find_if_not(line, isspace);
    
    if (first_non_space == line.end() || *first_non_space == '#')
	{
		return {};
	}

	const auto last_non_space = std::ranges::find_if_not(line.rbegin(), line.rend(), isspace);
	const std::string_view stripped_line(first_non_space, last_non_space.base());
	const auto separator_location = stripped_line.find_first_of(':');
	if (separator_location == std::string::npos)
	{
		return {};
	}

	if (stripped_line.starts_with("l_"))
	{
		if (stripped_line == "l_" + base_language_ + ":")
		{
			current_language = base_language_;
			return {};
		}
		for (const auto& language: other_languages_)
		{
			if (stripped_line != "l_" + language + ":")
			{
				continue;
			}
			current_language = language;
			return {};
		}

		return {};
	}

	if (current_language.empty())
	{
		Log(LogLevel::Warning) << "Scraping loc line [" << line << "] without language specified!";
		return {};
	}

	const auto key = stripped_line.substr(0, separator_location);
	const auto new_line = stripped_line.substr(separator_location + 1);
	const auto quote_index = new_line.find_first_of('\"');
	const auto quote_two_index = new_line.find_last_of('\"');
	if (quote_index == std::string::npos || quote_two_index == std::string::npos || quote_two_index - quote_index == 0)
	{
		return {std::string(key), ""};
	}

	const auto value = new_line.substr(quote_index + 1, quote_two_index - quote_index - 1);
	return {std::string(key, std::string(value)};
}


int commonItems::LocalizationDatabase::ScrapeFile(const std::string& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		return 0;
	}

	return ScrapeStream(file);
}