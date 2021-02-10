#include "ConverterVersion.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include <ostream>



commonItems::ConverterVersionParser::ConverterVersionParser()
{
	registerSetter("name", converterVersion.name);
	registerSetter("version", converterVersion.version);
	registerSetter("descriptionLine", converterVersion.descriptionLine);
	registerMatcher(catchallRegexMatch, ignoreItem);
}


commonItems::ConverterVersion commonItems::ConverterVersionParser::importVersion(std::string_view filename)
{
	parseFile(filename);
	return converterVersion;
}


std::ostream& commonItems::operator<<(std::ostream& output, const ConverterVersion& converterVersion)
{
	output << "Converter version " << converterVersion.version << " \"" << converterVersion.name << "\" - "
			 << converterVersion.descriptionLine;
	return output;
}