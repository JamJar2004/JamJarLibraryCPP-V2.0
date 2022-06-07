#pragma once

#include "../Collections/Lists/ArrayList.hpp"
#include "../../String.hpp"

class StringBuilder
{
private:
	ArrayList<Character> m_chars;
public:
	StringBuilder() {}

	Size Length() const { return m_chars.Count(); }

	StringBuilder& operator<<(const String& string) { m_chars.AddRange(string.ToCharacterArray()); return *this; }

	void Remove(Size index, Size length) { m_chars.RemoveRange(index, length); }

	String ToString() const { return String(m_chars); }
};

template<typename T>
String IIterable<T>::ToString() const// requires Printable<T>
{
	StringBuilder resultBuilder;

	resultBuilder << "{";

	for(const T& item : *this)
		resultBuilder << item.ToString() << ", ";

	resultBuilder.Remove(resultBuilder.Length() - 2U, 2U);

	resultBuilder << "}";

	return resultBuilder.ToString();
}