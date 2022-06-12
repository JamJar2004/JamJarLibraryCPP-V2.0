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

	HashCode GetHashCode() const { return HashCode(Size(m_value)); }

	String ToString() const;

	friend Boolean operator==(Character left, Character right) { return left.m_value == right.m_value; }
	friend Boolean operator!=(Character left, Character right) { return left.m_value != right.m_value; }

	friend class Console;
};

class MutableString;

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

	Size Length() const { return m_chars.Count(); }

	const Character& operator[](Size index) const;

	ArrayRef<Character> ToCharacterArray() const;

	SInt64     IndexOf(const String& string, Size offset = 0U) const;
	SInt64 LastIndexOf(const String& string, Size offset = 0U) const;

	String Slice(Size index)              const;
	String Slice(Size index, Size length) const;

	ArrayRef<String> Split(const String& splitter) const;

	String Replace(const String& oldString, const String& newString) const;

	Boolean Contains(const String& string) const;

	Boolean StartsWith(const String& string) const;
	Boolean   EndsWith(const String& string) const;

	String TrimStart() const;
	String TrimEnd()   const;
	String Trim()      const;

	const ArraySpan<Character> AsSpan() const { return m_chars; }

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

	friend class MutableString;
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

class StringBuilder
{
private:
	ArrayRef<Character> m_chars;
	Size                m_length;
public:
	StringBuilder(Size initialCapacity = 16U) : m_chars(initialCapacity) {}

	Size Length() const { return m_length; }

	StringBuilder& operator<<(const String& string) 
	{
		if(m_length + string.Length() >= m_chars.Count())
		{
			ArrayRef<Character> newChars(m_length * 2U + string.Length());
			m_chars.AsSpan().CopyTo(newChars, 0U, 0U, m_length);
			m_chars = newChars;
		}

		string.AsSpan().CopyTo(m_chars, 0U, m_length, string.Length());
		m_length += string.Length();

		return *this;
	}

	void Remove(Size index, Size length) { m_length -= length; }

	void Clear() { m_length = 0U; }

	String ToString() const { return String(ArraySpan<Character>(m_chars, 0U, m_length)); }
};

template<typename T>
String IIterable<T>::ToString() const requires Printable<T>
{
	StringBuilder resultBuilder;

	resultBuilder << "{";

	Boolean empty = true;

	for(const T& item : *this)
	{
		empty = false;
		resultBuilder << item.ToString() << ", ";
	}

	if(empty)
		return "<Empty>";
	
	resultBuilder.Remove(resultBuilder.Length() - 2U, 2U);

	resultBuilder << "}";

	return resultBuilder.ToString();
}

class MutableString : public ICollection<Character>
{
private:
	ArrayRef<Character> m_chars;
	Size                m_length;
public:
	MutableString() : m_chars(0U), m_length(0U) {}

	MutableString(const String& string) : m_chars(string.ToCharacterArray()), m_length(string.Length()) {}

	MutableString(const char*     cString);
	MutableString(const wchar_t* wcString);

	MutableString(Character character, Size length);

	MutableString(const MutableString& other);
	MutableString(MutableString&& other) noexcept;

	MutableString& operator=(const MutableString& other);

	virtual Size Count() const override { return Length(); }

	Size Length() const { return m_length; }

	operator String() const { return String(ArraySpan<Character>(m_chars, 0U, m_length)); }

 	      Character& operator[](Size index);
	const Character& operator[](Size index) const;

	String Slice(Size index)              const;
	String Slice(Size index, Size length) const;

	MutableString& Append(const MutableString& other);

	MutableString& operator+=(const MutableString& other) { return Append(other); }

	friend MutableString operator+(const MutableString& left, const MutableString& right);

	friend Boolean operator==(const MutableString& left, const MutableString& right);
	friend Boolean operator!=(const MutableString& left, const MutableString& right);

	virtual SharedRef<Iterator<Character>> Start() override { return m_chars.Start(); }
	virtual SharedRef<Iterator<Character>> End()   override { return m_chars.End();   }

	virtual SharedRef<Iterator<const Character>> Start() const override { return m_chars.Start(); }
	virtual SharedRef<Iterator<const Character>> End()   const override { return m_chars.End();   }

	String ToString() const { return String(m_chars.AsSpan()); }
};
