#ifndef GAME_VERSION_H
#define GAME_VERSION_H
#include <ostream>
#include <string>
#include "CTReParser.h"

class GameVersion: commonItems::CTReParser
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
	explicit GameVersion(std::string version);
	explicit GameVersion(std::istream& theStream);

	bool operator>=(const GameVersion& rhs) const;
	bool operator>(const GameVersion& rhs) const;
	bool operator<(const GameVersion& rhs) const;
	bool operator<=(const GameVersion& rhs) const;
	bool operator==(const GameVersion& rhs) const;
	bool operator!=(const GameVersion& rhs) const;

	friend std::ostream& operator<<(std::ostream&, const GameVersion& version);

  private:
	void registerKeys();
	int firstPart = 0;
	int secondPart = 0;
	int thirdPart = 0;
	int fourthPart = 0;
};

class GameVersion::Factory: commonItems::CTReParser
{
  public:
	Factory();
	GameVersion getVersion(std::istream& theStream);

  private:
	int firstPart = 0;
	int secondPart = 0;
	int thirdPart = 0;
	int fourthPart = 0;
};

std::ostream& operator<<(std::ostream&, const GameVersion& version);

#endif // GAME_VERSION_H