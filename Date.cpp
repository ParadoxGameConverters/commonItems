#include "Date.h"
#include "OSCompatibilityLayer.h"
#include <array>



date::date(std::string init, const bool AUC)
{
	if (init.length() < 1)
	{
		return;
	}

	if (init[0] == '\"')
	{
		init = init.substr(1, init.length() - 2);
	}
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


void date::increaseByMonths(const int months)
{
	year += months / 12;
	month += months % 12;
	if (month > 12)
	{
		year++;
		month -= 12;
	}
}


void date::subtractYears(const int years)
{
	year -= years;
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


bool date::operator==(const date& rhs) const
{
	return ((year == rhs.year) && (month == rhs.month) && (day == rhs.day));
}


bool date::operator!=(const date& rhs) const
{
	return !(*this == rhs);
}


bool date::operator<(const date& rhs) const
{
	return ((year < rhs.year) || ((year == rhs.year) && (month < rhs.month)) ||
			  ((year == rhs.year) && (month == rhs.month) && (day < rhs.day)));
}


bool date::operator>(const date& rhs) const
{
	return ((year > rhs.year) || ((year == rhs.year) && (month > rhs.month)) ||
			  ((year == rhs.year) && (month == rhs.month) && (day > rhs.day)));
}


bool date::operator<=(const date& rhs) const
{
	return ((*this == rhs) || (*this < rhs));
}


bool date::operator>=(const date& rhs) const
{
	return ((*this == rhs) || (*this > rhs));
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


const std::array<int, 12> daysByMonth{
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
	 334	// December
};
int date::calculateDayInYear() const
{
	if (month >= 1 && month <= 12)
		return day + daysByMonth[static_cast<size_t>(month) - 1];
	else
		return day;	
}