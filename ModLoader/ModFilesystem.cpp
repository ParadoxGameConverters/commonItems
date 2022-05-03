#include "ModFilesystem.h"

#include <algorithm>
#include <ranges>


#include "../OSCompatibilityLayer.h"



namespace
{

bool PathIsReplaced(std::string_view path, const std::set<std::string>& replaced_paths)
{
	return std::ranges::any_of(replaced_paths, [&path](const std::string& replaced_path) {
		if (path.starts_with(replaced_path))
		{
			return true;
		}
		if (!replaced_path.starts_with('/') && path.starts_with('/' + replaced_path))
		{
			return true;
		}
		return false;
	});
}

} // namespace


std::optional<std::string> commonItems::ModFilesystem::GetActualFileLocation(const std::string& path) const
{
	for (const auto& mod : mods_ | std::views::reverse)
	{
		if (DoesFileExist(mod.path + '/' + path))
		{
			return mod.path + '/' + path;
		}
		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return std::nullopt;
		}
	}

	// check game root
	if (DoesFileExist(game_root_ + '/' + path))
	{
		return game_root_ + '/' + path;
	}

	return std::nullopt;
}


std::optional<std::string> commonItems::ModFilesystem::GetActualFolderLocation(const std::string& path) const
{
	for (const auto& mod : mods_ | std::views::reverse)
	{
		if (DoesFolderExist(mod.path + '/' + path))
		{
			return mod.path + '/' + path;
		}
		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return std::nullopt;
		}
	}

	// check game root
	if (DoesFolderExist(game_root_ + '/' + path))
	{
		return game_root_ + '/' + path;
	}

	return std::nullopt;
}


std::set<std::string> commonItems::ModFilesystem::GetAllFilesInFolder(const std::string& path) const
{
	std::set<std::string> full_files;
	std::set<std::string> found_files;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const auto& new_file: commonItems::GetAllFilesInFolder(mod.path + '/' + std::string(path)))
		{
			if (found_files.contains(new_file))
			{
				continue;
			}

			found_files.insert(new_file);
			full_files.insert(mod.path + '/' + path + '/' + std::string(new_file));
		}

		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return full_files;
		}
	}

	for (const auto& new_file: commonItems::GetAllFilesInFolder(game_root_ + '/' + std::string(path)))
	{
		if (found_files.contains(new_file))
		{
			continue;
		}

		found_files.insert(new_file);
		full_files.insert(game_root_ + '/' + path + '/' + std::string(new_file));
	}

	return full_files;
}