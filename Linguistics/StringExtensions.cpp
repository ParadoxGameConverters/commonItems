#include "StringExtensions.h"



std::map<std::string, std::string> commonItems::GetAdjectiveRules()
{
	std::map<std::string, std::string> rules = {
#include "adjectives.txt"
	};

	return rules;
}