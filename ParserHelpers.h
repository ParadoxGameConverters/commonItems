#ifndef PARSER_HELPERS_H
#define PARSER_HELPERS_H


#include <map>


#include "newParser.h"



namespace commonItems
{

void absorbBOM(std::istream& theStream);

void ignoreItem(const std::string& unused, std::istream& theStream);
void ignoreObject(const std::string& unused, std::istream& theStream);
void ignoreString(const std::string& unused, std::istream& theStream);
std::string singleItem(const std::string& unused, std::istream& theStream);


class intList: commonItems::parser
{
	public:
		explicit intList(std::istream& theStream);

		std::vector<int> getInts() const { return ints; }

	private:
		std::vector<int> ints;
};


class singleInt: commonItems::parser
{
	public:
		explicit singleInt(std::istream& theStream);

		int getInt() const noexcept { return theInt; }

	private:
		int theInt;
};

// Parses an object where each entry is a simple assignment, key = value.
// Nested objects, key = { ... }, are ignored.
class simpleObject : commonItems::parser
{
        public:
                explicit simpleObject(std::istream& theStream);

                std::string getValue(const std::string& key) const;
                int getValueAsInt(const std::string& key) const;

        private:
                std::map<std::string, std::string> values;
};

class doubleList: commonItems::parser
{
	public:
		explicit doubleList(std::istream& theStream);

		std::vector<double> getDoubles() const { return doubles; }

	private:
		std::vector<double> doubles;
};


class singleDouble: commonItems::parser
{
	public:
		explicit singleDouble(std::istream& theStream);

		double getDouble() const noexcept { return theDouble; }

	private:
		double theDouble;
};


class stringList: commonItems::parser
{
	public:
		explicit stringList(std::istream& theStream);

		std::vector<std::string> getStrings() const { return strings; }

	private:
		std::vector<std::string> strings;
};


class singleString: commonItems::parser
{
	public:
		explicit singleString(std::istream& theStream);

		std::string getString() const { return theString; }

	private:
		std::string theString;
};


class stringOfObject: commonItems::parser
{
	public:
		explicit stringOfObject(std::istream& theStream);

		std::string getString() const { return theString; }

	private:
		std::string theString;
};


class stringOfItem: commonItems::parser
{
	public:
		explicit stringOfItem(std::istream& theStream);

		std::string getString() const { return theString; }

	private:
		std::string theString;
};


class stringsOfItems: commonItems::parser
{
	public:
		explicit stringsOfItems(std::istream& theStream);

		std::vector<std::string> getStrings() const { return theStrings; }

	private:
		std::vector<std::string> theStrings;
};


class stringsOfItemNames: commonItems::parser
{
	public:
		explicit stringsOfItemNames(std::istream& theStream);

		std::vector<std::string> getStrings() const { return theStrings; }

	private:
		std::vector<std::string> theStrings;
};


class assignments: commonItems::parser
{
	public:
		assignments(std::istream& theStream);

		std::map<std::string, std::string> getAssignments() const { return theAssignments; }

	private:
		std::map<std::string, std::string> theAssignments;
};

}



#endif // PARSER_HELPERS_H
