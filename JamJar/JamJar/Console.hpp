#pragma once

#include <iostream>

#include "String.hpp"

class Console
{
public:
	template<Printable T>
	static void Print(const T& value);
	static void Print(const String& value);

	template<std::unsigned_integral T>
	static void Print(T value);

	template<std::signed_integral T>
	static void Print(T value);

	template<std::floating_point T>
	static void Print(T value);
	

	template<Printable T>
	static void PrintLine(const T& value);
	static void PrintLine(const String& value);

	template<std::unsigned_integral T>
	static void PrintLine(T value);

	template<std::signed_integral T>
	static void PrintLine(T value);

	template<std::floating_point T>
	static void PrintLine(T value);
};

template<Printable T>
void Console::Print(const T& value) { Print(value.ToString()); }

template<Printable T>
void Console::PrintLine(const T& value) { PrintLine(value.ToString()); }

template<std::unsigned_integral T>
void Console::Print(T value) { std::cout << value; }

template<std::signed_integral T>
void Console::Print(T value) { std::cout << value; }

template<std::floating_point T>
void Console::Print(T value) { std::cout << value; }

template<std::unsigned_integral T>
void Console::PrintLine(T value) { std::cout << value << std::endl; }

template<std::signed_integral T>
void Console::PrintLine(T value) { std::cout << value << std::endl; }

template<std::floating_point T>
void Console::PrintLine(T value) { std::cout << value << std::endl; }

