#ifndef MOD_FILESYSTEM_H_
#define MOD_FILESYSTEM_H_


#include "Mod.h"
#include <optional>
#include <string>
#include <vector>



namespace commonItems
{

class ModFilesystem
{
  public:
	// The constructor establishes the root of the filesystem.
	// game_root points at the game's root folder, and all paths in the lookup functions will be based on that root.
	// mods is a list of the mods applied, in increasing order of precedence. Later mods will override files in the game root or earlier mods, and their
	// replace_paths will block earlier mods and the game It is the caller's responsibility to sort the mods appropriately
	explicit ModFilesystem(std::string_view game_root, std::vector<Mod> mods): game_root_(game_root), mods_(std::move(mods)) {}

	// lookup functions
	[[nodiscard]] std::optional<std::string> GetActualFileLocation(const std::string& path) const;
	[[nodiscard]] std::optional<std::string> GetActualFolderLocation(const std::string& path) const;
	[[nodiscard]] std::set<std::string> GetAllFilesInFolder(const std::string& path) const;
	[[nodiscard]] std::set<std::string> GetAllSubfolders(const std::string& path) const;
	[[nodiscard]] std::set<std::string> GetAllFilesInFolderRecursive(const std::string& path) const;

  private:
	std::string game_root_;
	std::vector<Mod> mods_;
};

} // namespace commonItems



#endif // MOD_FILESYSTEM_H_