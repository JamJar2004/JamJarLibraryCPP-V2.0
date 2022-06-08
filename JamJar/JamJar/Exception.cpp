#include "Exception.hpp"

#include "Console.hpp"

void Exception::Throw() const 
{
	Console::PrintLine(m_message);
	std::exit(1);
}