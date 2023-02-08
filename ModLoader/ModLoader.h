#ifndef MOD_LOADER_H
#define MOD_LOADER_H



#include "Mod.h"
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

	void loadMods(const std::string& gameDocumentsPath, const Mods& incomingMods);
	void loadMods(const std::vector<std::string>& gameModPaths, const Mods& incomingMods);
	void sortMods();

	[[nodiscard]] const auto& getMods() const { return usableMods; }

  private:
	void loadModDirectories(const std::vector<std::string>& gameModPaths, const Mods& incomingMods);
	void cacheModNames(const std::string& gameDocumentsPath);
	void processLoadedMod(ModParser& theMod,
		 const std::string& modName,
		 const std::string& modFileName,
		 const std::string& modPath,
		 const std::string& gameModPath);
	static void warnForInvalidPath(const ModParser& theMod, const std::string& name, const std::string& path);
	void fileUnderCategory(const ModParser& theMod, const std::string& path);

	[[nodiscard]] std::optional<std::string> uncompressAndReturnNewPath(const std::string& modName) const;
	[[nodiscard]] bool extractZip(const std::string& archive, const std::string& path) const;

	Mods possibleUncompressedMods = {}; // name, absolute path to mod directory
	Mods possibleCompressedMods = {};	// name, absolute path to zip file
	Mods usableMods = {};					// name, absolute path for directories, relative for unpacked

	std::map<Name, Path> modCache;
};

} // namespace commonItems



#endif // MOD_LOADER_H