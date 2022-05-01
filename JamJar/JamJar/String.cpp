#include "String.hpp"

#include "Data/Collections/Array.hpp"
#include "Data/Memory/Refs.hpp"

#include <string>

String::String(const char* cString) : m_chars(FromCString(cString)) {}

String::String(const wchar_t* wcString) : m_chars(FromWCString(wcString)) {}

String::String(Character character, Size length) : m_chars(FromChar(character, length)) {}

String::String(const ICollection<Character>& chars) : m_chars(FromCollection(chars)) {}

String::String(const Span<Character>& chars) : m_chars(chars) {}

String::String(const String& other) : m_chars(other.m_chars) {}

Size String::Length() const { return m_chars.Count(); }

SharedRef<IArray<Character>> String::ToCharacterArray() const { return m_chars.ToArray(); }

SInt64 String::IndexOf(const String& string, Size offset) const
{
	for(Size i = offset; i < Length() - string.Length(); i++)
	{
		if(SubString(i, string.Length()) == string)
			return SInt64(i);
	}

	return -1;
}

SInt64 String::LastIndexOf(const String& string, Size offset) const
{
	for(Size i = offset - string.Length(); i < Length(); i--)
	{
		if(SubString(i, string.Length()) == string)
			return SInt64(i);
	}

	return -1;
}

String String::SubString(Size index) const { return SubString(index, Length() - index); }

String String::SubString(Size index, Size length) const { return String(m_chars.Slice(index, length)); }

SharedRef<IArray<String>> String::Split(const String& splitter) const
{
	
}

Boolean String::Contains(const String& string) const
{
	return IndexOf(string) != -1;
}

Boolean String::StartsWith(const String& string) const { return SubString(0U                        , string.Length()) == string; }
Boolean String::EndsWith(const String& string)   const { return SubString(Length() - string.Length(), string.Length()) == string; }

String operator+(const String& left, const String& right)
{
	SharedRef<HeapArray<Character>> resultChars = New<HeapArray<Character>>(left.Length() + right.Length());
	left.m_chars.CopyTo(*resultChars, 0U, 0U, left.Length());
	right.m_chars.CopyTo(*resultChars, 0U, left.Length(), right.Length());
	return String(Span<Character>(resultChars));
}
