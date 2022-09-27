#ifndef MOD_H_
#define MOD_H_


#include <set>
#include <string>



using Name = std::string;
using Path = std::string;
struct Mod
{
	Mod() = default;
	explicit Mod(Name theName, Path thePath): name(std::move(theName)), path(std::move(thePath)) {}
	explicit Mod(Name theName, Path thePath, std::set<Name> theDependencies):
		 name(std::move(theName)), path(std::move(thePath)), dependencies(std::move(theDependencies))
	{
	}
	explicit Mod(Name theName, Path thePath, std::set<Name> theDependencies, std::set<std::string> theReplacedFolders):
		 name(std::move(theName)), path(std::move(thePath)), dependencies(std::move(theDependencies)), replacedFolders(std::move(theReplacedFolders))
	{
	}
	Name name;
	Path path;
	std::set<Name> dependencies;
	std::set<std::string> replacedFolders;
	bool operator==(const Mod& rhs) const { return rhs.name == name && rhs.path == path; }
};



#endif // MOD_H_