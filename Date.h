#ifndef DATE_H_
#define DATE_H_



// A class representing a Paradox - style date.
//
// Construction
// * Default construction gives a date of 0001 - 01 - 01
// * Can directly specify year, month, day
// * Can directly specify year, month, day, and if this is an AUC(years after the founding of Rome, used in Imperator) format date or not
// *Can pass a paradox - style string specifying the date
// * Can pass a paradox - style string specifying the date, and if this is an AUC(years after the founding of Rome, used in Imperator) format date or not
//
// Comparison
// * Dates can be compared using all the standard comparators.Additionally, the difference between two dates(in years) can be found.
//
// Modification
// * Dates can be increased by months or years, and can be decreased by years.In all cases thse must be whole - number changes to months or years.
//
// Output
// * Dates can be output to a stream or converted to a string.



#include <string>



class date
{
  public:
	date() = default;
	explicit date(const int year, const int month, const int day, const bool AUC): year(AUC ? convertAUCtoAD(year) : year), month(month), day(day) {}
	explicit date(const int year, const int month, const int day): date(year, month, day, false) {}
	explicit date(std::string init, bool AUC);
	explicit date(const std::string& init): date(init, false) {}

	void increaseByMonths(int months);
	void addYears(const int years) { year += years; }
	void subtractYears(int years);

	auto operator<=>(const date& rhs) const = default;

	[[nodiscard]] auto getYear() const { return year; }
	[[nodiscard]] auto getMonth() const { return month; }
	[[nodiscard]] auto getDay() const { return day; }

	[[nodiscard]] float diffInYears(const date& rhs) const;
	[[nodiscard]] bool isSet() const;
	[[nodiscard]] std::string toString() const;

	friend std::ostream& operator<<(std::ostream&, const date&);

  private:
	[[nodiscard]] int calculateDayInYear() const;
	[[nodiscard]] static int convertAUCtoAD(int yearAUC);

	int year = 1;
	int month = 1;
	int day = 1;
};



#endif // _DATE_H