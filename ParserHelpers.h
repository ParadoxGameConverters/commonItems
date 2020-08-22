#ifndef PARSER_HELPERS_H
#define PARSER_HELPERS_H


#include <map>


#include "Parser.h"



namespace commonItems
{

const std::string catchallRegex{R"([a-zA-Z0-9_\.]+)"};


void ignoreItem(const std::string& unused, std::istream& theStream);
void ignoreObject(const std::string& unused, std::istream& theStream);
void ignoreString(const std::string& unused, std::istream& theStream);
void ignoreRGB(const std::string& unused, std::istream& theStream);


std::string singleItem(const std::string& unused, std::istream& theStream);


class intList: parser
{
  public:
	explicit intList(std::istream& theStream);

	[[nodiscard]] std::vector<int> getInts() const { return integers; }

  private:
	std::vector<int> integers;
};


class singleInt: parser
{
  public:
	explicit singleInt(std::istream& theStream);

	[[nodiscard]] int getInt() const noexcept { return theInt; }

  private:
	int theInt = 0;
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

class singleRGB: parser // example of when it's needed: color2 = rgb { 100 7 0.5 }
{
  public:
  	singleRGB() = default;
	explicit singleRGB(std::istream& theStream);

	[[nodiscard]] std::vector<double> getDoubles() const { return doubles; }

  private:
	std::vector<double> doubles = {0, 0, 0};
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