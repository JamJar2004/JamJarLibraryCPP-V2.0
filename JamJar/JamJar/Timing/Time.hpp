#pragma once

#include "../Numerics.hpp"

class Duration
{
private:
	UInt64 m_nanoSeconds;
public:
	Duration(UInt64 nanoSeconds = 0U) : m_nanoSeconds(nanoSeconds) {}

	Float64 GetNanoSeconds()  const { return m_nanoSeconds;                       }
	Float64 GetMilliSeconds() const { return m_nanoSeconds / Float64(1000000);    }
	Float64 GetSeconds()      const { return m_nanoSeconds / Float64(1000000000); }

	friend Duration operator+(const Duration& left, const Duration& right) { return Duration(left.m_nanoSeconds + right.m_nanoSeconds); }
	friend Duration operator-(const Duration& left, const Duration& right) { return Duration(left.m_nanoSeconds - right.m_nanoSeconds); }
};
