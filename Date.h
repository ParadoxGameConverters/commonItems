#ifndef DATE_H_
#define DATE_H_



#include <iostream>
#include <memory>
#include <string>



class date
{
  public:
	date() = default;
	explicit date(const int year, const int month, const int day, const bool AUC): year(AUC ? convertAUCtoAD(year) : year), month(month), day(day) {}
	explicit date(const int year, const int month, const int day): date(year, month, day, false) {}
	explicit date(std::string init, bool AUC);
	explicit date(const std::string& init) : date(init, false) {}

	void increaseByMonths(int months);
	void subtractYears(int years);

	bool operator==(const date& rhs) const;
	bool operator!=(const date& rhs) const;
	bool operator<(const date& rhs) const;
	bool operator>(const date& rhs) const;
	bool operator<=(const date& rhs) const;
	bool operator>=(const date& rhs) const;

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