#pragma once

#include "../Memory/Refs.hpp"

template<typename T>
class Iterator
{
public:
	virtual void MoveNext() = 0;

	virtual T& Current() const = 0;

	virtual Boolean Equals(const Iterator<T>& other) const = 0;
};

class String;

template<typename T>
class IteratorStorage;

template<typename T>
class IIterable
{
public:
	virtual SharedRef<Iterator<T>> Start() = 0;
	virtual SharedRef<Iterator<T>> End()   = 0;

	virtual SharedRef<Iterator<const T>> Start() const = 0;
	virtual SharedRef<Iterator<const T>> End()   const = 0;

	IteratorStorage<T> begin() { return Start(); }
	IteratorStorage<T> end()   { return End();   }

	IteratorStorage<const T> begin() const { return Start(); }
	IteratorStorage<const T> end()   const { return End();   }

	operator IIterable<const T>() { return *this; }

	String ToString() const requires Printable<T>;
};

template<typename T>
class IteratorStorage
{
private:
	SharedRef<Iterator<T>> m_iterator;
public:
	IteratorStorage(const SharedRef<Iterator<T>>& iterator) : m_iterator(iterator) {}

	IteratorStorage<T>& operator++() { m_iterator->MoveNext(); return *this; }

	Boolean operator==(const IteratorStorage<T>& other) const { return  m_iterator->Equals(*other.m_iterator); }
	Boolean operator!=(const IteratorStorage<T>& other) const { return !m_iterator->Equals(*other.m_iterator); }

	      T& operator*()       { return m_iterator->Current(); }
	const T& operator*() const { return m_iterator->Current(); }
};
