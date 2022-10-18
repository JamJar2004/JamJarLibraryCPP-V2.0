#pragma once

#include "Boolean.hpp"

template<std::unsigned_integral T>
class UnsignedInteger;

class HashCode
{
private:
	size_t m_value;
public:
	HashCode(size_t value = 17U) : m_value(value) {}

	HashCode(UnsignedInteger<size_t> value);

	size_t GetValue() const { return m_value; }

	friend HashCode operator&(const HashCode& left, const HashCode& right)
	{
		size_t hash = 17U;
		hash = hash * 31 + left.m_value;
		hash = hash * 31 + right.m_value;
		return HashCode(hash);
	}

	HashCode& operator&=(const HashCode& other)
	{
		size_t hash = 17U;
		hash = hash * 31 + m_value;
		hash = hash * 31 + other.m_value;
		m_value = hash;
		return *this;
	}

	friend Boolean operator==(const HashCode& left, const HashCode& right) { return left.m_value == right.m_value; }
	friend Boolean operator!=(const HashCode& left, const HashCode& right) { return left.m_value != right.m_value; }
};

