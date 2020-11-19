#ifndef CTRE_PARSER_H
#define CTRE_PARSER_H

#include "Parser.h"

namespace commonItems
{
class CTReParser: public parser
{
  public:
	enum class ctRegex
	{
		CATCHALL, NUMBER, QUOTED_NUMBER
	};

	void registerCTRegex(ctRegex regexEnum, const parsingFunction& function){
		parser::registerRegex(static_cast<int>(regexEnum), function);
	}
	[[nodiscard]] bool matchCTRegex(const ctRegex regexId, const std::string_view subject) const
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
	}

  protected:
	// compile time regexes, cool stuff
	static constexpr ctll::fixed_string number{R"(\d+)"};
	static constexpr ctll::fixed_string quotedNumber{R"("\d+")"};
	[[nodiscard]] constexpr bool numberMatch(std::string_view sv) const noexcept { return ctre::match<number>(sv); }
	[[nodiscard]] constexpr bool quotedNumberMatch(std::string_view sv) const noexcept { return ctre::match<quotedNumber>(sv); }
};
} // namespace commonItems

#endif
