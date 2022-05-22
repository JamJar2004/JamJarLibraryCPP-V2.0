#pragma once

#include "Refs.hpp"

template<typename T>
class ArrayRef;

template<typename T>
class BufferRef
{
private:
	T*       m_buffer;
	Size     m_count;
	Size*    m_refCount;
	Boolean* m_isInitialized;

	void AddRef() { ++(*m_refCount); }

	Boolean RemRef() 
	{
		--(*m_refCount); 
		return *m_refCount == 0U;
	}

	void Delete()
	{
		free(m_buffer);
		delete m_refCount;
		delete m_isInitialized;
	}
public:
	BufferRef(Size count) : m_buffer((T*)malloc(sizeof(T) * count.ToRawValue())), m_count(count), m_refCount(new Size(1U)), m_isInitialized(new Boolean(false)) {}

	Size Count() const { return m_count; }

	BufferRef(const BufferRef<T>& other) : m_buffer(other.m_buffer), m_count(other.m_count), m_refCount(other.m_refCount), m_isInitialized(other.m_isInitialized)
	{
		AddRef();
	}

	~BufferRef()
	{
		if(RemRef())
			Delete();
	}

	BufferRef<T>& operator=(const BufferRef<T>& other) 
	{
		if(RemRef())
			Delete();

		m_buffer        = other.m_buffer;
		m_count         = other.m_count;
		m_refCount      = other.m_refCount;
		m_isInitialized = other.m_isInitialized;

		AddRef();
	}

	friend class ArrayRef<T>;
};