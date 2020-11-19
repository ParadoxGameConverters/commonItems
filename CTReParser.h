#ifndef CTRE_PARSER_H
#define CTRE_PARSER_H

#include "Parser.h"

namespace commonItems
{
// compile time regexes, cool stuff

	// numbers
	static constexpr ctll::fixed_string integerRe{R"([0-9]+)"};
	constexpr bool integerMatch(std::string_view sv) noexcept {return ctre::match<integerRe>(sv);}
	static constexpr ctll::fixed_string quotedIntegerRe{R"("\d+")"};
	constexpr bool quotedIntegerMatch(std::string_view sv) noexcept{return ctre::match<quotedIntegerRe>(sv);}
	static constexpr ctll::fixed_string floatRe {R"(-?\d+(.\d+)?)"};
	constexpr bool floatMatch(std::string_view sv) noexcept{return ctre::match<floatRe>(sv);}
	static constexpr ctll::fixed_string quotedFloatRe{R"("-?\d+(.\d+)?")"};
	constexpr bool quotedFloatMatch(std::string_view sv) noexcept{return ctre::match<quotedFloatRe>(sv);}

class CTReParser: public parser
{
  public:
	enum class ctRegex
	{
		CATCHALL, INTEGER, QUOTED_INTEGER, FLOAT, QUOTED_FLOAT
	};

	void registerRegex(ctRegex regexEnum, const parsingFunction& function){
		const auto regexID = static_cast<int>(regexEnum);
		addCTRegex(regexID, ctreMatchers[regexEnum]);
		parser::registerRegex(regexID, function);
	}

  protected:
	std::map<ctRegex, bool (*)(std::string_view)> ctreMatchers = {
		 {ctRegex::CATCHALL, &catchallRegexMatch},
		 {ctRegex::INTEGER, &integerMatch},
		 {ctRegex::QUOTED_INTEGER, &quotedIntegerMatch},
		 {ctRegex::FLOAT, &floatMatch},
		 {ctRegex::QUOTED_FLOAT, &quotedFloatMatch}
	};

};
} // namespace commonItems

#endif
