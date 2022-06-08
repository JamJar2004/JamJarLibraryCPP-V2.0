#pragma once

#include "../Numerics.hpp"

class Duration
{
private:
	UInt64 m_nanoSeconds;
public:
	Duration(UInt64 nanoSeconds = 0U) : m_nanoSeconds(nanoSeconds) {}

	Duration& InitFromMilliSeconds(Float64 milliSeconds) { m_nanoSeconds = UInt64(milliSeconds * 1000000   ); return *this; }
	Duration& InitFromSeconds     (Float64 seconds     ) { m_nanoSeconds = UInt64(seconds      * 1000000000); return *this; }
	Duration& InitFromMinutes     (Float64 minutes     ) { return InitFromSeconds(minutes      * 60.0f     ); }
	Duration& InitFromHours       (Float64 hours       ) { return InitFromMinutes(hours        * 60.0f     ); }
	Duration& InitFromDays        (Float64 days        ) { return InitFromHours  (days         * 24.0f     ); }

	Float64 GetNanoSeconds()  const { return m_nanoSeconds;                       }
	Float64 GetMilliSeconds() const { return m_nanoSeconds / Float64(1000000);    }
	Float64 GetSeconds()      const { return m_nanoSeconds / Float64(1000000000); }
	Float64 GetMinutes()      const { return GetSeconds()  / 60.0f;               }
	Float64 GetHours()        const { return GetMinutes()  / 60.0f;               }
	Float64 GetDays()         const { return GetHours()    / 24.0f;               }

	friend Duration operator+(const Duration& left, const Duration& right) { return Duration(left.m_nanoSeconds + right.m_nanoSeconds); }
	friend Duration operator-(const Duration& left, const Duration& right) { return Duration(left.m_nanoSeconds - right.m_nanoSeconds); }
};
