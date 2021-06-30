#include "ModLoader.h"
#include "../CommonFunctions.h"
#include "../Log.h"
#include "../OSCompatibilityLayer.h"
#include "../ZipLib/ZipFile.h"
#include "ModParser.h"
#include <filesystem>
#include <fstream>
#include <ranges>
#include <set>
#include <stdexcept>
#include <string>
namespace fs = std::filesystem;

void commonItems::ModLoader::loadMods(const std::string& gameDocumentsPath, const IncomingMods& incomingMods)
{
	if (incomingMods.empty())
	{
		// We shouldn't even be here if the save didn't have mods! Why were Mods called?
		Log(LogLevel::Info) << "No mods were detected in savegame. Skipping mod processing.";
		return;
	}

	// We enter this function with a vector of (optional) mod names and (required) mod file locations from the savegame.
	// We need to read all the mod files, check their paths (and potential archives for ancient mods) unpack what's
	// necessary, and exit, with a map of updated mod names (savegame can differ from actual mod file) and mod folder
	// locations.

	// The function below reads all the incoming .mod files and verifies their internal paths/archives are correct and
	// point to something present on disk. No unpacking yet.
	loadModDirectory(gameDocumentsPath, incomingMods);

	// Now we merge all detected .mod files together.
	Log(LogLevel::Info) << "\tDetermining Mod Usability";
	auto allMods = possibleUncompressedMods;
	allMods.insert(possibleCompressedMods.begin(), possibleCompressedMods.end());

	// With a list of all detected and matched mods, we unpack the compressed ones if any and store the result.
	for (const auto& usedModName: std::views::keys(allMods))
	{
		// This invocation (getModPath) will unpack any compressed mods into our converter's folder.
		if (auto possibleModPath = getModPath(usedModName); possibleModPath)
		{
			if (!DoesFolderExist(*possibleModPath) && !DoesFileExist(*possibleModPath))
			{
				Log(LogLevel::Warning) << "\t\t" << usedModName
											  << " could not be found in the specified mod directory. Tried: " << *possibleModPath;
				continue;
			}

			// All verified mods go into usableMods
			Log(LogLevel::Info) << "\t\t->> Found potentially useful [" << usedModName << "]: " << *possibleModPath + "/";
			usableMods.insert(std::pair(usedModName, *possibleModPath + "/"));
		}
		else
		{
			Log(LogLevel::Warning)
				 << "\t\tNo path could be found for " << usedModName
				 << ". Check that the mod is present and that the .mod file specifies the path for the mod";
		}
	}
}

void commonItems::ModLoader::loadModDirectory(const std::string& gameDocumentsPath, const IncomingMods& incomingMods)
{
	const auto& modsPath = gameDocumentsPath + "/mod";
	if (!DoesFolderExist(modsPath))
		throw std::invalid_argument("Mods directory path is invalid! Is it at: " + gameDocumentsPath + "/mod/ ?");

	Log(LogLevel::Info) << "\tMods directory is " << modsPath;

	const auto diskModNames = GetAllFilesInFolder(modsPath);
	for (const auto& [usedModName, usedModFilePath]: incomingMods)
	{
		const auto trimmedModFileName = trimPath(usedModFilePath);
		if (!diskModNames.contains(trimmedModFileName))
		{
			if (usedModName.empty())
				Log(LogLevel::Warning)
					 << "\t\tSavegame uses mod at " << usedModFilePath
					 << " which is not present on disk.  Skipping at your risk, but this can greatly affect conversion.";
			else
				Log(LogLevel::Warning)
					 << "\t\tSavegame uses [" << usedModName << "] at " << usedModFilePath
					 << " which is not present on disk.  Skipping at your risk, but this can greatly affect conversion.";
			continue;
		}
		if (getExtension(trimmedModFileName) != "mod")
			continue; // shouldn't be necessary but just in case.
		try
		{
			std::ifstream modFile(fs::u8path(modsPath + "/" + trimmedModFileName));
			ModParser theMod(modFile);
			modFile.close();

			if (!theMod.isValid())
			{
				Log(LogLevel::Warning) << "\t\tMod at " << modsPath + "/" + trimmedModFileName << " does not look valid.";
				continue;
			}

			if (!theMod.isCompressed())
			{
				if (!DoesFolderExist(theMod.getPath()))
				{
					// Maybe we have a relative path
					if (DoesFolderExist(gameDocumentsPath + "/" + theMod.getPath()))
					{
						// fix this.
						theMod.setPath(gameDocumentsPath + "/" + theMod.getPath());
					}
					else
					{
						if (usedModName.empty())
							Log(LogLevel::Warning)
								 << "\t\tMod at " + usedModFilePath + " points to " + theMod.getPath() +
										  " which does not exist! Skipping at your risk, but this can greatly affect conversion.";
						else
							Log(LogLevel::Warning)
								 << "\t\tMod [" << usedModName
								 << "] at " + usedModFilePath + " points to " + theMod.getPath() +
										  " which does not exist! Skipping at your risk, but this can greatly affect conversion.";

						continue;
					}
				}

				possibleUncompressedMods.emplace(theMod.getName(), theMod.getPath());
				Log(LogLevel::Info) << "\t\tFound potential mod named " << theMod.getName() << " with a mod file at "
										  << modsPath + "/" + trimmedModFileName << " and itself at " << theMod.getPath();
			}
			else
			{
				// Maybe we have a relative path
				if (DoesFileExist(gameDocumentsPath + "/" + theMod.getPath()))
				{
					// fix this.
					theMod.setPath(gameDocumentsPath + "/" + theMod.getPath());
				}
				else
				{
					if (!DoesFileExist(theMod.getPath()))
					{
						if (usedModName.empty())
							Log(LogLevel::Warning)
								 << "\t\tMod at " + usedModFilePath + " points to " + theMod.getPath() +
										  " which does not exist! Skipping at your risk, but this can greatly affect conversion.";
						else
							Log(LogLevel::Warning)
								 << "\t\tMod [" << usedModName
								 << "] at " + usedModFilePath + " points to " + theMod.getPath() +
										  " which does not exist! Skipping at your risk, but this can greatly affect conversion.";
						continue;
					}
				}
				possibleCompressedMods.emplace(theMod.getName(), theMod.getPath());
				Log(LogLevel::Info) << "\t\tFound a compressed mod [" << theMod.getName() << "] with a mod file at "
										  << modsPath << "/" << trimmedModFileName << " and itself at " << theMod.getPath();
			}
		}
		catch (std::exception&)
		{
			Log(LogLevel::Warning) << "Error while reading " << modsPath << "/" << trimmedModFileName
										  << "! Mod will not be useable for conversions.";
		}
	}
}

std::optional<std::string> commonItems::ModLoader::getModPath(const std::string& modName) const
{
	if (const auto& mod = possibleUncompressedMods.find(modName); mod != possibleUncompressedMods.end())
	{
		return mod->second;
	}

	if (const auto& compressedMod = possibleCompressedMods.find(modName); compressedMod != possibleCompressedMods.end())
	{
		const auto archivePath = compressedMod->second;
		const auto uncompressedName = trimPath(trimExtension(archivePath));

		TryCreateFolder("mods/");

		if (!DoesFolderExist("mods/" + uncompressedName))
		{
			Log(LogLevel::Info) << "\t\tUncompressing: " << archivePath;
			if (!extractZip(archivePath, "mods/" + uncompressedName))
			{
				Log(LogLevel::Warning) << "We're having trouble automatically uncompressing your mod.";
				Log(LogLevel::Warning) << "Please, manually uncompress: " << archivePath;
				Log(LogLevel::Warning) << "Into: [converter's folder]/mods/" << uncompressedName;
				Log(LogLevel::Warning) << "Then run the converter again. Thank you and good luck.";
				return std::nullopt;
			}
		}

		if (DoesFolderExist("mods/" + uncompressedName))
		{
			return "mods/" + uncompressedName;
		}
	}

	return std::nullopt;
}

bool commonItems::ModLoader::extractZip(const std::string& archive, const std::string& path) const
{
	TryCreateFolder(path);
	auto modFile = ZipFile::Open(archive);
	if (!modFile)
		return false;
	for (size_t entryNum = 0; entryNum < modFile->GetEntriesCount(); ++entryNum)
	{
		const auto& entry = modFile->GetEntry(static_cast<int>(entryNum));
		const auto& inPath = entry->GetFullName();
		const auto& name = entry->GetName();
		if (entry->IsDirectory())
			continue;

		// Does target directory exist?
		const auto dirNamePos = inPath.find(name);

		if (const auto dirName = path + "/" + inPath.substr(0, dirNamePos); !DoesFolderExist(dirName))
		{
			// we need to craft our way through to target directory.
			auto remainder = inPath;
			auto currentPath = path;
			while (remainder != name)
			{
				if (const auto pos = remainder.find_first_of('/'); pos != std::string::npos)
				{
					auto makeDirName = remainder.substr(0, pos);
					currentPath += "/" + makeDirName;
					TryCreateFolder(currentPath);
					remainder = remainder.substr(pos + 1, remainder.length());
				}
				else
					break;
			}
		}
		ZipFile::ExtractFile(archive, inPath, path + "/" + inPath);
	}
	return true;
}