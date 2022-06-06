#include "Console.hpp"

void Console::Print(const String& value) 
{
	wchar_t* chars = new wchar_t[value.Length().ToRawValue() + 1U];
	for(Size i = 0U; i < value.Length(); i++)
		chars[i.ToRawValue()] = value[i].m_value;

	chars[value.Length().ToRawValue()] = '\0';
	std::wcout << chars;
	delete[] chars;
}

void Console::PrintLine(const String& value) 
{
	wchar_t* chars = new wchar_t[value.Length().ToRawValue() + 1U];
	for(Size i = 0U; i < value.Length(); i++)
		chars[i.ToRawValue()] = value[i].m_value;

	chars[value.Length().ToRawValue()] = '\0';
	std::wcout << chars << std::endl;
	delete[] chars;
}
