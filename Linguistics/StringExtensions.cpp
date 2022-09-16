#include "StringExtensions.h"



std::vector<std::pair<std::string, std::string>> commonItems::GetAdjectiveRules()
{
	std::vector<std::pair<std::string, std::string>> rules = {
#include "adjectives.txt"
	};

	return rules;
}