#include "ConvenientParser.h"
#include "ParserHelpers.h"

void commonItems::convenientParser::registerSetter(const std::string& keyword, std::string& targetString)
{
	registerKeyword(keyword,
		 [&targetString](std::istream& theStream)
		 {
			 targetString = getString(theStream);
		 });
}

void commonItems::convenientParser::registerSetter(const std::string& keyword, int& targetInt)
{
	registerKeyword(keyword,
		 [&targetInt](std::istream& theStream)
		 {
			 targetInt = getInt(theStream);
		 });
}

void commonItems::convenientParser::registerSetter(const std::string& keyword, double& targetDouble)
{
	registerKeyword(keyword,
		 [&targetDouble](std::istream& theStream)
		 {
			 targetDouble = getDouble(theStream);
		 });
}

void commonItems::convenientParser::registerSetter(const std::string& keyword, long long int& targetLlong)
{
	registerKeyword(keyword,
		 [&targetLlong](std::istream& theStream)
		 {
			 targetLlong = getLlong(theStream);
		 });
}

void commonItems::convenientParser::registerSetter(const std::string& keyword, unsigned long long int& targetULlong)
{
	registerKeyword(keyword,
		 [&targetULlong](std::istream& theStream)
		 {
			 targetULlong = getULlong(theStream);
		 });
}