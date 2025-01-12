#ifndef MOD_FILESYSTEM_H_
#define MOD_FILESYSTEM_H_


#include "Mod.h"
#include <filesystem>
#include <optional>
#include <set>
#include <vector>



namespace commonItems
{

class ModFilesystem
{
  public:
	explicit ModFilesystem() = default;

	// The constructor establishes the root of the filesystem.
	// game_root points at the game's root folder, and all paths in the lookup functions will be based on that root.
	// mods is a list of the mods applied, in increasing order of precedence. Later mods will override files in the game root or earlier mods, and their
	// replace_paths will block earlier mods and the game It is the caller's responsibility to sort the mods appropriately
	explicit ModFilesystem(std::filesystem::path game_root, std::vector<Mod> mods): game_root_(game_root), mods_(std::move(mods)) {}
#pragma warning(push)
#pragma warning(disable : 4996)
	[[deprecated("Use the std::filesystem::path version")]] explicit ModFilesystem(std::string_view game_root, std::vector<Mod> mods):
		 game_root_(std::filesystem::u8path(game_root)), mods_(std::move(mods))
	{
	}
#pragma warning(pop)

	// lookup functions
	[[nodiscard]] std::optional<std::filesystem::path> GetActualFileLocation(const std::filesystem::path& path) const;
	[[nodiscard]] std::optional<std::filesystem::path> GetActualFolderLocation(const std::filesystem::path& path) const;
	[[nodiscard]] std::set<std::filesystem::path> GetAllFilesInFolder(const std::filesystem::path& path) const;
	[[nodiscard]] std::set<std::filesystem::path> GetAllSubfolders(const std::filesystem::path& path) const;
	[[nodiscard]] std::set<std::filesystem::path> GetAllFilesInFolderRecursive(const std::filesystem::path& path) const;

  private:
	std::filesystem::path game_root_;
	std::vector<Mod> mods_;
};

} // namespace commonItems



#endif // MOD_FILESYSTEM_H_