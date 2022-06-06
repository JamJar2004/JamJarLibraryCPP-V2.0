#pragma once

#include "../Memory/Array.hpp"

template<typename T>
class IList : public IResizableCollection<T>, IArray<T>
{
public:
	virtual void Add     (const T& item)               = 0;
	virtual void AddRange(const ICollection<T>& items) = 0;

	virtual void Insert     (Size index, const T& item)               = 0;
	virtual void InsertRange(Size index, const ICollection<T>& items) = 0;

	virtual void RemoveAt   (Size index)             = 0;
	virtual void RemoveRange(Size index, Size count) = 0;

	virtual Boolean Remove(const T& item) = 0;

	virtual SharedRef<IList<T>> SubList(Size index, Size count) const = 0;
};
