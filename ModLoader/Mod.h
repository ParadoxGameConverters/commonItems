#ifndef MOD_H_
#define MOD_H_


#include <filesystem>
#include <set>
#include <string>



using Name = std::string;
using Path = std::filesystem::path;
struct Mod
{
	Mod() = default;
	explicit Mod(Name theName, Path thePath): name(std::move(theName)), path(std::move(thePath)) {}
	explicit Mod(Name theName, Path thePath, std::set<Name> theDependencies):
		 name(std::move(theName)), path(std::move(thePath)), dependencies(std::move(theDependencies))
	{
	}
	explicit Mod(Name theName, Path thePath, std::set<Name> theDependencies, std::set<Path> theReplacedFolders):
		 name(std::move(theName)), path(std::move(thePath)), dependencies(std::move(theDependencies)), replacedFolders(std::move(theReplacedFolders))
	{
	}

	bool operator==(const Mod& rhs) const { return rhs.name == name && rhs.path == path && rhs.name == name; }

	Name name;
	Path path;
	std::set<Name> dependencies;
	std::set<Path> replacedFolders;
};



#endif // MOD_H_