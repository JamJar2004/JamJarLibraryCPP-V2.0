#pragma once

#include "../../Numerics.hpp"

template<typename T>
class Stack
{
private:
	T* m_base;
	T* m_top;

	Size m_capacity;

	void ReAllocate();
public:
	Stack(Size initialCapacity = 4U) : m_base(malloc(sizeof(T) * initialCapacity.ToRawValue())), m_top(m_base), m_capacity(initialCapacity) {}

	Size Count() const { return Size(m_top - m_base); }

	      T& Last()       { return *m_top; }
	const T& Last() const { return *m_top; }

	void Push(const T& item);

	template<typename... Args>
	void Push(Args&&... args) requires ConstructibleFrom<T, Args...>;

	void Pop();

	void Clear();
};

template<typename T>
inline void Stack<T>::ReAllocate()
{
	Size count = Count();

	T* newBase = malloc(sizeof(T) * m_capacity.ToRawValue() * 2);
	memcpy(newBase, m_base, count.ToRawValue());
	m_base = newBase;
	m_top  = m_base + count.ToRawValue();
}

template<typename T>
inline void Stack<T>::Push(const T& item)
{
	if(Count() >= m_capacity)
		ReAllocate();

	new(m_top) T(item);
	++m_top;
}

template<typename T>
template<typename... Args>
inline void Stack<T>::Push(Args&&... args) requires ConstructibleFrom<T, Args...>
{

}

template<typename T>
inline void Stack<T>::Pop()
{

	--m_top;
}

template<typename T>
inline void Stack<T>::Clear()
{

}


