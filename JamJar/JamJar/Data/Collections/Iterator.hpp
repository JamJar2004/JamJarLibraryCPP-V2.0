#pragma once

#include "../Memory/Refs.hpp"

template<typename T>
class Iterator
{
public:
	virtual void MoveNext() = 0;

	virtual T& Current() = 0;

	virtual Boolean Equals(const Iterator<T>& other) const = 0;
};

template<typename T>
class IIterable
{
public:
	virtual SharedRef<Iterator<T>> Start() = 0;
	virtual SharedRef<Iterator<T>> End()   = 0;

	IteratorStorage begin() { return Start(); }
	IteratorStorage end()   { return End();   }
};

template<typename T>
class IteratorStorage
{
private:
	SharedRef<Iterator<T>> m_iterator;
public:
	IteratorStorage(const SharedRef<Iterator<T>>& iterator) : m_iterator(iterator) {}

	IteratorStorage<T>& operator++() { m_iterator->MoveNext(); return *this; }

	Boolean operator==(const IteratorStorage& other) const { return  m_iterator->Equals(other.m_iterator); }
	Boolean operator!=(const IteratorStorage& other) const { return !m_iterator->Equals(other.m_iterator); }
};
