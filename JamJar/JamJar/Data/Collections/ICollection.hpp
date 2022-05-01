#pragma once

#include "IIterator.hpp"

template<typename T>
class ICollection : public IIterable<T>
{
public:
	virtual Size Count() const = 0;

	virtual Boolean Contains(const T& item) const requires Equatable<T> = 0;
};

template<typename T>
class IArray : public ICollection<T>
{
public:
	virtual       T& operator[](Size index)       = 0;
	virtual const T& operator[](Size index) const = 0;

	virtual NullableRef<Size> IndexOf(const T& item) const requires Equatable<T> = 0;

	virtual SharedRef<IArray<T>> Slice(Size index, Size count) const = 0;

	virtual void CopyTo(const IArray<T>& dest, Size sourceIndex, Size destIndex, Size count) const = 0;

	virtual void Fill(const T& item) = 0;
};

template<typename T>
class IResizableCollection : public ICollection<T>
{
public:
	virtual Boolean Add(const T& item)    = 0;
	virtual Boolean Remove(const T& item) = 0;
	
	virtual void Clear() = 0;

	virtual Size AddRange(const ICollection<T>& items) = 0;
};

template<typename T>
class IList : public IResizableCollection<T>, public IArray<T>
{
public:
	virtual void Insert(Size index) = 0;
	virtual void InsertRange(Size index, const ICollection<T>& items) = 0;

	virtual void RemoveAt(Size index) = 0;
	virtual void RemoveRange(Size index, Size count) = 0;
};

template<typename T>
class ISet : public IResizableCollection<const T>
{
public:
	virtual Size RemoveRange(const ICollection<T>& items) = 0;
};
