#pragma once

#include "String.hpp"

class Console
{
public:
	template<Printable T>
	static void Print(const T& value);

	template<Printable T>
	static void PrintLine(const T& value);
};