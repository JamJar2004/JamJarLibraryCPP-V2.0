#pragma once

#include "Boolean.hpp"

template<typename T>
class Nullable
{
private:
	Boolean m_hasValue;
	T       m_value;
public:
	Nullable(std::nullptr_t) : m_hasValue(false) {}

	Nullable(const T& value) : m_value(value), m_hasValue(true) {}

	Boolean IsNull() const { return !m_hasValue; }

	T& GetValue() const { return m_value; }

	friend Boolean operator==(const Nullable<T>& left, std::nullptr_t) { return  left.IsNull(); }
	friend Boolean operator!=(const Nullable<T>& left, std::nullptr_t) { return !left.IsNull(); }
};