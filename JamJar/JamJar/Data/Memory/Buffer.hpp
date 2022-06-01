#pragma once

#include "Refs.hpp"

template<typename T>
class ArrayRef;

template<typename T>
class InitializedRange
{
private:
	T*   m_address;
	Size m_count;
public:
	InitializedRange(T* address) : m_address(address), m_count(0U) {}

      	  T& operator[](Size index)       { return m_address[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_address[index.ToRawValue()]; }

	void Add() requires DefaultConstructible<T>
	{
		new(m_address + m_count.ToRawValue()) T();
		++m_count;
	}

	void Add(const T& item) requires CopyConstructible<T>
	{
		new(m_address + m_count.ToRawValue()) T(item);
		++m_count;
	}

	void Remove(Size index)
	{
		T* location = m_address + index.ToRawValue();

		location->~T();
		if(index < m_count - 1U)
			memcpy(location, location + 1U, (m_count - 1U - index).ToRawValue());

		--m_count;
	}

	void Clear()
	{
		for(Size i = 0U; i < m_count; i++)
			(m_address + i.ToRawValue())->~T();

		m_count = 0U;
	}
};

template<typename T>
class BufferRef
{
private:
	T*                   m_address;
	Size*                m_refCount;
	InitializedRange<T>* m_initRange;
	Size                 m_count;

	void AddRef() { ++(*m_refCount) }

	void RemRef() 
	{
		--(*m_refCount);
		if(*m_refCount == 0U)
			free(m_address);
	}
public:
	BufferRef(Size count) : m_address((T*)malloc(sizeof(T) * count.ToRawValue())), m_refCount(new Size(1U)), m_count(count) {}

	BufferRef(const BufferRef<T>& other) : 
		m_address(other.m_address), m_refCount(other.m_refCount), m_count(other.m_count) { AddRef(); }

	~BufferRef() { RemRef(); }

	Size Count() const { return m_count; }

	BufferRef<T>& operator=(const BufferRef<T>& other) 
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		m_count    = other.m_count;
		AddRef();
	}

	InitializedRange<T>& GetInitializedRange()
	{
		if(!m_initRange)
			m_initRange = new InitializedRange<T>()
	}

	friend class ArrayRef<T>;
};
