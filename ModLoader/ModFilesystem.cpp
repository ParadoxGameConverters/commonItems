#include "ModFilesystem.h"

#include <algorithm>
#include <ranges>


#include "../OSCompatibilityLayer.h"



namespace
{

std::vector<std::filesystem::path> SplitPath(std::filesystem::path path)
{
	std::vector<std::filesystem::path> split_path;

	std::filesystem::path reassembled_path;
	for (const std::filesystem::path& component: path)
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


bool PathIsReplaced(std::filesystem::path path, const std::set<std::filesystem::path>& replaced_paths)
{
	const auto split_path = SplitPath(path);
	return std::ranges::any_of(replaced_paths, [&split_path](const std::filesystem::path& replaced_path) {
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


std::optional<std::filesystem::path> commonItems::ModFilesystem::GetActualFileLocation(const std::filesystem::path& path) const
{
	for (const auto& mod: mods_ | std::views::reverse)
	{
		std::filesystem::path mod_path = mod.path / path;
		if (DoesFileExist(mod_path))
		{
			return mod_path;
		}
		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return std::nullopt;
		}
	}

	// check game root
	if (std::filesystem::path root_path = game_root_ / path; DoesFileExist(root_path))
	{
		return root_path;
	}

	return std::nullopt;
}


std::optional<std::filesystem::path> commonItems::ModFilesystem::GetActualFolderLocation(const std::filesystem::path& path) const
{
	for (const auto& mod: mods_ | std::views::reverse)
	{
		std::filesystem::path mod_path = mod.path / path;
		if (DoesFolderExist(mod_path))
		{
			return mod_path;
		}
		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return std::nullopt;
		}
	}

	// check game root
	std::filesystem::path root_path = game_root_ / path;
	if (DoesFolderExist(root_path))
	{
		return root_path;
	}

	return std::nullopt;
}


std::set<std::filesystem::path> commonItems::ModFilesystem::GetAllFilesInFolder(const std::filesystem::path& path) const
{
	std::set<std::filesystem::path> full_files;
	std::set<std::filesystem::path> found_files;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const std::filesystem::path& new_file: commonItems::GetAllFilesInFolder(mod.path / path))
		{
			if (found_files.contains(new_file))
			{
				continue;
			}

			found_files.insert(new_file);
			full_files.insert(mod.path / path / new_file);
		}

		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return full_files;
		}
	}

	for (const std::filesystem::path& new_file: commonItems::GetAllFilesInFolder(game_root_ / path))
	{
		if (found_files.contains(new_file))
		{
			continue;
		}

		found_files.insert(new_file);
		full_files.insert(game_root_ / path / new_file);
	}

	return full_files;
}


std::set<std::filesystem::path> commonItems::ModFilesystem::GetAllSubfolders(const std::filesystem::path& path) const
{
	std::set<std::filesystem::path> full_folders;
	std::set<std::filesystem::path> found_folders;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const auto& new_folder: commonItems::GetAllSubfolders(mod.path / path))
		{
			if (found_folders.contains(new_folder))
			{
				continue;
			}

			found_folders.insert(new_folder);
			full_folders.insert(mod.path / path / new_folder);
		}

		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return full_folders;
		}
	}

	for (const auto& new_folder: commonItems::GetAllSubfolders(game_root_ / path))
	{
		if (found_folders.contains(new_folder))
		{
			continue;
		}

		found_folders.insert(new_folder);
		full_folders.insert(game_root_ / path / new_folder);
	}

	return full_folders;
}


std::set<std::filesystem::path> commonItems::ModFilesystem::GetAllFilesInFolderRecursive(const std::filesystem::path& path) const
{
	std::set<std::filesystem::path> full_files;
	std::set<std::filesystem::path> found_files;

	for (const auto& mod: mods_ | std::views::reverse)
	{
		for (const std::filesystem::path& new_file: commonItems::GetAllFilesInFolderRecursive(mod.path / path))
		{
			if (found_files.contains(new_file))
			{
				continue;
			}

			found_files.insert(new_file);
			full_files.insert((mod.path / path / new_file).make_preferred());
		}

		if (PathIsReplaced(path, mod.replacedFolders))
		{
			return full_files;
		}
	}

	for (const std::filesystem::path& new_file: commonItems::GetAllFilesInFolderRecursive(game_root_ / path))
	{
		if (found_files.contains(new_file))
		{
			continue;
		}

		found_files.insert(new_file);
		full_files.insert((game_root_ / path / new_file).make_preferred());
	}

	return full_files;
}