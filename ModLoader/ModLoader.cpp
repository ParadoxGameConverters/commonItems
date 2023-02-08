#include "ModLoader.h"
#include "../CommonFunctions.h"
#include "../Log.h"
#include "../OSCompatibilityLayer.h"
#include "../external/zip/src/zip.h"
#include "ModParser.h"
#include <filesystem>
#include <set>
#include <stdexcept>
#include <string>



void commonItems::ModLoader::loadMods(const std::string& gameDocumentsPath, const Mods& incomingMods)
{
	loadMods(std::vector{gameDocumentsPath + "/mod"}, incomingMods);
}


void commonItems::ModLoader::loadMods(const std::vector<std::string>& gameModPaths, const Mods& incomingMods)
{
	if (gameModPaths.empty())
	{
		Log(LogLevel::Info) << "No mod directories were provided. Skipping mod processing.";
		return;
	}


	if (incomingMods.empty())
	{
		// We shouldn't even be here if the save didn't have mods! Why were Mods called?
		Log(LogLevel::Info) << "No mods were detected in savegame. Skipping mod processing.";
		return;
	}

	// First see what we're up against. Load mod folders, and cache the mod names. We need the names as bare minimum in case
	// we're doing old-style name-recognition modfinding and don't have the paths in incomingMods.
	Log(LogLevel::Info) << "\tMods directories are:";
	for (const auto& gameModPath: gameModPaths)
	{
		Log(LogLevel::Info) << "\t\t" << gameModPath;
		cacheModNames(gameModPath);
	}

	// We enter this function with a vector of (optional) mod names and (required) mod file locations from the savegame.
	// We need to read all the mod files, check their paths (and potential archives for ancient mods) unpack what's
	// necessary, and exit with a vector of updated mod names (savegame can differ from actual mod file) and mod folder
	// locations.

	// The function below reads all the incoming .mod files and verifies their internal paths/archives are correct and
	// point to something present on disk. No unpacking yet.
	loadModDirectories(gameModPaths, incomingMods);

	// Now we merge all detected .mod files together.
	Log(LogLevel::Info) << "\tDetermining Mod Usability";
	auto allMods = possibleUncompressedMods;
	allMods.insert(allMods.end(), possibleCompressedMods.begin(), possibleCompressedMods.end());

	// With a list of all detected and matched mods, we unpack the compressed ones (if any) and store the results.
	for (const auto& mod: allMods)
	{
		// This invocation will unpack any compressed mods into our converter's folder, and skip already unpacked ones.
		const auto possibleModPath = uncompressAndReturnNewPath(mod.name);
		if (!possibleModPath)
		{
			Log(LogLevel::Warning) << "\t\tFailure unpacking " << mod.name << ", skipping this mod at your risk.";
			continue;
		}

		// All verified mods go into usableMods
		Log(LogLevel::Info) << "\t\t->> Found potentially useful [" << mod.name << "]: " << *possibleModPath + "/";
		usableMods.emplace_back(Mod(mod.name, *possibleModPath + "/", mod.dependencies, mod.replacedFolders));
	}
}

void commonItems::ModLoader::loadModDirectories(const std::vector<std::string>& gameModPaths, const Mods& incomingMods)
{
	std::set<std::string> diskModNames;
	for (const auto& modPath: gameModPaths)
	{
		for (const auto& diskModName: GetAllFilesInFolder(modPath))
		{
			diskModNames.insert(diskModName);
		}
	}

	for (auto mod: incomingMods)
	{
		// If we don't have a loaded mod path but have it in our cache, might as well fix it.
		if (mod.path.empty() && modCache.contains(mod.name))
			mod.path = modCache.at(mod.name);

		const auto trimmedModFileName = trimPath(mod.path);

		// We either have the path as the reference point (in which case name we'll read ourselves), or the name, in which case we looked up the
		// cached map for the path.
		// If we have neither, that's unusable.

		if (!diskModNames.contains(trimmedModFileName) && !modCache.contains(mod.name))
		{
			if (mod.name.empty())
				Log(LogLevel::Warning) << "\t\tSavegame uses mod at " << mod.path
											  << " which is not present on disk. Skipping at your risk, but this can greatly affect conversion.";
			else if (mod.path.empty())
				Log(LogLevel::Warning) << "\t\tSavegame uses [" << mod.name
											  << "] which is not present on disk. Skipping at your risk, but this can greatly affect conversion.";
			else
				Log(LogLevel::Warning) << "\t\tSavegame uses [" << mod.name << "] at " << mod.path
											  << " which is not present on disk. Skipping at your risk, but this can greatly affect conversion.";
			continue;
		}

		// if we do have a path incoming from the save, just make sure it's not some abnormality.
		if (!trimmedModFileName.empty() && getExtension(trimmedModFileName) != "mod")
		{
			// Vic3 mods won't have .mod, but they will be in the cache and lack .mod there
			if (!modCache.contains(mod.name) || modCache.at(mod.name).ends_with(".mod"))
				continue; // shouldn't be necessary but just in case.
		}

		// Attempt parsing .mod file
		for (const auto& gameModPath: gameModPaths)
		{
			if (!trimmedModFileName.empty() && trimmedModFileName.ends_with(".mod"))
			{
				const std::string mod_file_location = gameModPath + "/" + trimmedModFileName;

				if (!DoesFileExist(mod_file_location))
				{
					continue;
				}

				ModParser theMod;
				try
				{
					theMod.parseMod(mod_file_location);
				}
				catch (std::exception&)
				{
					Log(LogLevel::Warning) << "\t\tError while reading " << mod_file_location << "! Mod will not be useable for conversions.";
					continue;
				}
				processLoadedMod(theMod, mod.name, trimmedModFileName, mod.path, gameModPath);
				break;
			}
			else
			{
				// Vic3 mods

				std::string mod_folder = mod.path.substr(mod.path.find_last_of('/') + 1, mod.path.size());
				const std::string metadata_location = gameModPath + "/" + mod_folder + "/.metadata/metadata.json";
				if (!DoesFileExist(metadata_location))
				{
					continue;
				}

				ModParser theMod;
				try
				{
					theMod.parseMetadata(metadata_location);
				}
				catch (std::exception&)
				{
					Log(LogLevel::Warning) << "\t\tError while reading " << metadata_location << "! Mod will not be useable for conversions.";
					continue;
				}
				possibleUncompressedMods.emplace_back(Mod(theMod.getName(), theMod.getPath(), theMod.getDependencies(), theMod.getReplacedPaths()));
				Log(LogLevel::Info) << "\t\tFound a potential mod [" << theMod.getName() << "] at " << theMod.getPath();
				break;
			}
		}
	}
}

void commonItems::ModLoader::cacheModNames(const std::string& gameDocumentsPath)
{
	if (!DoesFolderExist(gameDocumentsPath))
		throw std::invalid_argument("Mods directory path is invalid! Is it at: " + gameDocumentsPath + " ?");

	for (const auto& diskModFile: GetAllFilesInFolder(gameDocumentsPath))
	{
		if (getExtension(diskModFile) != "mod")
			continue;
		ModParser theMod;
		const auto trimmedModFileName = trimPath(diskModFile);
		try
		{
			theMod.parseMod(gameDocumentsPath + "/" + trimmedModFileName);
		}
		catch (std::exception&)
		{
			Log(LogLevel::Warning) << "\t\tError while caching " << gameDocumentsPath << "/" << trimmedModFileName << "! Mod will not be useable for conversions.";
			continue;
		}
		if (theMod.isValid())
			modCache.emplace(theMod.getName(), diskModFile);
	}

	for (const auto& possible_mod_folder: GetAllSubfolders(gameDocumentsPath))
	{
		const std::string metadata_location = gameDocumentsPath + "/" + possible_mod_folder + "/.metadata/metadata.json";
		if (!DoesFileExist(metadata_location))
		{
			continue;
		}

		ModParser theMod;
		try
		{
			theMod.parseMetadata(metadata_location);
		}
		catch (std::exception&)
		{
			Log(LogLevel::Warning) << "\t\tError while caching " << possible_mod_folder << "! Mod will not be useable for conversions.";
			continue;
		}
		if (theMod.isValid())
			modCache.emplace(theMod.getName(), theMod.getPath());
	}
}

void commonItems::ModLoader::processLoadedMod(ModParser& theMod,
	 const std::string& modName,
	 const std::string& modFileName,
	 const std::string& modPath,
	 const std::string& gameModPath)
{
	if (!theMod.isValid())
	{
		Log(LogLevel::Warning) << "\t\tMod at " << gameModPath + "/" + modFileName << " does not look valid.";
		return;
	}

	// Fix potential pathing issues.
	if (!theMod.isCompressed() && !DoesFolderExist(theMod.getPath()))
	{
		// Maybe we have a relative path
		const auto trimmedPath = trimPath(theMod.getPath());
		if (DoesFolderExist(gameModPath + "/" + trimmedPath))
		{
			// fix this.
			theMod.setPath(gameModPath + "/" + trimmedPath);
		}
		else
		{
			warnForInvalidPath(theMod, modName, modPath);
			return;
		}
	}
	else if (theMod.isCompressed() && !DoesFileExist(theMod.getPath()))
	{
		// Maybe we have a relative path
		const auto trimmedPath = trimPath(theMod.getPath());
		if (DoesFileExist(gameModPath + "/" + trimmedPath))
		{
			// fix this.
			theMod.setPath(gameModPath + "/" + trimmedPath);
		}
		else
		{
			warnForInvalidPath(theMod, modName, modPath);
			return;
		}
	}

	// file under category.
	fileUnderCategory(theMod, gameModPath + "/" + modFileName);
}

void commonItems::ModLoader::warnForInvalidPath(const ModParser& theMod, const std::string& name, const std::string& path)
{
	if (name.empty())
		Log(LogLevel::Warning) << "\t\tMod at " + path + " points to " + theMod.getPath() +
												" which does not exist! Skipping at your risk, but this can greatly affect conversion.";
	else
		Log(LogLevel::Warning) << "\t\tMod [" << name
									  << "] at " + path + " points to " + theMod.getPath() +
												" which does not exist! Skipping at your risk, but this can greatly affect conversion.";
}

void commonItems::ModLoader::fileUnderCategory(const ModParser& theMod, const std::string& path)
{
	if (!theMod.isCompressed())
	{
		possibleUncompressedMods.emplace_back(Mod(theMod.getName(), theMod.getPath(), theMod.getDependencies(), theMod.getReplacedPaths()));
		Log(LogLevel::Info) << "\t\tFound a potential mod [" << theMod.getName() << "] with a mod file at " << path << " and itself at " << theMod.getPath();
	}
	else
	{
		possibleCompressedMods.emplace_back(Mod(theMod.getName(), theMod.getPath(), theMod.getDependencies(), theMod.getReplacedPaths()));
		Log(LogLevel::Info) << "\t\tFound a compressed mod [" << theMod.getName() << "] with a mod file at " << path << " and itself at " << theMod.getPath();
	}
}

std::optional<std::string> commonItems::ModLoader::uncompressAndReturnNewPath(const std::string& modName) const
{
	for (const auto& mod: possibleUncompressedMods)
	{
		if (mod.name == modName)
			return mod.path;
	}

	for (const auto& compressedMod: possibleCompressedMods)
	{
		if (compressedMod.name != modName)
			continue;

		const auto uncompressedName = trimPath(trimExtension(compressedMod.path));

		TryCreateFolder("mods/");

		if (!DoesFolderExist("mods/" + uncompressedName))
		{
			Log(LogLevel::Info) << "\t\tUncompressing: " << compressedMod.path;
			if (!extractZip(compressedMod.path, "mods/" + uncompressedName))
			{
				Log(LogLevel::Warning) << "We're having trouble automatically uncompressing your mod.";
				Log(LogLevel::Warning) << "Please, manually uncompress: " << compressedMod.path;
				Log(LogLevel::Warning) << "Into converter's folder, mods/" << uncompressedName << " subfolder.";
				Log(LogLevel::Warning) << "Then run the converter again. Thank you and good luck.";
				return std::nullopt;
			}
		}

		if (DoesFolderExist("mods/" + uncompressedName))
		{
			return "mods/" + uncompressedName;
		}
		return std::nullopt;
	}

	return std::nullopt;
}

bool commonItems::ModLoader::extractZip(const std::string& archive, const std::string& path) const
{
	TryCreateFolder(path);

	const int result = zip_extract(archive.c_str(), path.c_str(), NULL, NULL);

	if (result != 0)
	{
		DeleteFolder(path);
		return false;
	}

	return true;
}


void commonItems::ModLoader::sortMods()
{
	// using Kahn's algorithm - https://en.wikipedia.org/wiki/Topological_sorting#Kahn's_algorithm
	auto unsortedMods = usableMods;

	// track incoming edges
	std::map<std::string, std::set<std::string>> incomingDependencies;
	for (const auto& mod: unsortedMods)
	{
		for (const auto& dependency: mod.dependencies)
		{
			if (auto [itr, inserted] = incomingDependencies.emplace(dependency, std::set{mod.name}); !inserted)
			{
				itr->second.insert(mod.name);
			}
		}
	}

	// add mods with no incoming edges to the sorted mods
	Mods sortedMods;
	while (!unsortedMods.empty())
	{
		auto itr = unsortedMods.begin();
		while (incomingDependencies.contains(itr->name))
		{
			++itr;
			if (itr == unsortedMods.end())
			{
				throw std::invalid_argument("A mod dependency was missing.");
			}
		}

		sortedMods.push_back(*itr);

		for (const auto& dependencyName: itr->dependencies)
		{
			auto dependency = incomingDependencies.find(dependencyName);
			dependency->second.erase(itr->name);
			if (dependency->second.empty())
			{
				incomingDependencies.erase(dependencyName);
			}
		}

		unsortedMods.erase(itr);
	}

	usableMods = sortedMods;
}