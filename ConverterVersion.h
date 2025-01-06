#ifndef CONVERTER_VERSION_H
#define CONVERTER_VERSION_H



// A class to handle converter-style versions



#include "GameVersion.h"
#include "Parser.h"



namespace commonItems
{

class ConverterVersion: convenientParser
{
  public:
	ConverterVersion() = default;
	void loadVersion(const std::filesystem::path& filename);
	[[deprecated("Use std::filesystem::path version")]] void loadVersion(const std::string& filename);
	void loadVersion(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getVersion() const { return version; }
	[[nodiscard]] const auto& getSource() const { return source; }
	[[nodiscard]] const auto& getTarget() const { return target; }
	[[nodiscard]] const auto& getMinSource() const { return minSource; }
	[[nodiscard]] const auto& getMaxSource() const { return maxSource; }
	[[nodiscard]] const auto& getMinTarget() const { return minTarget; }
	[[nodiscard]] const auto& getMaxTarget() const { return maxTarget; }

	[[nodiscard]] std::string getDescription() const;

	friend std::ostream& operator<<(std::ostream& output, const ConverterVersion& version);

  private:
	void registerKeys();

	std::string name;
	std::string version;
	std::string source;
	std::string target;
	GameVersion minSource;
	GameVersion maxSource;
	GameVersion minTarget;
	GameVersion maxTarget;
};
std::ostream& operator<<(std::ostream& output, const ConverterVersion& version);

} // namespace commonItems



#endif // CONVERTER_VERSION_H