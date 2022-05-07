#ifndef MOD_H_
#define MOD_H_


#include <set>
#include <string>



using Name = std::string;
using Path = std::string;
struct Mod
{
	Mod() = default;
	explicit Mod(const Name& theName, const Path& thePath): name(theName), path(thePath) {}
	explicit Mod(const Name& theName, const Path& thePath, const std::set<Name> theDependencies): name(theName), path(thePath), dependencies(theDependencies) {}
	explicit Mod(const Name& theName, const Path& thePath, const std::set<Name> theDependencies, const std::set<std::string> theReplacedFolders):
		 name(theName), path(thePath), dependencies(theDependencies), replacedFolders(theReplacedFolders)
	{
	}
	Name name;
	Path path;
	std::set<Name> dependencies;
	std::set<std::string> replacedFolders;
	bool operator==(const Mod& rhs) const { return rhs.name == name && rhs.path == path; }
};



#endif // MOD_H_