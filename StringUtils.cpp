#include "StringUtils.h"



namespace commonItems
{


bool IsQuoted(const std::string& string)
{
	return string.starts_with('"') && string.ends_with('"');
}


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
	if (in.size() > 2 && in.starts_with('"') && in.ends_with('"'))
	{
		return in;
	}
	if (!in.starts_with('"') && !in.ends_with('"'))
	{
		return std::string{'"' + in + '"'};
	}
	return in;
}

} // namespace commonItems