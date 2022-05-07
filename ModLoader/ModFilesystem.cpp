#include "ModFilesystem.h"

#include <algorithm>
#include <ranges>


#include "../OSCompatibilityLayer.h"



namespace
{

std::vector<std::string> SplitPath(std::string path)
{
	std::vector<std::string> split_path;

	auto separator = std::min(path.find_first_of('/'), path.find_first_of('\\'));
	while (separator != std::string::npos)
	{
		split_path.push_back(path.substr(0, separator));
		path = path.substr(separator + 1, path.length());
		separator = std::min(path.find_first_of('/'), path.find_first_of('\\'));
	}
	if (!path.empty())
	{
		split_path.push_back(path);
	}

	return split_path;
}


bool PathIsReplaced(std::string_view path, const std::set<std::string>& replaced_paths)
{
	const auto split_path = SplitPath(std::string(path));

	return std::ranges::any_of(replaced_paths, [&split_path](const std::string& replaced_path) {
		const auto split_replaced_path = SplitPath(replaced_path);
		for (size_t i = 0; i < split_replaced_path.size(); i++)
		{
			if (i >= split_path.size())
			{
				return false;
			}
			if (split_path[i] != split_replaced_path[i])
			{
				return false;
			}
		}

		return true;
	});
}

} // namespace


std::optional<std::string> commonItems::ModFilesystem::GetActualFileLocation(const std::string& path) const
{
	for (const auto& mod: mods_ | std::views::reverse)
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
	for (const auto& mod: mods_ | std::views::reverse)
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


std::set<std::string> commonItems::ModFilesystem::GetAllSubfolders(const std::string& path) const
{
	std::set<std::string> full_folders;
	std::set<std::string> found_folders;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const auto& new_folder: commonItems::GetAllSubfolders(mod.path + '/' + std::string(path)))
		{
			if (found_folders.contains(new_folder))
			{
				continue;
			}

			found_folders.insert(new_folder);
			full_folders.insert(mod.path + '/' + path + '/' + std::string(new_folder));
		}

		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return full_folders;
		}
	}

	for (const auto& new_folder: commonItems::GetAllSubfolders(game_root_ + '/' + std::string(path)))
	{
		if (found_folders.contains(new_folder))
		{
			continue;
		}

		found_folders.insert(new_folder);
		full_folders.insert(game_root_ + '/' + path + '/' + std::string(new_folder));
	}

	return full_folders;
}


std::set<std::string> commonItems::ModFilesystem::GetAllFilesInFolderRecursive(const std::string& path) const
{
	std::set<std::string> full_files;
	std::set<std::string> found_files;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const auto& new_file: commonItems::GetAllFilesInFolderRecursive(mod.path + '/' + std::string(path)))
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

	for (const auto& new_file: commonItems::GetAllFilesInFolderRecursive(game_root_ + '/' + std::string(path)))
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