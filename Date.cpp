#include "Date.h"
#include "OSCompatibilityLayer.h"
#include "StringUtils.h"
#include <array>



namespace
{

const std::array<int, 13> days_by_month{
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

}



int CommonItems::DaysInMonth(int month)
{
	return days_by_month[month] - days_by_month[month - 1];
}



date::date(std::string init, const bool AUC)
{
	if (init.length() < 1)
	{
		return;
	}

	init = commonItems::remQuotes(init);

	const auto first_dot = init.find_first_of('.');
	const auto last_dot = init.find_last_of('.');

	try
	{
		year = stoi(init.substr(0, first_dot));
		if (AUC)
			year = convertAUCtoAD(year);
		month = stoi(init.substr(first_dot + 1, last_dot - first_dot));
		day = stoi(init.substr(last_dot + 1, 2));
	}
	catch (const std::exception& e)
	{
		Log(LogLevel::Warning) << "Problem inputting date: " << e.what();
		year = 1;
		month = 1;
		day = 1;
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


bool date::isSet() const
{
	return *this != date{};
}


std::string date::toString() const
{
	std::stringstream buf;
	buf << year << '.' << month << '.' << day;
	return buf.str();
}


std::ostream& operator<<(std::ostream& out, const date& d)
{
	out << d.year << '.' << d.month << '.' << d.day;
	return out;
}


int date::calculateDayInYear() const
{
	if (month >= 1 && month <= 12)
		return day + days_by_month[static_cast<size_t>(month) - 1];
	else
		return day;
}