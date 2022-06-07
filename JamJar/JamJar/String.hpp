#pragma once

#include "Data/Memory/Refs.hpp"
#include "Data/Memory/Array.hpp"

#include <string>

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

class String : public ICollection<const Character>
{
private:
	const ArraySpan<Character> m_chars;

	static ArrayRef<Character> FromCString(const char* cString);
	static ArrayRef<Character> FromWCString(const wchar_t* wcString);
	static ArrayRef<Character> FromChar(Character character, Size length);
	static ArrayRef<Character> FromCollection(const ICollection<Character>& items);
public:
	String() : String('\0', 0U) {}

	String(const char*     cString);
	String(const wchar_t* wcString);

	String(Character character, Size length);
	
	explicit String(const ICollection<Character>& chars);

	explicit String(const ArraySpan<Character>& chars);

	String(const String& other);

	virtual Size Count() const override { return Length(); }

	Size Length() const;

	const Character& operator[](Size index) const;

	ArrayRef<Character> ToCharacterArray() const;

	SInt64     IndexOf(const String& string, Size offset = 0U) const;
	SInt64 LastIndexOf(const String& string, Size offset = 0U) const;

	String Slice(Size index)              const;
	String Slice(Size index, Size length) const;

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

	virtual SharedRef<Iterator<const Character>> Start() override { return m_chars.Start(); }
	virtual SharedRef<Iterator<const Character>> End()   override { return m_chars.End();   }

	virtual SharedRef<Iterator<const Character>> Start() const override { return m_chars.Start(); }
	virtual SharedRef<Iterator<const Character>> End()   const override { return m_chars.End();   }
};

template<Printable T>
inline String operator+(const String& left, const T& right) { return left + right.ToString(); }

template<Printable T>
inline String operator+(const T& left, const String& right) { return left.ToString() + right; }

template<std::unsigned_integral T>
String UnsignedInteger<T>::ToString() const { return std::to_string(m_value).c_str(); }

template<std::signed_integral T>
String SignedInteger<T>::ToString() const { return std::to_string(m_value).c_str(); }

template<std::floating_point T>
String Float<T>::ToString() const { return std::to_string(m_value).c_str(); }
