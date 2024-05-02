#include "Date.h"
#include "OSCompatibilityLayer.h"
#include "StringUtils.h"
#include <array>
#include <vector>



namespace
{

const std::array days_by_month{
	 0,	// January
	 31,	// February
	 59,	// March
	 90,	// April
	 120, // May
	 151, // June
	 181, // July
	 212, // August
	 243, // September
	 273, // October
	 304, // November
	 334, // December
	 365, // End of year
};

} // namespace



int CommonItems::DaysInMonth(int month)
{
	return days_by_month[month] - days_by_month[month - 1];
}



date::date(std::string init, const bool AUC)
{
	init = commonItems::remQuotes(init);

	std::vector<std::string> dateElements = GetDateElementStrings(init);
	try
	{
		if (dateElements.size() >= 3)
		{
			year = std::stoi(dateElements[0]);
			month = std::stoi(dateElements[1]);
			day = std::stoi(dateElements[2]);
		}
		else if (dateElements.size() == 2)
		{
			year = std::stoi(dateElements[0]);
			month = std::stoi(dateElements[1]);
		}
		else if (dateElements.size() == 1)
		{
			year = std::stoi(dateElements[0]);
		}
		else
		{
			Log(LogLevel::Warning) << "Problem constructing date: at least a year should be provided!";
		}
	}
	catch (std::exception& e)
	{
		Log(LogLevel::Warning) << "Problem constructing date from string \"" + init + "\": " + e.what() + "!";
	}
	if (AUC)
	{
		year = convertAUCtoAD(year);
	}
}


void date::ChangeByDays(int days)
{
	auto current_day_in_year = calculateDayInYear() + days;
	while (current_day_in_year < 0)
	{
		--year;
		current_day_in_year += 365;
	}

	year += current_day_in_year / 365;
	current_day_in_year = current_day_in_year % 365;
	for (month = 0; current_day_in_year > days_by_month[month]; ++month)
	{
	}
	if (month > 0)
	{
		day = current_day_in_year - days_by_month[month - 1];
	}
	else
	{
		day = current_day_in_year;
	}
}


void date::ChangeByMonths(int months)
{
	int new_year = year;
	int new_month = month;

	new_year += months / 12;
	new_month += months % 12;
	if (new_month > 12)
	{
		++new_year;
		new_month -= 12;
	}
	else if (new_month < 1)
	{
		--new_year;
		new_month += 12;
	}

	year = new_year;
	month = new_month;
}



void date::increaseByMonths(const int months)
{
	ChangeByMonths(months);
}


int date::convertAUCtoAD(const int yearAUC)
{
	auto yearAD = yearAUC - 753;
	if (yearAD <= 0)
	{
		--yearAD;
	}
	return yearAD;
}


float date::diffInYears(const date& rhs) const
{
	auto years = static_cast<float>(year - rhs.year);
	years += static_cast<float>(calculateDayInYear() - rhs.calculateDayInYear()) / 365;

	return years;
}


std::string date::toString() const
{
	std::stringstream buf;
	buf << year << '.' << month << '.' << day;
	return buf.str();
}


std::ostream& operator<<(std::ostream& out, const date& date)
{
	out << date.year << '.' << date.month << '.' << date.day;
	return out;
}


int date::calculateDayInYear() const
{
	if (month >= 1 && month <= 12)
		return day + days_by_month[static_cast<size_t>(month) - 1];

	return day;
}


std::vector<std::string> GetDateElementStrings(const std::string& s)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, '.'))
	{
		tokens.push_back(token);
	}
	return tokens;
}