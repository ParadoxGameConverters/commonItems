#include "StringUtils.h"



namespace commonItems
{

std::string remQuotes(const std::string& in)
{
	const auto length = in.size();

	if (length < 2)
	{
		return in;
	}

	if (in.at(0) != '"' || in.at(length - 1) != '"')
	{
		return in;
	}

	return in.substr(1, length - 2);
}


std::string addQuotes(const std::string& in)
{
	const auto length = in.size();

	if (length == 0)
	{
		return "\"\"";
	}

	if (in.at(0) == '"' && in.at(length - 1) == '"')
	{
		return in;
	}

	return std::string{'"' + in + '"'};
}

} // namespace commonItems