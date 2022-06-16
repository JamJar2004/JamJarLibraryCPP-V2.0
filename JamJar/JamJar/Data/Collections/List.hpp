#pragma once

#include "../Memory/Array.hpp"

template<typename T>
class IList : public IResizableCollection<T>
{
public:
	virtual       T& operator[](Size index)       = 0;
	virtual const T& operator[](Size index) const = 0;

	virtual void Add     (const T& item)               = 0;
	virtual void AddRange(const ICollection<T>& items) = 0;

	virtual void Insert     (Size index, const T& item)               = 0;
	virtual void InsertRange(Size index, const ICollection<T>& items) = 0;

	virtual void RemoveAt   (Size index)             = 0;
	virtual void RemoveRange(Size index, Size count) = 0;
};
