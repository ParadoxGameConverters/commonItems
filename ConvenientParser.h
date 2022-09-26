#ifndef CONVENIENT_PARSER_H
#define CONVENIENT_PARSER_H



// A parser that allows simple keyword to variable matches



#include "Parser.h"



namespace commonItems
{

class convenientParser: public parser
{
  public:
	void registerSetter(const std::string& keyword, std::string& targetString);
	void registerSetter(const std::string& keyword, int& targetInt);
	void registerSetter(const std::string& keyword, double& targetDouble);
	void registerSetter(const std::string& keyword, long long& targetLlong);
	void registerSetter(const std::string& keyword, unsigned long long& targetULlong);
};

} // namespace commonItems



#endif // CONVENIENT_PARSER_H