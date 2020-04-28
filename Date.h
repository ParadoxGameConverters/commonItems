#ifndef DATE_H_
#define DATE_H_



#include <iostream>
#include <memory>
#include <string>



class date
{
  public:
	date() = default;
	explicit date(int year, int month, int day): year(year), month(month), day(day) {}
	explicit date(std::string init);

	void increaseByMonths(int months);
	void subtractYears(int years);

	bool operator==(const date& rhs) const;
	bool operator!=(const date& rhs) const;
	bool operator<(const date& rhs) const;
	bool operator>(const date& rhs) const;
	bool operator<=(const date& rhs) const;
	bool operator>=(const date& rhs) const;

	[[nodiscard]] float diffInYears(const date& rhs) const;
	[[nodiscard]] bool isSet() const;
	[[nodiscard]] std::string toString() const;

	friend std::ostream& operator<<(std::ostream&, const date&);

  private:
	[[nodiscard]] int calculateDayInYear() const;

	int year = 1;
	int month = 1;
	int day = 1;
};



#endif // _DATE_H