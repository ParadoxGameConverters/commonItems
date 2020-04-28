#include "StringUtils.h"



namespace stringutils
{

std::string remQuotes(const std::string& in)
{
	const auto length = in.size();

	if (length < 2)
	{
		return in;
	}

	if (in[0] != '"' || in[length - 1] != '"')
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

	if (in[0] == '"' && in[length - 1] == '"')
	{
		return in;
	}

	return std::string{'"' + in + '"'};
}

} // namespace stringutils