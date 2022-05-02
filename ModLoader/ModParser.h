#ifndef MOD_PARSER_H
#define MOD_PARSER_H
#include "../ConvenientParser.h"
#include <set>

namespace commonItems
{
class ModParser: convenientParser
{
  public:
	ModParser() = default;
	void parseMod(std::istream& theStream);
	void parseMod(const std::string& fileName);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getPath() const { return path; }
	[[nodiscard]] auto isValid() const { return !name.empty() && !path.empty(); }
	[[nodiscard]] auto isCompressed() const { return compressed; }
	[[nodiscard]] const auto& getDependencies() const { return dependencies; }
	[[nodiscard]] const auto& getReplacedPaths() const { return replacedPaths; }

	void setPath(const std::string& thePath) { path = thePath; }

  private:
	void registerKeys();

	std::string name;
	std::string path;
	bool compressed = false;
	std::set<std::string> dependencies;
	std::set<std::string> replacedPaths;
};
} // namespace commonItems

#endif // MOD_PARSER_H