#include "Date.hpp"

Date::Date()
{
	unixTime = time(NULL);
	timeStruct = gmtime((const time_t*)&unixTime);

	m_hour = timeStruct->tm_hour;
	m_minute = timeStruct->tm_min;
	m_second = timeStruct->tm_sec;
	
	m_day = timeStruct->tm_mday;
	m_month = timeStruct->tm_mon;
	m_year = timeStruct->tm_year + 1900;
}

int Date::getDayOfWeek(int day, int month, int year)
{
	//http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week
	day += 2 * (3 - ((year / 100) % 4));
	year %= 100;
	day += year + (year / 4);
	day += m_daysAtStartOfMonthLUT[month] - (isLeapYear(year) && (month <= 1));
	return day % 7;
}