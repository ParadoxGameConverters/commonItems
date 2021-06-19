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
	bool isLargerishThan(const GameVersion& rhs) const;

	[[nodiscard]] std::string toString() const;
	[[nodiscard]] std::string toShortString() const;

	friend std::ostream& operator<<(std::ostream&, const GameVersion& version);

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