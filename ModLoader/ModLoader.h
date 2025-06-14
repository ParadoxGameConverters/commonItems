#ifndef MOD_LOADER_H
#define MOD_LOADER_H



#include "Mod.h"
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>



using Mods = std::vector<Mod>;



namespace commonItems
{

class ModParser;


class ModLoader
{
  public:
	ModLoader() = default;

	void loadMods(const std::filesystem::path& gameDocumentsPath, const Mods& incomingMods);
	void loadMods(const std::vector<std::filesystem::path>& gameModPaths, const Mods& incomingMods);
	void sortMods();

	[[nodiscard]] const auto& getMods() const { return usableMods; }

  private:
	void loadModDirectories(const std::vector<std::filesystem::path>& gameModPaths, const Mods& incomingMods);
	void cacheModNames(const std::filesystem::path& gameDocumentsPath);
	void processLoadedMod(ModParser& theMod,
		 const std::string& modName,
		 const std::filesystem::path& modFileName,
		 const std::filesystem::path& modPath,
		 const std::filesystem::path& gameModPath,
		 bool metaMod = false);
	static void warnForInvalidPath(const ModParser& theMod, const std::string& name, const std::filesystem::path& path);
	void fileUnderCategory(const ModParser& theMod, const std::filesystem::path& path, bool metaMod = false);

	[[nodiscard]] std::optional<std::filesystem::path> uncompressAndReturnNewPath(const std::string& modName) const;
	[[nodiscard]] bool extractZip(const std::filesystem::path& archive, const std::filesystem::path& path) const;

	Mods possibleUncompressedMods = {}; // name, absolute path to mod directory
	Mods possibleCompressedMods = {};	// name, absolute path to zip file
	Mods usableMods = {};					// name, absolute path for directories, relative for unpacked

	std::map<Name, Path> modCache;
};

} // namespace commonItems



#endif // MOD_LOADER_H