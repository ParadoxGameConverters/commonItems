#ifndef COMMON_REGEXES_H
#define COMMON_REGEXES_H

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

	//strings
	static constexpr ctll::fixed_string stringRe{R"([^\n^=^{^}^"]+)"};
	constexpr bool stringMatch(std::string_view sv) noexcept { return ctre::match<stringRe>(sv); }
	
	static constexpr ctll::fixed_string quotedStringRe{R"("[^\n^=^{^}^"]+")"};
	constexpr bool quotedStringMatch(std::string_view sv) noexcept { return ctre::match<quotedStringRe>(sv); }
	} // namespace commonItems

#endif
