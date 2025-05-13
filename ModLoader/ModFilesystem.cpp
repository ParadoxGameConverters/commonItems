#include "ModFilesystem.h"

#include <algorithm>
#include <map>
#include <ranges>

#include "../OSCompatibilityLayer.h"



using std::filesystem::path;



namespace
{

std::vector<path> SplitPath(path path_to_split)
{
	std::vector<path> split_path;

	path reassembled_path;
	for (const path& component: path_to_split)
	{
		if (component.empty())
		{
			continue;
		}
		if (!reassembled_path.empty())
		{
			reassembled_path /= component;
		}
		else
		{
			reassembled_path = component;
		}
		reassembled_path.make_preferred();
		split_path.push_back(reassembled_path);
	}

	return split_path;
}


bool PathIsReplaced(path path_to_check, const std::set<path>& replaced_paths)
{
	const auto split_path = SplitPath(path_to_check);
	return std::ranges::any_of(replaced_paths, [&split_path](const path& replaced_path) {
		const auto split_replaced_path = SplitPath(replaced_path);
		for (size_t i = 0; i < split_replaced_path.size(); i++)
		{
			if (split_path[i] != split_replaced_path[i])
			{
				return false;
			}
		}

		return true;
	});
}

} // namespace


std::optional<path> commonItems::ModFilesystem::GetActualFileLocation(const path& path_to_file) const
{
	for (const auto& mod: mods_ | std::views::reverse)
	{
		path mod_path = mod.path / path_to_file;
		if (DoesFileExist(mod_path))
		{
			return mod_path;
		}
		if (PathIsReplaced(path_to_file, mod.replacedFolders))
		{
			return std::nullopt;
		}
	}

	// check game root
	if (path root_path = game_root_ / path_to_file; DoesFileExist(root_path))
	{
		return root_path;
	}

	return std::nullopt;
}


std::optional<path> commonItems::ModFilesystem::GetActualFolderLocation(const path& path_to_folder) const
{
	for (const auto& mod: mods_ | std::views::reverse)
	{
		path mod_path = mod.path / path_to_folder;
		if (DoesFolderExist(mod_path))
		{
			return mod_path;
		}
		if (PathIsReplaced(path_to_folder, mod.replacedFolders))
		{
			return std::nullopt;
		}
	}

	// check game root
	path root_path = game_root_ / path_to_folder;
	if (DoesFolderExist(root_path))
	{
		return root_path;
	}

	return std::nullopt;
}


std::vector<path> commonItems::ModFilesystem::GetAllFilesInFolder(const path& path_to_folder) const
{
	std::map<std::string, path> full_files;
	std::set<path> found_files;
	std::vector<path> to_return;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const path& new_file: commonItems::GetAllFilesInFolder(mod.path / path_to_folder))
		{
			if (found_files.contains(new_file))
			{
				continue;
			}

			found_files.insert(new_file);
			full_files.emplace(new_file.string(), mod.path / path_to_folder / new_file);
		}

		if (PathIsReplaced(path_to_folder, mod.replacedFolders))
		{
			for (const auto& path: full_files | std::views::values)
			{
				to_return.emplace_back(path);
			}
			return to_return;
		}
	}

	for (const path& new_file: commonItems::GetAllFilesInFolder(game_root_ / path_to_folder))
	{
		if (found_files.contains(new_file))
		{
			continue;
		}

		found_files.insert(new_file);
		full_files.emplace(new_file.string(), game_root_ / path_to_folder / new_file);
	}

	for (const auto& path: full_files | std::views::values)
	{
		to_return.emplace_back(path);
	}
	return to_return;
}


std::vector<path> commonItems::ModFilesystem::GetAllSubfolders(const path& path_to_folder) const
{
	std::map<std::string, path> full_folders;
	std::set<path> found_folders;
	std::vector<path> to_return;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const auto& new_folder: commonItems::GetAllSubfolders(mod.path / path_to_folder))
		{
			if (found_folders.contains(new_folder))
			{
				continue;
			}

			found_folders.insert(new_folder);
			full_folders.emplace(new_folder.string(), mod.path / path_to_folder / new_folder);
		}

		if (PathIsReplaced(path_to_folder, mod.replacedFolders))
		{
			for (const auto& path: full_folders | std::views::values)
			{
				to_return.emplace_back(path);
			}
			return to_return;
		}
	}

	for (const auto& new_folder: commonItems::GetAllSubfolders(game_root_ / path_to_folder))
	{
		if (found_folders.contains(new_folder))
		{
			continue;
		}

		found_folders.insert(new_folder);
		full_folders.emplace(new_folder.string(), game_root_ / path_to_folder / new_folder);
	}

	for (const auto& path: full_folders | std::views::values)
	{
		to_return.emplace_back(path);
	}
	return to_return;
}


std::vector<path> commonItems::ModFilesystem::GetAllFilesInFolderRecursive(const path& path_to_folder) const
{
	std::map<std::string, path> full_files;
	std::set<path> found_files;
	std::vector<path> to_return;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const path& new_file: commonItems::GetAllFilesInFolderRecursive(mod.path / path_to_folder))
		{
			if (found_files.contains(new_file))
			{
				continue;
			}

			found_files.insert(new_file);
			full_files.emplace(new_file.string(), (mod.path / path_to_folder / new_file).make_preferred());
		}

		if (PathIsReplaced(path_to_folder, mod.replacedFolders))
		{
			for (const auto& path: full_files | std::views::values)
			{
				to_return.emplace_back(path);
			}
			return to_return;
		}
	}

	for (const path& new_file: commonItems::GetAllFilesInFolderRecursive(game_root_ / path_to_folder))
	{
		if (found_files.contains(new_file))
		{
			continue;
		}

		found_files.insert(new_file);
		full_files.emplace(new_file.string(), (game_root_ / path_to_folder / new_file).make_preferred());
	}

	for (const auto& path: full_files | std::views::values)
	{
		to_return.emplace_back(path);
	}
	return to_return;
}