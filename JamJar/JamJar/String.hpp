#pragma once

#include "Numerics.hpp"

class String;

class Character
{
private:
	const wchar_t m_value;
public:
	Character(wchar_t value = '\0') : m_value(value) {}

	Boolean IsLetter() const { return (m_value >= 'A' && m_value <= 'Z') || (m_value >= 'a' && m_value <= 'z'); }
	Boolean IsNumber() const { return (m_value >= '0' && m_value <= '9');                                       }

	Boolean IsWhiteSpace() const { return m_value == ' ' || m_value == '\t' || m_value == '\n' || m_value == '\r'; }

	String ToString() const;
};


template<typename T>
concept Printable = requires(T obj)
{
	{ obj.ToString() } -> SameAs<String>;
};

class String
{
private:
	//const Span<Character> m_chars;
public:
	String(const char*     cString);
	String(const wchar_t* wcString);

	String(Character character, Size length);
	
	//String(const ICollection<Character>& chars);

	//String(const IArray<String> chars);

	String(const String& other);

	Size Length() const;

	//SharedRef<IArray<Character>> ToCharacterArray() const;

	SInt64     IndexOf(const String& string, Size offset = 0U) const;
	SInt64 LastIndexOf(const String& string, Size offset = 0U) const;

	String SubString(Size index)              const;
	String SubString(Size index, Size length) const;

	//SharedRef<IArray<String>> Split(const String& splitter) const;

	Boolean Contains(const String& string) const;

	Boolean StartsWith(const String& string) const;
	Boolean   EndsWith(const String& string) const;

	String TrimStart() const;
	String TrimEnd()   const;
	String Trim()      const;

	friend String operator+(const String& right, const String& left);

	template<Printable T>
	friend String operator+(const String& left, const T& right);
	template<Printable T>
	friend String operator+(const T& right, const String& left);

	friend Boolean operator==(const String& left, const String& right);
	friend Boolean operator!=(const String& left, const String& right);

	String ToString() const { return *this; }
};

String Boolean::ToString() const { return m_value ? "True" : "False"; }

String Character::ToString() const { return &m_value; }
