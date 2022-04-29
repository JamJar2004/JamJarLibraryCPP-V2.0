#pragma once

class String;

class Boolean
{
private:
	bool m_value;
public:
	Boolean(bool value = false) : m_value(value) {}

	operator bool() const { return m_value; }

	String ToString() const;
};