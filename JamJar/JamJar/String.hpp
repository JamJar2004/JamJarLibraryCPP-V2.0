#pragma once

#include "Numerics.hpp"
#include "Data/Memory/Span.hpp"

class String;

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
};


template<typename T>
concept Printable = requires(T obj)
{
	{ obj.ToString() } -> SameAs<String>;
};

class String
{
private:
	const Span<Character> m_chars;

	static SharedRef<HeapArray<Character>> FromCString(const char* cString)
	{
		SharedRef<HeapArray<Character>> result = New<HeapArray<Character>>(strlen(cString));
		for(Size i = 0U; i < result->Count(); i++)
			(*result)[i] = cString[i.ToRawValue()];

		return result;
	}

	static SharedRef<HeapArray<Character>> FromWCString(const wchar_t* wcString)
	{
		SharedRef<HeapArray<Character>> result = New<HeapArray<Character>>(wcslen(wcString));
		for(Size i = 0U; i < result->Count(); i++)
			(*result)[i] = wcString[i.ToRawValue()];

		return result;
	}

	static SharedRef<HeapArray<Character>> FromChar(Character character, Size length)
	{
		SharedRef<HeapArray<Character>> result = New<HeapArray<Character>>(length);
		result->Fill(character);
		return result;
	}
	
	static SharedRef<HeapArray<Character>> FromCollection(const ICollection<Character>& items)
	{
		SharedRef<HeapArray<Character>> result = New<HeapArray<Character>>(items.Count());
		Size i = 0U;
		for(Character c : items)
			(*result)[i++] = c;

		return result;
	}
public:
	String(const char*     cString);
	String(const wchar_t* wcString);

	String(Character character, Size length);
	
	String(const ICollection<Character>& chars);

	String(const Span<Character>& chars);

	String(const String& other);

	Size Length() const;

	SharedRef<IArray<Character>> ToCharacterArray() const;

	SInt64     IndexOf(const String& string, Size offset = 0U) const;
	SInt64 LastIndexOf(const String& string, Size offset = 0U) const;

	String SubString(Size index)              const;
	String SubString(Size index, Size length) const;

	SharedRef<IArray<String>> Split(const String& splitter) const;

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

String Boolean::ToString() const { return m_value ? "True" : "False"; }

String Character::ToString() const { return &m_value; }

template<Printable T>
inline String operator+(const String& left, const T& right) { return left + right.ToString(); }

template<Printable T>
inline String operator+(const T& left, const String& right) { return left.ToString() + right; }
