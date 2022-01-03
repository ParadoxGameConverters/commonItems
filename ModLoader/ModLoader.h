#ifndef MOD_LOADER_H
#define MOD_LOADER_H
#include <optional>
#include <set>
#include <string>
#include <vector>

using Name = std::string;
using Path = std::string;
struct Mod
{
	Mod() = default;
	explicit Mod(const Name& theName, const Path& thePath): name(theName), path(thePath) {}
	explicit Mod(const Name& theName, const Path& thePath, const std::set<Name> theDependencies):
		 name(theName), path(thePath), dependencies(theDependencies)
	{
	}
	Name name;
	Path path;
	std::set<Name> dependencies;
	bool operator==(const Mod& rhs) const { return rhs.name == name && rhs.path == path; }
};
using Mods = std::vector<Mod>;

namespace commonItems
{
class ModParser;
class ModLoader
{
  public:
	ModLoader() = default;

	void loadMods(const std::string& gameDocumentsPath, const Mods& incomingMods);

	[[nodiscard]] const auto& getMods() const { return usableMods; }

  private:
	void loadModDirectory(const std::string& gameDocumentsPath, const Mods& incomingMods);
	void processLoadedMod(ModParser& theMod,
		 const std::string& modName,
		 const std::string& modFileName,
		 const std::string& modPath,
		 const std::string& modsPath,
		 const std::string& gameDocumentsPath);
	static void warnForInvalidPath(const ModParser& theMod, const std::string& name, const std::string& path);
	void fileUnderCategory(const ModParser& theMod, const std::string& path);

	[[nodiscard]] std::optional<std::string> uncompressAndReturnNewPath(const std::string& modName) const;
	[[nodiscard]] bool extractZip(const std::string& archive, const std::string& path) const;

	Mods possibleUncompressedMods = {}; // name, absolute path to mod directory
	Mods possibleCompressedMods = {};	// name, absolute path to zip file
	Mods usableMods = {};					// name, absolute path for directories, relative for unpacked
};
} // namespace commonItems

#endif // MOD_LOADER_H