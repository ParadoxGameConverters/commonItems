#ifndef VERSION_H
#define VERSION_H

#include "Parser.h"
#include <ostream>
#include <string>

class Version: commonItems::parser
{
  public:
	class Factory;
	Version() = default;
	Version(const Version&) = default;
	Version(Version&&) = default;
	Version& operator=(const Version&) = default;
	Version& operator=(Version&&) = default;
	~Version() = default;

	explicit Version(const int firstPart, const int secondPart, const int thirdPart, const int fourthPart):
		 firstPart(firstPart), secondPart(secondPart), thirdPart(thirdPart), fourthPart(fourthPart)
	{
	}
	explicit Version(std::string version);
	explicit Version(std::istream& theStream);

	bool operator>=(const Version& rhs) const;
	bool operator>(const Version& rhs) const;
	bool operator<(const Version& rhs) const;
	bool operator<=(const Version& rhs) const;
	bool operator==(const Version& rhs) const;
	bool operator!=(const Version& rhs) const;

	friend std::ostream& operator<<(std::ostream&, const Version& version);

  private:
	int firstPart = 0;
	int secondPart = 0;
	int thirdPart = 0;
	int fourthPart = 0;
};


class Version::Factory: parser
{
  public:
	Factory();
	Version getVersion(std::istream& theStream);

  private:
	int firstPart = 0;
	int secondPart = 0;
	int thirdPart = 0;
	int fourthPart = 0;
};

std::ostream& operator<<(std::ostream&, const Version& version);

#endif // VERSION_H