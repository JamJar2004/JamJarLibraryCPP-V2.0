#pragma once

#include "../Memory/Refs.hpp"

template<typename I, typename T>
concept Iterator = Inherits<I, IIterator<T>> || (Equatable<T> && requires(I obj)
{
	{ ++obj } -> SameAs<I&>;
	{ --obj } -> SameAs<I&>;

	{ obj.Current() } -> SameAs<      T&>;
	{ obj.Current() } -> SameAs<const T&>;
});

template<typename T>
class IIterator
{
public:
	IIterator<T>& operator++() = 0;
	IIterator<T>& operator--() = 0;

	      T& Current()       = 0;
	const T& Current() const = 0;

	friend Boolean operator==(const IIterator<T>& left, const IIterator<T>& right) const = 0;
	friend Boolean operator!=(const IIterator<T>& left, const IIterator<T>& right) const = 0;
};

template<typename T>
class IteratorStorage
{
private:
	SharedRef<IIterator<T>> m_iterator;
public:
	IteratorStorage(const SharedRef<IIterator<T>>& iterator) : m_iterator(iterator) {}

	IteratorStorage<T>& operator++() { ++(*m_iterator); return *this; }
	IteratorStorage<T>& operator--() { --(*m_iterator); return *this; }

	      T& operator*()       { return m_iterator->Current(); }
	const T& operator*() const { return m_iterator->Current(); }

	Boolean operator==(const IteratorStorage<T>& other) const { return (*m_iterator) == (*other.m_iterator); }
	Boolean operator!=(const IteratorStorage<T>& other) const { return (*m_iterator) != (*other.m_iterator); }
};

template<typename T>
class IIterable
{
public:
	SharedRef<IIterator<T>> Start() = 0;
	SharedRef<IIterator<T>> End()   = 0;

	SharedRef<const IIterator<T>> Start() const = 0;
	SharedRef<const IIterator<T>> End()   const = 0;

	IteratorStorage<T> begin() { return IteratorStorage<T>(Start()); }
	IteratorStorage<T> end()   { return IteratorStorage<T>(End());   }

	IteratorStorage<T> begin() const { return IteratorStorage<T>(Start()); }
	IteratorStorage<T> end()   const { return IteratorStorage<T>(End());   }
};