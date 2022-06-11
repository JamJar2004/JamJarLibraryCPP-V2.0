#pragma once

#include <concepts>

class HashCode;

class String;

class Boolean
{
private:
	bool m_value;
public:
	template<std::same_as<bool> T>
	Boolean(T value = false) : m_value(value) {}

	operator bool() const { return m_value; }

	HashCode GetHashCode() const;

	String ToString() const;
};