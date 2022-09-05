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
	int new_year = year;
	int new_month = month;
	int new_day = day;

	if (days > 0)
	{
		do
		{
			const auto current_month_index = new_month - 1;
			const auto current_day_in_year = days_by_month[current_month_index] + new_day + days;

			bool does_month_change;
			if (new_month < 12)
			{
				does_month_change = current_day_in_year > days_by_month[new_month];
			}
			else
			{
				does_month_change = current_day_in_year > 365;
			}

			if (does_month_change)
			{
				date temp_date(new_year, new_month, new_day);
				temp_date.ChangeByMonths(1);
				new_year = temp_date.year;
				new_month = temp_date.month;
				new_day = temp_date.day;

				const auto days_in_month = CommonItems::DaysInMonth(new_month);
				const auto days_forward = days_in_month - new_day + 1;
				days -= days_forward;
				new_day = 1;
			}
			else
			{
				new_day += days;
				days = 0;
			}
		} while (days > 0);
	}
	else if (days < 0)
	{
		do
		{
			const auto current_month_index = new_month - 1;
			const auto current_day_in_year = days_by_month[current_month_index] + new_day + days;

			bool does_month_change;
			if (new_month > 1)
			{
				does_month_change = current_day_in_year <= days_by_month[current_month_index];
			}
			else
			{
				does_month_change = current_day_in_year <= 0;
			}

			if (does_month_change)
			{
				date temp_date(new_year, new_month, new_day);
				temp_date.ChangeByMonths(-1);
				new_year = temp_date.year;
				new_month = temp_date.month;
				new_day = temp_date.day;

				const auto days_in_month = CommonItems::DaysInMonth(new_month);
				days += new_day;
				new_day = days_in_month;
			}
			else
			{
				new_day += days;
				days = 0;
			}
		} while (days < 0);
	}

	year = new_year;
	month = new_month;
	day = new_day;
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