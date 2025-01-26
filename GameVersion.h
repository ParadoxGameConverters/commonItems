#ifndef GAME_VERSION_H
#define GAME_VERSION_H



// A class and some helpers representing the version of a Paradox game.Assumes the version consists of four integers(1.12.4.5), but versions with fewer parts
// will work seamlessly.
//
// GameVersion
//  Construction
//   * Default construction gives a version of 0.0.0.0
//   * Can directly specify all four parts
//   * Can construct via string - "1.2.3.4", "1.6.7", ""
//   * Can construct via stream - "version = { first = 1 second = 2 third = 3 forth = 4 }"
//   * The misspelling of 'fourth' is Paradox's
//  Comparison
//   GameVersions can be compared using all the standard comparators.It is a simple lexicographic comparison in order of the parts.
//
// GameVersion::Factory
//  A factory is provided for an alternate construction method. It only provides for construction via a stream.
//
// Output
//  A freestanding output function allows writing a GameVersion to output streams.


#include <filesystem>

#include "ConvenientParser.h"
#include <string>



class GameVersion: commonItems::convenientParser
{
  public:
	class Factory;
	GameVersion() = default;
	GameVersion(const GameVersion&) = default;
	GameVersion(GameVersion&&) = default;
	GameVersion& operator=(const GameVersion&) = default;
	GameVersion& operator=(GameVersion&&) = default;
	~GameVersion() = default;

	explicit GameVersion(const int theFirstPart, const int theSecondPart, const int theThirdPart, const int theFourthPart):
		 firstPart(theFirstPart), secondPart(theSecondPart), thirdPart(theThirdPart), fourthPart(theFourthPart)
	{
	}
	explicit GameVersion(std::optional<int> theFirstPart, std::optional<int> theSecondPart, std::optional<int> theThirdPart, std::optional<int> theFourthPart):
		 firstPart(theFirstPart), secondPart(theSecondPart), thirdPart(theThirdPart), fourthPart(theFourthPart)
	{
	}
	explicit GameVersion(std::string version);
	explicit GameVersion(std::istream& theStream);

	bool operator>=(const GameVersion& rhs) const;
	bool operator>(const GameVersion& rhs) const;
	bool operator<(const GameVersion& rhs) const;
	bool operator<=(const GameVersion& rhs) const;
	bool operator==(const GameVersion& rhs) const;
	bool operator!=(const GameVersion& rhs) const;

	// Largerish is intended for fuzzy comparisons like "converter works with up to 1.9",
	// so everything incoming on rhs from 0.0.0.0 to 1.9.x.y will match, (where x and y are >= 0),
	// thus overshooting the internal "1.9.0.0" setup. This works if ".0.0" are actually undefined.
	[[nodiscard]] bool isLargerishThan(const GameVersion& rhs) const;

	[[nodiscard]] std::string toString() const;
	[[nodiscard]] std::string toShortString() const;
	[[nodiscard]] std::string toWildCard() const;

	friend std::ostream& operator<<(std::ostream& out, const GameVersion& version);

	// modern PDX games, scrapes launcher-settings.json.
	[[nodiscard]] static std::optional<GameVersion> extractVersionFromLauncher(const std::filesystem::path& filePath);
	[[deprecated("Use the std::filesystem::path version")]] [[nodiscard]] static std::optional<GameVersion> extractVersionFromLauncher(
		 const std::string& filePath);
	[[nodiscard]] static std::optional<GameVersion> extractVersionByStringFromLauncher(const std::string& versionString, const std::filesystem::path& filePath);
	[[deprecated("Use the std::filesystem::path version")]] [[nodiscard]] static std::optional<GameVersion> extractVersionByStringFromLauncher(
		 const std::string& versionString,
		 const std::string& filePath);
	// extracts version from CK2 ChangeLog.txt.
	// extracts version from Vic2 ReadMe.txt/Readme.txt.
	[[nodiscard]] static std::optional<GameVersion> extractVersionFromReadMe(const std::filesystem::path& filePath);
	[[deprecated("Use the std::filesystem::path version")]] [[nodiscard]] static std::optional<GameVersion> extractVersionFromReadMe(
		 const std::string& filePath);
	// extracts version from CK2 ChangeLog.txt.
	[[nodiscard]] static std::optional<GameVersion> extractVersionFromChangeLog(const std::filesystem::path& filePath);
	[[deprecated("Use the std::filesystem::path version")]] [[nodiscard]] static std::optional<GameVersion> extractVersionFromChangeLog(
		 const std::string& filePath);

  private:
	void registerKeys();
	std::optional<int> firstPart;
	std::optional<int> secondPart;
	std::optional<int> thirdPart;
	std::optional<int> fourthPart;
};


class GameVersion::Factory: convenientParser
{
  public:
	Factory();
	GameVersion getVersion(std::istream& theStream);

  private:
	std::optional<int> firstPart;
	std::optional<int> secondPart;
	std::optional<int> thirdPart;
	std::optional<int> fourthPart;
};


std::ostream& operator<<(std::ostream& out, const GameVersion& version);



#endif // GAME_VERSION_H