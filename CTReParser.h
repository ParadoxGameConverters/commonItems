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
		CATCHALL, NUMBER, QUOTED_NUMBER//, DATE, QUOTED_DATE
	};

	void registerRegex(ctRegex regexEnum, const parsingFunction& function){
		parser::registerRegex(static_cast<unsigned short>(regexEnum), function);
	}
	[[nodiscard]] bool matchCTRegex(const ctRegex regexId, const std::string_view subject) const
	{
		switch (regexId)
		{
			case ctRegex::CATCHALL:
				return catchallRegexMatch(subject);
			case ctRegex::NUMBER:
				return numberMatch(subject);
			//case ctRegex::DATE:
				//return dateMatch(subject);
			default:
				return false;
		}
	}

  protected:
	// compile time regexes, cool stuff
	[[nodiscard]] constexpr bool numberMatch(std::string_view sv) const noexcept { return ctre::match<number>(sv); }
	[[nodiscard]] constexpr bool quotedNumberMatch(std::string_view sv) const noexcept { return ctre::match<quotedNumber>(sv); }
	//[[nodiscard]] constexpr bool dateMatch(std::string_view sv) const noexcept { return ctre::match<date>(sv); }
	//[[nodiscard]] constexpr bool quotedDateMatch(std::string_view sv) const noexcept { return ctre::match<quotedDate>(sv); }

  private:
	static constexpr ctll::fixed_string number{R"(\d+)"};
	static constexpr ctll::fixed_string quotedNumber{R"("\d+")"};
	//static constexpr ctll::fixed_string date{R"(\d+)"};
	//static constexpr ctll::fixed_string quotedDate{R"("\d+")"};
};
} // namespace commonItems

#endif CTRE_PARSER_H