#ifndef PARSER_HELPERS_H
#define PARSER_HELPERS_H
#include <map>
#include "Parser.h"


namespace commonItems
{

// const std::string catchallRegex{R"([a-zA-Z0-9_\.\-:@\'\"]+)"}; Leaving this here in case we need to revert.
// We grab everything that's NOT =, { or }, OR we grab everything within quotes, except newlines, which we already drop
// in the parser.
constexpr char catchallRegex[] = R"([^=^{^}]+|\".+\")";

void ignoreItem(const std::string& unused, std::istream& theStream);
void ignoreObject(const std::string& unused, std::istream& theStream);
void ignoreString(const std::string& unused, std::istream& theStream);

class intList: parser
{
  public:
	explicit intList(std::istream& theStream);

	[[nodiscard]] std::vector<int> getInts() const { return integers; }

  private:
	std::vector<int> integers;
};

class llongList: parser
{
  public:
	explicit llongList(std::istream& theStream);

	[[nodiscard]] std::vector<long long> getLlongs() const { return llongs; }

  private:
	std::vector<long long> llongs;
};

class ullongList: parser
{
  public:
	explicit ullongList(std::istream& theStream);

	[[nodiscard]] std::vector<unsigned long long> getULlongs() const { return ullongs; }

  private:
	std::vector<unsigned long long> ullongs;
};


class singleInt: parser
{
  public:
	explicit singleInt(std::istream& theStream);

	[[nodiscard]] int getInt() const noexcept { return theInt; }

  private:
	int theInt = 0;
};


class singleLlong: parser
{
  public:
	explicit singleLlong(std::istream& theStream);

	[[nodiscard]] long long getLlong() const noexcept { return theLongLong; }

  private:
	long long theLongLong = 0;
};

class singleULlong: parser
{
  public:
	explicit singleULlong(std::istream& theStream);

	[[nodiscard]] unsigned long long getULlong() const noexcept { return theUnsignedLongLong; }

  private:
	unsigned long long theUnsignedLongLong = 0;
};

// Parses an object where each entry is a simple assignment, key = value.
// Nested objects, key = { ... }, are ignored.
class simpleObject: parser
{
  public:
	explicit simpleObject(std::istream& theStream);

	[[nodiscard]] std::string getValue(const std::string& key) const;
	[[nodiscard]] int getValueAsInt(const std::string& key) const;

  private:
	std::map<std::string, std::string> values;
};

class doubleList: parser
{
  public:
	explicit doubleList(std::istream& theStream);

	[[nodiscard]] std::vector<double> getDoubles() const { return doubles; }

  private:
	std::vector<double> doubles;
};

class blobList: parser
{
  public:
	explicit blobList(std::istream& theStream);

	[[nodiscard]] std::vector<std::string> getBlobs() const { return blobs; }

  private:
	std::vector<std::string> blobs;
};


class singleDouble: parser
{
  public:
	explicit singleDouble(std::istream& theStream);

	[[nodiscard]] double getDouble() const noexcept { return theDouble; }

  private:
	double theDouble = 0.0;
};


class stringList: parser
{
  public:
	explicit stringList(std::istream& theStream);

	[[nodiscard]] std::vector<std::string> getStrings() const { return strings; }

  private:
	std::vector<std::string> strings;
};


class singleString: parser
{
  public:
	explicit singleString(std::istream& theStream);

	[[nodiscard]] std::string getString() const { return theString; }

  private:
	std::string theString;
};


class stringOfObject: parser
{
  public:
	explicit stringOfObject(std::istream& theStream);

	[[deprecated("Use stringOfItem")]] [[nodiscard]] std::string getString() const { return theString; }

  private:
	std::string theString;
};


class stringOfItem: parser
{
  public:
	explicit stringOfItem(std::istream& theStream);

	[[nodiscard]] std::string getString() const { return theString; }

  private:
	std::string theString;
};


class stringsOfItems: parser
{
  public:
	explicit stringsOfItems(std::istream& theStream);

	[[nodiscard]] std::vector<std::string> getStrings() const { return theStrings; }

  private:
	std::vector<std::string> theStrings;
};


class stringsOfItemNames: parser
{
  public:
	explicit stringsOfItemNames(std::istream& theStream);

	[[nodiscard]] std::vector<std::string> getStrings() const { return theStrings; }

  private:
	std::vector<std::string> theStrings;
};


class assignments: parser
{
  public:
	explicit assignments(std::istream& theStream);

	[[nodiscard]] std::map<std::string, std::string> getAssignments() const { return theAssignments; }

  private:
	std::map<std::string, std::string> theAssignments;
};

} // namespace commonItems



#endif // PARSER_HELPERS_H