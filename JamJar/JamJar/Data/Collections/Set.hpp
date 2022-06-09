#pragma once

#include "Collection.hpp"

template<typename T>
class Set : public IResizableCollection<const T>
{
public:
	virtual Boolean    Add(const T& item) = 0;
	virtual Boolean Remove(const T& item) = 0;

	virtual Size    AddRange(const ICollection<T>& items) = 0;
	virtual Size RemoveRange(const ICollection<T>& items) = 0;
};