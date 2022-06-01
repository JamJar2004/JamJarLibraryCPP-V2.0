#include "String.hpp"

#include "Data/Collections/Lists/ArrayList.hpp"
#include "Data/Memory/Refs.hpp"

#include <string>

String Boolean::ToString() const { return m_value ? "True" : "False"; }

String Character::ToString() const { return String(m_value, 1U); }

ArrayRef<Character> String::FromCString(const char* cString)
{
	ArrayRef<Character> result = ArrayRef<Character>(strlen(cString));
	for (Size i = 0U; i < result.Count(); i++)
		result[i] = cString[i.ToRawValue()];

	return result;
}

ArrayRef<Character> String::FromWCString(const wchar_t* wcString)
{
	ArrayRef<Character> result = ArrayRef<Character>(wcslen(wcString));
	for (Size i = 0U; i < result.Count(); i++)
		result[i] = wcString[i.ToRawValue()];

	return result;
}

ArrayRef<Character> String::FromChar(Character character, Size length)
{
	ArrayRef<Character> result = ArrayRef<Character>(length);
	result.Fill(character);
	return result;
}

//ArrayRef<Character> String::FromCollection(const ICollection<Character>& items)
//{
//	ArrayRef<Character> result = NewArray<Character>((items.Count());
//	Size i = 0U;
//	for(Character c : items)
//		(*result)[i++] = c;
//
//	return result;
//}

String::String(const char* cString) : m_chars(FromCString(cString)) {}

String::String(const wchar_t* wcString) : m_chars(FromWCString(wcString)) {}

String::String(Character character, Size length) : m_chars(FromChar(character, length)) {}

//String::String(const ICollection<Character>& chars) : m_chars(FromCollection(chars)) {}

String::String(const ArraySpan<Character>& chars) : m_chars(chars) {}

String::String(const String& other) : m_chars(other.m_chars) {}

Size String::Length() const { return m_chars.Count(); }

const Character& String::operator[](Size index) const { return m_chars[index]; }

ArrayRef<Character> String::ToCharacterArray() const { return m_chars.ToArray(); }

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

ArrayRef<String> String::Split(const String& splitter) const
{
	ArrayList<String> resultList;

	if(splitter.Length() == 0U)
		return resultList.ToArray();

	Size lastIndex = 0U;
	for(Size i = 0U; i < Length() - splitter.Length(); i++)
	{
		if(SubString(i, splitter.Length()) == splitter)
		{
			if(lastIndex == i)
			{
				lastIndex = splitter.Length();
				i = lastIndex - 1U;
				continue;
			}
			resultList.Add(SubString(lastIndex, i - lastIndex));
		}
	}

	Size leftLength = Length() - lastIndex;

	if(leftLength > 0U)
		resultList.Add(SubString(lastIndex, leftLength));

	return resultList.ToArray();
}

Boolean String::Contains(const String& string) const { return IndexOf(string) != -1; }

Boolean String::StartsWith(const String& string) const { return SubString(0U                        , string.Length()) == string; }
Boolean String::EndsWith(const String& string)   const { return SubString(Length() - string.Length(), string.Length()) == string; }

String String::TrimStart() const
{
	Size i;
	for(i = 0U; m_chars[i].IsWhiteSpace() && i < Length(); i++);
	return SubString(i);
}

String String::TrimEnd() const
{
	Size i;
	for(i = Length(); m_chars[i].IsWhiteSpace() && i < Length(); i--);
	return SubString(0U, i);
}

String String::Trim() const { return TrimStart().TrimEnd(); }

String operator+(const String& left, const String& right)
{
	ArrayRef<Character> resultChars = ArrayRef<Character>(left.Length() + right.Length());
	left.m_chars.CopyTo(resultChars, 0U, 0U, left.Length());
	right.m_chars.CopyTo(resultChars, 0U, left.Length(), right.Length());
	return String(ArraySpan<Character>(resultChars));
}

Boolean operator==(const String& left, const String& right) { return left.m_chars == right.m_chars; }
Boolean operator!=(const String& left, const String& right) { return left.m_chars != right.m_chars; }
