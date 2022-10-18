#pragma once

#include "../String.hpp"

class Duration
{
private:
	UInt64 m_ticks;
public:
	static Duration Zero;

	static Duration CreateFromNanoSeconds(Float32 nanoSeconds);
	static Duration CreateFromMilliSeconds(Float32 milliSeconds);
	static Duration CreateFromSeconds(Float32 seconds);
	static Duration CreateFromMinutes(Float32 minutes);
	static Duration CreateFromHours(Float32 hours);

	Duration() : m_ticks() {}

	UInt64 GetTicks() const { return m_ticks; }

	Float32 GetNanoSeconds()  const;
	Float32 GetMilliSeconds() const;
	Float32 GetSeconds()      const;
	Float32 GetMinutes()      const;
	Float32 GetHours()        const;

	friend Boolean operator==(const Duration& left, const Duration& right);
	friend Boolean operator!=(const Duration& left, const Duration& right);
};

enum class Month
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December,
};

enum class Day
{
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday,
};

class DateTime
{
private:
public:
	DateTime() {}

	UInt32 GetSecond() const;
	UInt32 GetMinute() const;
	UInt32 GetHour()   const;

	Day GetWeekDay() const;

	UInt32 GetDay()   const;
	Month  GetMonth() const;
	UInt32 GetYear()  const;

	DateTime operator+(const Duration& other) const;
	Duration operator-(const DateTime& other) const;
	
	friend Boolean operator==(const DateTime& left, const DateTime& right);
	friend Boolean operator!=(const DateTime& left, const DateTime& right);

	String ToString() const 
	{
		MutableString weekDay;
		switch(GetWeekDay())
		{
			case Day::Monday:
				weekDay = "Monday";
				break;
			case Day::Tuesday:
				weekDay = "Tuesday";
				break;
			case Day::Wednesday:
				weekDay = "Wednesday";
				break;
			case Day::Thursday:
				weekDay = "Thursday";
				break;
			case Day::Friday:
				weekDay = "Friday";
				break;
			case Day::Saturday:
				weekDay = "Saturday";
				break;
			case Day::Sunday:
				weekDay = "Sunday";
				break;
		}

		MutableString daySuffix;
		UInt32 day = GetDay();
		if(day == 1U)
			daySuffix = "st";
		else if(day == 2U)
			daySuffix = "nd";
		else if(day == 3U)
			daySuffix = "rd";
		else
			daySuffix = "th";

		MutableString month;
		switch(GetMonth())
		{
			case Month::January:
				month = "January";
				break;
			case Month::February:
				month = "February";
				break;
			case Month::March:
				month = "March";
				break;
			case Month::April:
				month = "April";
				break;
			case Month::May:
				month = "May";
				break;
			case Month::June:
				month = "June";
				break;
			case Month::July:
				month = "July";
				break;
			case Month::August:
				month = "August";
				break;
			case Month::September:
				month = "September";
				break;
			case Month::October:
				month = "October";
				break;
			case Month::November:
				month = "November";
				break;
			case Month::December:
				month = "December";
				break;
		}

		UInt32 hour   = GetHour();
		UInt32 minute = GetMinute();
		UInt32 second = GetSecond();

		String   hourStr =   GetHour() > 0U ?   hour.ToString() : "0" +   hour.ToString();
		String minuteStr = GetMinute() > 0U ? minute.ToString() : "0" + minute.ToString();
		String secondStr = GetSecond() > 0U ? second.ToString() : "0" + second.ToString();

		String date = "Date: " + day + daySuffix + " " + month + " " + GetYear();

		String time = "Time: " + hourStr + ":" + minuteStr + ":" + secondStr;

		return date + ", " + time;
	}
};

class Clock
{
public:
	static DateTime GetTime();
};
