#pragma once

#include "Iterator.hpp"

template<typename T>
class ICollection : public IIterable<T>
{
public:
	virtual Size Count() const = 0;
};

template<typename T>
class IResizableCollection : public ICollection<T>
{
public:
	virtual void Clear() = 0;
};