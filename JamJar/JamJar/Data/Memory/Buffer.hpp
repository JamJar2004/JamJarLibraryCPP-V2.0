#pragma once

#include "../../Numerics.hpp"

template<typename T>
class Buffer
{
private:
	T*   m_address;
	Size m_capacity;
	Size m_count;
public:
	Buffer(Size capacity) : m_address(malloc(sizeof(T) * capacity.ToRawValue())) {}

	~Buffer() 
	{
		Clear();
		free(m_address);
	}

	      T& operator[](Size index)       { return m_address[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_address[index.ToRawValue()]; }

	template<typename... Args>
	void Push(Args&&... args) requires ConstructibleFrom<T, Args...> 
	{
		new(m_address + m_count.ToRawValue()) T(args...);
		++m_count;
	}

	void Push(const T& value) requires CopyConstructible<T>
	{
		new(m_address + m_count.ToRawValue()) T(value);
		++m_count;
	}

	void PushRange(const T& value, Size count) requires CopyConstructible<T>
	{
		for(Size i = m_count; i < m_count + count; i++)
			new(m_address + i.ToRawValue()) T(value);

		m_count += count;
	}

	void Pop(Size count = 1U)
	{
		for(Size i = 0; i < count; i++)
			m_address[m_count.ToRawValue() - i.ToRawValue() - 1U].~T();

		m_count -= count;
	}

	void Clear()
	{
		for(Size i = 0; i < m_count; i++)
			m_address[i].~T();

		m_count = 0;
	}

	void Reallocate(Size newCapacity)
	{
		if(newCapacity < m_capacity)
			return;

		T* newAddress = malloc(sizeof(T) * newCapacity.ToRawValue());
		memcpy(newAddress, m_address, m_count);
		m_address = newAddress;
	}
};