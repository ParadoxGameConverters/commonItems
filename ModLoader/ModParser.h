#ifndef MOD_PARSER_H
#define MOD_PARSER_H


#include <filesystem>

#include "../ConvenientParser.h"
#include <set>



namespace commonItems
{
class ModParser: convenientParser
{
  public:
	ModParser() = default;

	void parseMod(std::istream& theStream);
	void parseMod(const std::filesystem::path& fileName);
	void parseMetadata(std::istream& theStream);
	void parseMetadata(const std::filesystem::path& fileName);

	[[nodiscard]] const auto& getName() const { return name_; }
	[[nodiscard]] const auto& getFilesystemPath() const { return path_; }
	[[nodiscard]] auto isValid() const { return !name_.empty() && !path_.empty(); }
	[[nodiscard]] auto isCompressed() const { return compressed_; }
	[[nodiscard]] const auto& getDependencies() const { return dependencies_; }
	[[nodiscard]] const auto& getFilesystemReplacedPaths() const { return replacedPaths_; }

	void setPath(const std::filesystem::path& path) { path_ = path; }

  private:
	void registerKeys();

	std::string name_;
	std::filesystem::path path_;
	bool compressed_ = false;
	std::set<std::string> dependencies_;
	std::set<std::filesystem::path> replacedPaths_;
};
} // namespace commonItems

#endif // MOD_PARSER_H