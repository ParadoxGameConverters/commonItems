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

	if (!in.starts_with('"') || !in.ends_with('"'))
	{
		return in;
	}

	return in.substr(1, length - 2);
}


std::string addQuotes(const std::string& in)
{
	if (in.empty())
	{
		return "\"\"";
	}

	if (in.starts_with('"') && in.ends_with('"'))
	{
		return in;
	}

	return std::string{'"' + in + '"'};
}

} // namespace commonItems