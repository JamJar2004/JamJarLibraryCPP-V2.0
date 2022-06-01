#pragma once

#include <iostream>

#include "String.hpp"

class Console
{
public:
	template<Printable T>
	static void Print(const T& value);

	template<Printable T>
	static void PrintLine(const T& value);

	static void Print(const String& value);

	static void PrintLine(const String& value);
};

template<Printable T>
void Console::Print(const T& value) { Print(value.ToString()); }

template<Printable T>
void Console::PrintLine(const T& value) { Print(value.ToString()); }


