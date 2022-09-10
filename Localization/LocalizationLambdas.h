#ifndef LOCALIZATION_LOCALIZATION_LAMBDAS_H
#define LOCALIZATION_LOCALIZATION_LAMBDAS_H



#include <functional>
#include <optional>
#include <string>



namespace commonItems
{

using LocalizationLambda = std::function<std::string(const std::string& base_localization, const std::string& language)>;
using TwoArgLocalizationLambda =
	 std::function<std::string(const std::string& base_localization, const std::optional<std::string>& modifying_localization, const std::string& language)>;

} // namespace commonItems



#endif // LOCALIZATION_LOCALIZATION_LAMBDAS_H