#include "String.hpp"

//#include "Data/Collections/Lists/ArrayList.hpp"
#include "Data/Memory/Refs.hpp"

String Boolean::ToString() const { return m_value ? "True" : "False"; }

String Character::ToString() const { return String(m_value, 1U); }

SharedArrayRef<Character> String::FromCString(const char* cString)
{
	SharedArrayRef<Character> result = HeapArray<Character>(strlen(cString));
	for(Size i = 0U; i < result.Count(); i++)
		result[i] = cString[i.ToRawValue()];

	return result;
}

SharedArrayRef<Character> String::FromWCString(const wchar_t* wcString)
{
	SharedArrayRef<Character> result = HeapArray<Character>(wcslen(wcString));
	for(Size i = 0U; i < result.Count(); i++)
		result[i] = wcString[i.ToRawValue()];

	return result;
}

SharedArrayRef<Character> String::FromChar(Character character, Size length)
{
	SharedArrayRef<Character> result = HeapArray<Character>(length);
	result.AsSpan().Fill(character);
	return result;
}


String::String(const char* cString) : m_chars(FromCString(cString)) {}

String::String(const wchar_t* wcString) : m_chars(FromWCString(wcString)) {}

String::String(Character character, Size length) : m_chars(FromChar(character, length)) {}

String::String(const SharedArraySpan<Character>& chars) : m_chars(chars) {}

String::String(const String& other) : m_chars(other.m_chars) {}

const Character& String::operator[](Size index) const { return m_chars[index]; }

SharedArrayRef<Character> String::ToCharacterArray() const { return m_chars.ToArray(); }

Nullable<Size> String::IndexOf(const String& string, Size offset) const
{
	for(Size i = offset; i < Length() - string.Length(); i++)
	{
		if(Slice(i, string.Length()) == string)
			return i;
	}

	return nullptr;
}

Nullable<Size> String::LastIndexOf(const String& string, Size offset) const
{
	for(Size i = offset - string.Length(); i < Length(); i--)
	{
		if(Slice(i, string.Length()) == string)
			return i;
	}

	return nullptr;
}

String String::Slice(Size index) const { return Slice(index, Length() - index); }

String String::Slice(Size index, Size length) const { return String(m_chars.Slice(index, length)); }

//SharedArrayRef<String> String::Split(const String& splitter) const
//{
//	ArrayList<String> resultList;
//
//	if(splitter.Length() == 0U)
//		return resultList.ToArray();
//
//	Size lastIndex = 0U;
//	for(Size i = 0U; i < Length() - splitter.Length(); i++)
//	{
//		if(Slice(i, splitter.Length()) == splitter)
//		{
//			if(lastIndex != i)
//				resultList.Add(Slice(lastIndex, i - lastIndex));
//
//			lastIndex = i + splitter.Length();
//		}
//	}
//
//	Size leftLength = Length() - lastIndex;
//
//	if(leftLength > 0U)
//		resultList.Add(Slice(lastIndex, leftLength));
//
//	return resultList.ToArray();
//}

String String::Replace(const String& oldString, const String& newString) const
{
	MutableString resultBuilder;

	if(oldString.Length() == 0U)
		return *this;

	Size lastIndex = 0U;
	for(Size i = 0U; i < Length() - oldString.Length(); i++)
	{
		if(Slice(i, oldString.Length()) == oldString)
		{
			if(lastIndex != i)
				resultBuilder += MutableString(Slice(lastIndex, i - lastIndex)) += newString;

			lastIndex = i + oldString.Length();
		}
	}

	Size leftLength = Length() - lastIndex;

	if(leftLength > 0U)
		resultBuilder += Slice(lastIndex, leftLength);

	return resultBuilder.ToString();
}

Boolean String::Contains(const String& string) const { return IndexOf(string) != nullptr; }

Boolean String::StartsWith(const String& string) const { return Slice(0U                        , string.Length()) == string; }
Boolean String::EndsWith(const String& string)   const { return Slice(Length() - string.Length(), string.Length()) == string; }

String String::TrimStart() const
{
	Size i;
	for(i = 0U; m_chars[i].IsWhiteSpace() && i < Length(); i++);
	return Slice(i);
}

String String::TrimEnd() const
{
	Size i;
	for(i = Length(); m_chars[i].IsWhiteSpace() && i < Length(); i--);
	return Slice(0U, i);
}

String String::Trim() const { return TrimStart().TrimEnd(); }

void String::CopyTo(char* cString) const
{
	for(Size i = 0U; i < Length(); i++)
		cString[i.ToRawValue()] = (char)m_chars[i].m_value;

	cString[Length().ToRawValue()] = '\0';
}

void String::CopyTo(wchar_t* wcString) const
{
	for(Size i = 0U; i < Length(); i++)
		wcString[i.ToRawValue()] = m_chars[i].m_value;

	wcString[Length().ToRawValue()] = '\0';
}

MutableString String::ToMutableString() const { return MutableString(*this); }

String operator+(const String& left, const String& right)
{
	SharedArrayRef<Character> resultChars = HeapArray<Character>(left.Length() + right.Length());
	left.m_chars.CopyTo(resultChars.AsSpan().Slice(0U, left.Length()));
	right.m_chars.CopyTo(resultChars.AsSpan(left.Length(), right.Length()));
	return String(resultChars);
}

Boolean operator==(const String& left, const String& right) { return left.m_chars == right.m_chars; }
Boolean operator!=(const String& left, const String& right) { return left.m_chars != right.m_chars; }

MutableString operator+(const MutableString& left, const MutableString& right)
{
	MutableString result = left;
	result.Append(right);
	return result;
}

Boolean operator==(const MutableString& left, const MutableString& right) { return left.m_chars.AsSpan() == right.m_chars.AsSpan(); }
Boolean operator!=(const MutableString& left, const MutableString& right) { return left.m_chars.AsSpan() == right.m_chars.AsSpan(); }

MutableString::MutableString(const char* cString) : m_chars(String::FromCString(cString)), m_length(m_chars.Count()) {}

MutableString::MutableString(const wchar_t* wcString) : m_chars(String::FromWCString(wcString)), m_length(m_chars.Count()) {}

MutableString::MutableString(Character character, Size length) : m_chars(String::FromChar(character, length)), m_length(length) {}

MutableString::MutableString(const MutableString& other) : m_chars(other.Length()), m_length(other.Length())
{
	other.m_chars.AsSpan().CopyTo(m_chars.AsSpan(0U, other.Length()));
}

MutableString::MutableString(MutableString&& other) noexcept : m_chars(other.m_chars), m_length(other.m_length) {}

MutableString& MutableString::operator=(const MutableString& other)
{
	m_chars = HeapArray<Character>(other.Length());
	other.m_chars.AsSpan().CopyTo(m_chars.AsSpan());
	m_length = other.Length();
	return *this;
}

      Character& MutableString::operator[](Size index)       { return m_chars[index]; }
const Character& MutableString::operator[](Size index) const { return m_chars[index]; }

void MutableString::TrimEnd(Size length) { m_length -= length; }

MutableString& MutableString::Append(const MutableString& other)
{
	if(m_length + other.Length() >= m_chars.Count())
	{
		SharedArrayRef<Character> newChars(m_chars.Count() * 2U + other.Length());
		m_chars.AsSpan().CopyTo(newChars.AsSpan().Slice(0U, m_length));
		m_chars = newChars;
	}

	other.m_chars.AsSpan().CopyTo(m_chars.AsSpan(m_length, other.Length()));
	m_length += other.Length();
	return *this;
}
