#pragma once

#include "../../Concepts.hpp"

template<typename I, typename T>
concept Iterator = Inherits<I, IIterator<T>> || (Equatable<T> && requires(I obj)
{
	{ ++obj } -> SameAs<I&>;
	{ --obj } -> SameAs<I&>;

	{ obj.Current() } -> SameAs<T&>;
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

	Boolean operator==(const IIterator<T>& other) const = 0;
	Boolean operator!=(const IIterator<T>& other) const = 0;
};

template<typename T>
class IteratorStorage
{

};