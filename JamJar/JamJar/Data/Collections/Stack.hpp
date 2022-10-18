#pragma once

#include "../../Numerics.hpp"
#include "../Memory/Buffer.hpp"

template<typename T>
class Stack
{
private:
	UnsafeRef<T> m_start;
	UnsafeRef<T> m_end;
public:
	Stack(Size initialCapacity = 4U) : m_start()
	{
		
	}
};