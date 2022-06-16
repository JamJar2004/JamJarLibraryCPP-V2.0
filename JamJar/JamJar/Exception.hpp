#pragma once

#include "String.hpp"
#include "Data/Memory/Refs.hpp"

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

template<typename T>
inline NullableRef<T>::operator SharedRef<T>() const
{
	if(!m_address)
		NullReferenceException().Throw();

	return SharedRef<T>(m_address, m_refCount);
}

template<typename T>
T& NullableRef<T>::operator*() const
{
	if(!m_address)
		NullReferenceException().Throw();

	return *m_address;
}

template<typename T>
T* const NullableRef<T>::operator->() const
{
	if(!m_address)
		NullReferenceException().Throw();

	return m_address;
}