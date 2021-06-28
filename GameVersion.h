#ifndef GAME_VERSION_H
#define GAME_VERSION_H
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

	explicit GameVersion(const int theFirstPart,
		 const int theSecondPart,
		 const int theThirdPart,
		 const int theFourthPart):
		 firstPart(theFirstPart),
		 secondPart(theSecondPart), thirdPart(theThirdPart), fourthPart(theFourthPart)
	{
	}
	explicit GameVersion(std::optional<int> theFirstPart,
		 std::optional<int> theSecondPart,
		 std::optional<int> theThirdPart,
		 std::optional<int> theFourthPart):
		 firstPart(theFirstPart),
		 secondPart(theSecondPart), thirdPart(theThirdPart), fourthPart(theFourthPart)
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
	bool isLargerishThan(const GameVersion& rhs) const;

	[[nodiscard]] std::string toString() const;
	[[nodiscard]] std::string toShortString() const;

	friend std::ostream& operator<<(std::ostream&, const GameVersion& version);

	[[nodiscard]] static std::optional<GameVersion> extractLauncherVersion(const std::string& filePath); // modern PDX games, scrapes launcher-settings.json.
	[[nodiscard]] static std::optional<GameVersion> extractReadMeVersion(const std::string& filePath); // extracts version from Vic2 ReadMe.txt/Readme.txt.
	[[nodiscard]] static std::optional<GameVersion> extractChangeLogVersion(const std::string& filePath); // extracts version from CK2 ChangeLog.txt.

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

std::ostream& operator<<(std::ostream&, const GameVersion& version);

#endif // GAME_VERSION_H