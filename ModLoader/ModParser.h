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
	[[deprecated("Use std::filesystem::path version")]] void parseMod(const std::string& fileName);

	void parseMetadata(std::istream& theStream);
	void parseMetadata(const std::filesystem::path& fileName);
	[[deprecated("Use std::filesystem::path version")]] void parseMetadata(const std::string& fileName);

	[[nodiscard]] const auto& getName() const { return name_; }
	[[nodiscard]] const auto& getFilesystemPath() const { return path_; }
	[[deprecated("Use getFilesystemPath()")]] [[nodiscard]] const std::string getPath() const { return path_.string(); }
	[[nodiscard]] auto isValid() const { return !name_.empty() && !path_.empty(); }
	[[nodiscard]] auto isCompressed() const { return compressed_; }
	[[nodiscard]] const auto& getDependencies() const { return dependencies_; }
	[[nodiscard]] const auto& getFilesystemReplacedPaths() const { return replacedPaths_; }
	[[deprecated("Use getFilesystemReplacedPaths()")]] [[nodiscard]] const std::set<std::string> getReplacedPaths() const;

	void setPath(const std::filesystem::path& path) { path_ = path; }
	[[deprecated("Use std::filesystem::path version")]] void setPath(const std::string& path);

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