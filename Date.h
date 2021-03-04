#ifndef DATE_H
#define DATE_H


#include <string>


namespace commonItems
{

class Date
{
  public:
	Date() = default;
	explicit Date(const int year, const int month, const int day, const bool AUC):
		 year(AUC ? convertAUCtoAD(year) : year), month(month), day(day)
	{
	}
	explicit Date(const int year, const int month, const int day): Date(year, month, day, false) {}
	explicit Date(std::string init, bool AUC);
	explicit Date(const std::string& init): Date(init, false) {}

	void increaseByMonths(int months);
	void addYears(const int years) { year += years; }
	void subtractYears(int years);

	auto operator<=>(const Date& rhs) const = default;

	[[nodiscard]] auto getYear() const { return year; }
	[[nodiscard]] auto getMonth() const { return month; }
	[[nodiscard]] auto getDay() const { return day; }

	[[nodiscard]] float diffInYears(const Date& rhs) const;
	[[nodiscard]] bool isSet() const;
	[[nodiscard]] std::string toString() const;

	friend std::ostream& operator<<(std::ostream&, const Date&);

  private:
	[[nodiscard]] int calculateDayInYear() const;
	[[nodiscard]] static int convertAUCtoAD(int yearAUC);

	int year = 1;
	int month = 1;
	int day = 1;
};

}


[[deprecated("Use commonItems::Date")]] typedef commonItems::Date date;


#endif // DATE_H