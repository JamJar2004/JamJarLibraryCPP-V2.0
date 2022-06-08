#pragma once

#include "Array.hpp"
#include "Buffer.hpp"
#include "../Collections/Collection.hpp"

template<typename T>
class Queue : public IResizableCollection<T>
{
private:
	BufferRef<T> m_buffer;
	UnsafeRef<T> m_back;
	UnsafeRef<T> m_front;

	void ReAllocate(Size extraCount);
public:
	Queue(Size initialCapacity = 4U) : m_buffer(initialCapacity), m_back(m_buffer.Last()), m_front(m_buffer.Last()) {}

	~Queue() { Clear(); }

	virtual Size Count() const override { return m_front - m_back; }

		  T& Front()       { return *(m_front - 1U); }
	const T& Front() const { return *(m_front - 1U); }

	template<typename... Args>
	void Push(Args&&... args) requires ConstructibleFrom<T, Args...>
	{
		--m_back;
		if(m_back == m_buffer.First())
			ReAllocate(1U);

		m_back.Initialize(args...);
	}

	void Pop()
	{
		--m_front;
		m_front.Destroy();
	}

	virtual void Clear() override
	{
		while(Count() > 0U)
			Pop();
	}

	virtual SharedRef<Iterator<T>> Start() override { return New<ArrayIterator<T>>(m_front.ToUnsafePointer() - 1U, true); }
	virtual SharedRef<Iterator<T>> End()   override { return New<ArrayIterator<T>>(m_back.ToUnsafePointer()  - 1U, true); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ArrayIterator<const T>>(m_front.ToUnsafePointer() - 1U, true); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ArrayIterator<const T>>(m_back.ToUnsafePointer()  - 1U, true); }
};

template<typename T>
inline void Queue<T>::ReAllocate(Size extraCount)
{
	Size count = Count();

	BufferRef<T> newBuffer = BufferRef<T>(m_buffer.Count() * 2U + extraCount);
	m_buffer.CopyTo(newBuffer.Last() - count , count);
	m_back   = newBuffer.Last() - count;
	m_front  = newBuffer.Last();
	m_buffer = newBuffer;
}
