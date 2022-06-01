#pragma once

#include "Data/Memory/Refs.hpp"
#include "Data/Memory/Array.hpp"

class Character
{
private:
	wchar_t m_value;
public:
	Character(wchar_t value = '\0') : m_value(value) {}

	Boolean IsLetter() const { return (m_value >= 'A' && m_value <= 'Z') || (m_value >= 'a' && m_value <= 'z'); }
	Boolean IsNumber() const { return (m_value >= '0' && m_value <= '9');                                       }

	Boolean IsWhiteSpace() const { return m_value == ' ' || m_value == '\t' || m_value == '\n' || m_value == '\r'; }

	String ToString() const;

	friend Boolean operator==(Character left, Character right) { return left.m_value == right.m_value; }
	friend Boolean operator!=(Character left, Character right) { return left.m_value != right.m_value; }

	friend class Console;
};


template<typename T>
concept Printable = requires(T obj)
{
	{ obj.ToString() } -> SameAs<String>;
};

class String
{
private:
	const ArraySpan<Character> m_chars;

	static ArrayRef<Character> FromCString(const char* cString);
	static ArrayRef<Character> FromWCString(const wchar_t* wcString);
	static ArrayRef<Character> FromChar(Character character, Size length);
	//static ArrayRef<Character> FromCollection(const ICollection<Character>& items);
public:
	String(const char*     cString);
	String(const wchar_t* wcString);

	String(Character character, Size length);
	
	//explicit String(const ICollection<Character>& chars);

	explicit String(const ArraySpan<Character>& chars);

	String(const String& other);

	Size Length() const;

	const Character& operator[](Size index) const;

	ArrayRef<Character> ToCharacterArray() const;

	SInt64     IndexOf(const String& string, Size offset = 0U) const;
	SInt64 LastIndexOf(const String& string, Size offset = 0U) const;

	String SubString(Size index)              const;
	String SubString(Size index, Size length) const;

	ArrayRef<String> Split(const String& splitter) const;

	Boolean Contains(const String& string) const;

	Boolean StartsWith(const String& string) const;
	Boolean   EndsWith(const String& string) const;

	String TrimStart() const;
	String TrimEnd()   const;
	String Trim()      const;

	friend String operator+(const String& left, const String& right);

	template<Printable T>
	friend String operator+(const String& left, const T& right);
	template<Printable T>
	friend String operator+(const T& left, const String& right);

	friend Boolean operator==(const String& left, const String& right);
	friend Boolean operator!=(const String& left, const String& right);

	String ToString() const { return *this; }
};



template<Printable T>
inline String operator+(const String& left, const T& right) { return left + right.ToString(); }

template<Printable T>
inline String operator+(const T& left, const String& right) { return left.ToString() + right; }
