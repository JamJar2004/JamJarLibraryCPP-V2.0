#include "Console.hpp"

void Console::Print(const String& value) 
{
	wchar_t* chars = new wchar_t[value.Length().ToRawValue() + 1U];
	value.CopyTo(chars);
	std::wcout << chars;
	delete[] chars;
}

void Console::PrintLine(const String& value) 
{
	wchar_t* chars = new wchar_t[value.Length().ToRawValue() + 1U];
	value.CopyTo(chars);
	std::wcout << chars << std::endl;
	delete[] chars;
}


