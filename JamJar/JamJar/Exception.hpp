#pragma once

#include "String.hpp"

class Exception
{
private:
	String m_message;
public:
	Exception(const String& message) : m_message(message) {}

	void Throw() const;
};

class NullReferenceException : public Exception
{
public:
	NullReferenceException() : Exception("Cannot dereference a null reference.") {}
};