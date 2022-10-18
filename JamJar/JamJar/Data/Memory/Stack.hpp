#pragma once

#include "Array.hpp"
#include "Buffer.hpp"
#include "../Collections/Collection.hpp"

template<typename T>
class Stack : public IResizableCollection<T>
{
private:
	BufferRef<T> m_buffer;
	UnsafeRef<T> m_top;

	void ReAllocate(Size extraCount);
public:
	Stack(Size initialCapacity = 4U) : m_buffer(initialCapacity), m_top(m_buffer.First()) {}

	~Stack() { Clear(); }
	
	virtual Size Count() const override { return m_top - m_buffer.First(); }

	      T& Top()       { return *(m_top - 1U); }
	const T& Top() const { return *(m_top - 1U); }

	template<typename... Args>
	void Push(Args&&... args) requires ConstructibleFrom<T, Args...>
	{
		if(Count() + 1U >= m_buffer.Count())
			ReAllocate(1U);

		m_top.Initialize(args...);
		++m_top;
	}

	void Pop()
	{
		--m_top;
		m_top.Destroy();
	}

	virtual void Clear() override
	{
		while(Count() > 0U)
			Pop();
	}

	virtual SharedRef<IIterator<T>> Start() override { return New<ArrayIterator<T>>(m_buffer.First().ToUnsafePointer()); }
	virtual SharedRef<IIterator<T>> End()   override { return New<ArrayIterator<T>>(m_top.ToUnsafePointer());            }

	virtual SharedRef<IIterator<const T>> Start() const override { return New<ArrayIterator<const T>>(m_buffer.First().ToUnsafePointer()); }
	virtual SharedRef<IIterator<const T>> End()   const override { return New<ArrayIterator<const T>>(m_top.ToUnsafePointer());            }
};

template<typename T>
inline void Stack<T>::ReAllocate(Size extraCount)
{
	Size count = Count();

	BufferRef<T> newBuffer = BufferRef<T>(m_buffer.Count() * 2U + extraCount);
	m_buffer.CopyTo(newBuffer, count);
	m_top = newBuffer[count];
	m_buffer = newBuffer;
}
