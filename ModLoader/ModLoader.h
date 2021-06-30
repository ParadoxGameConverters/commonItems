#ifndef MOD_LOADER_H
#define MOD_LOADER_H
#include <map>
#include <optional>
#include <string>
#include <vector>

using Mods = std::map<std::string, std::string>;							  // name, path.
using IncomingMods = std::vector<std::pair<std::string, std::string>>; // vector of name/path pairs.

namespace commonItems
{
class ModParser;
class ModLoader
{
  public:
	ModLoader() = default;

	void loadMods(const std::string& gameDocumentsPath, const IncomingMods& incomingMods);

	[[nodiscard]] const auto& getMods() const { return usableMods; }

  private:
	void loadModDirectory(const std::string& gameDocumentsPath, const IncomingMods& incomingMods);
	void processLoadedMod(ModParser& theMod,
		 const std::string& modName,
		 const std::string& modFileName,
		 const std::string& modPath,
		 const std::string& modsPath,
		 const std::string& gameDocumentsPath);
	static void warnForInvalidPath(const ModParser& theMod, const std::string& name, const std::string& path);
	void fileUnderCategory(const ModParser& theMod, const std::string& path);

	[[nodiscard]] std::optional<std::string> getModPath(const std::string& modName) const;
	[[nodiscard]] bool extractZip(const std::string& archive, const std::string& path) const;

	Mods possibleUncompressedMods = {}; // name, absolute path to mod directory
	Mods possibleCompressedMods = {};	// name, absolute path to zip file
	Mods usableMods = {};					// name, absolute path for directories, relative for unpacked
};
} // namespace commonItems

#endif // MOD_LOADER_H