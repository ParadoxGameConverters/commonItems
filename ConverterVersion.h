#ifndef VERSION_PARSER_H
#define VERSION_PARSER_H



#include "ConvenientParser.h"
#include <string_view>



namespace commonItems
{

struct ConverterVersion
{
	std::string name;
	std::string version;
	std::string descriptionLine;

	friend std::ostream& operator<<(std::ostream& output, const ConverterVersion& converterVersion);
};


class ConverterVersionParser: convenientParser
{
  public:
	explicit ConverterVersionParser();
	ConverterVersion importVersion(std::string_view filename);

  private:
	ConverterVersion converterVersion;
};


std::ostream& operator<<(std::ostream& output, const ConverterVersion& converterVersion);

} // namespace commonItems



#endif // VERSION_PARSER_H