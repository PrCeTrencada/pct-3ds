#pragma once

#ifndef DATE_HPP
#define DATE_HPP

#include <stdlib.h>
#include <time.h>

#include <3ds/types.h>

class Date
{
public:
	Date();
	
	char* getWeekDayName() { return (char*)m_weekDays[getDayOfWeek(m_day, m_month, m_year)]; }
	char* getMonthName() { return (char*)m_months[m_month]; }
	
	int getHour() { return m_hour; }
	int getMinute() { return m_minute; }
	int getSecond() { return m_second; }
	
	int getDay() { return m_day; }
	int getMonth() { return m_month; }
	int getYear() { return m_year; }
	
private:
	bool isLeapYear(int year) { return (year % 4) == 0 && !((year % 100) == 0 && (year % 400) != 0); }
	int getDayOfWeek(int day, int month, int year);

	int m_hour, m_minute, m_second;
	int m_day, m_month, m_year;
	
	time_t unixTime;
	struct tm* timeStruct;
	
	const char* const m_months[12] = { "Gener", "Febrer", "Març", "Abril", "Maig", "Juny", "Juliol", "Agost", "Setembre", "Octubre", "Novembre", "Desembre" };

	const char* const m_weekDays[7] = { "Diumenge", "Dilluns", "Dimarts", "Dimecres", "Dijous", "Divendres", "Dissabte" };

	const u16 m_daysAtStartOfMonthLUT[12] =
	{
		0 % 7, //january    31
		31 % 7, //february   28+1(leap year)
		59 % 7, //march      31
		90 % 7, //april      30
		120 % 7, //may        31
		151 % 7, //june       30
		181 % 7, //july       31
		212 % 7, //august     31
		243 % 7, //september  30
		273 % 7, //october    31
		304 % 7, //november   30
		334 % 7  //december   31
	};
};

#endif // DATE_HPP