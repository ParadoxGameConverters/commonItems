#ifndef CTRE_PARSER_H
#define CTRE_PARSER_H

#include "Parser.h"

namespace commonItems
{
// compile time regexes, cool stuff
static constexpr ctll::fixed_string number{R"([0-9]+)"};
constexpr bool numberMatch(std::string_view sv) noexcept
{
	return ctre::match<number>(sv);
}
static constexpr ctll::fixed_string quotedNumber{R"("\d+")"};
constexpr bool quotedNumberMatch(std::string_view sv) noexcept
{
	return ctre::match<quotedNumber>(sv);
}




	
class CTReParser: public parser
{
  public:
	enum class ctRegex
	{
		CATCHALL, NUMBER, QUOTED_NUMBER
	};

	void registerCTRegex(ctRegex regexEnum, const parsingFunction& function){
		
		addCTRegex(static_cast<int>(regexEnum), &numberMatch);
		registerRegex(static_cast<int>(regexEnum), function);
		
	}
	/*auto matchCTRegex(const ctRegex regexId, const std::string_view subject)
	{
		switch (regexId)
		{
			case ctRegex::CATCHALL:
				return catchallRegexMatch(subject);
			case ctRegex::NUMBER:
				return numberMatch(subject);
			case ctRegex::QUOTED_NUMBER:
				return quotedNumberMatch(subject);
			//default:
				//return false;
		}
	}*/

  protected:

};
} // namespace commonItems

#endif
