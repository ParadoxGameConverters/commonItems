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
	explicit ModFilesystem(std::string_view game_root, std::vector<Mod> mods): game_root_(game_root), mods_(std::move(mods)) {}

	// lookup functions
	[[nodiscard]] std::optional<std::string> GetActualFileLocation(const std::string& path) const;
	[[nodiscard]] std::optional<std::string> GetActualFolderLocation(const std::string& path) const;
	[[nodiscard]] std::set<std::string> GetAllFilesInFolder(const std::string& path) const;
	[[nodiscard]] std::set<std::string> GetAllSubfolders(std::string_view path) const;
	[[nodiscard]] std::set<std::string> GetAllFilesInFolderRecursive(std::string_view path) const;

  private:
	std::string game_root_;
	std::vector<Mod> mods_;
};

} // namespace commonItems



#endif // MOD_FILESYSTEM_H_