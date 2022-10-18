#pragma once

#include "Refs.hpp"

#include "../Reflection.hpp"

class DynamicBufferRef;

class DynamicBuffer
{
private:
	void* m_address;
	Size* m_refCount;
	Size  m_count;

	const TypeInfo& m_elementType;
public:
	DynamicBuffer(Size count, const TypeInfo& type) :
		m_address(malloc((type.GetSize() * count).ToRawValue())), m_refCount(new Size(1U)), m_count(count), m_elementType(type) {}

	template<typename T>
	DynamicBuffer(const Buffer<T>& other) : 
		m_address((T*)malloc(sizeof(T) * other.Count().ToRawValue())), m_refCount(new Size(1U)), m_count(other.Count()), m_elementType(Reflect::GetType<T>())
	{
	}

	~DynamicBuffer()
	{
		Size& refCount = *m_refCount;
		--refCount;

		if(refCount == 0U)
			free(m_address);
	}

	Size Count() const { return m_count; }

	const TypeInfo& GetElementType() const { return m_elementType; }

	friend class DynamicBufferRef;
};

template<typename T>
class SharedBufferRef;

template<typename T>
class Buffer
{
private:
	T*    m_address;
	Size* m_refCount;
	Size  m_count;
public:
	Buffer(Size count) : m_address((T*)malloc(sizeof(T) * count.ToRawValue())), m_refCount(new Size(1U)), m_count(count) {}

	Buffer(const Buffer<T>& other) : m_address((T*)malloc(sizeof(T)* other.m_count.ToRawValue())), m_refCount(new Size(1U)), m_count(other.m_count) {}

	~Buffer()
	{
		Size& refCount = *m_refCount;
		--refCount;

		if(refCount == 0U)
			free(m_address);
	}

	Size Count() const { return m_count; }

	friend class SharedBufferRef<T>;
};

template<typename T>
class BufferSpan;

class DynamicBufferRef
{
private:
	void* m_address;
	Size* m_refCount;
	Size  m_count;

	const TypeInfo& m_elementType;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		Size& refCount = *m_refCount;
		--refCount;

		if(refCount == 0U)
		{
			delete[] m_address;
			delete   m_refCount;
		}
	}
public:
	DynamicBufferRef(const DynamicBuffer& buffer) :
		m_address(buffer.m_address), m_refCount(buffer.m_refCount), m_count(buffer.m_count), m_elementType(buffer.m_elementType) { AddRef(); }

	~DynamicBufferRef() { RemRef(); }

	const TypeInfo& GetElementType() const { return m_elementType; }

	Size Count() const { return m_count; }

	      DynamicBufferSpan AsSpan()       { return *this; }
	const DynamicBufferSpan AsSpan() const { return *this; }

		  DynamicBufferSpan AsSpan(Size index, Size count)       { return DynamicBufferSpan(*this, index, count); }
	const DynamicBufferSpan AsSpan(Size index, Size count) const { return DynamicBufferSpan(*this, index, count); }

	friend class DynamicBufferSpan;
};

template<typename T>
class SharedBufferRef
{
private:
	T*    m_address;
	Size* m_refCount;
	Size  m_count;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		Size& refCount = *m_refCount;
		--refCount;

		if(refCount == 0U)
		{
			delete[] m_address;
			delete   m_refCount;
		}
	}

public:
	SharedBufferRef(const Buffer<T>& buffer) : m_address(buffer.m_address), m_refCount(buffer.m_refCount), m_count(buffer.m_count) { AddRef(); }

	SharedBufferRef(const SharedBufferRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount), m_count(other.m_count) { AddRef(); }

	~SharedBufferRef() { RemRef(); }

	SharedBufferRef<T>& operator=(const SharedBufferRef<T>& other)
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		m_count    = other.m_count;
		AddRef();

		return *this;
	}

	Size Count() const { return m_count; }

	      BufferSpan<T> AsSpan()       { return *this; }
	const BufferSpan<T> AsSpan() const { return *this; }

		  BufferSpan<T> AsSpan(Size index, Size count)       { return BufferSpan<T>(*this, index, count); }
	const BufferSpan<T> AsSpan(Size index, Size count) const { return BufferSpan<T>(*this, index, count); }

	friend class BufferSpan<T>;
};

class DynamicBufferSpan
{
private:
	DynamicBufferRef m_buffer;
	Size             m_index;
	Size             m_count;
public:
	DynamicBufferSpan(const DynamicBufferRef& buffer) : m_buffer(buffer), m_index(0U), m_count(buffer.Count()) {}

	DynamicBufferSpan(const DynamicBufferRef& buffer, Size index, Size count) : m_buffer(buffer), m_index(index), m_count(count) {}

	Size Index() const { return m_index; }
	Size Count() const { return m_count; }

	DynamicBufferSpan Slice(Size index, Size count) const { return DynamicBufferSpan(m_buffer, index + m_index, count); }

	void CopyTo(DynamicBufferSpan destination) const 
	{ 
		Size typeSize = m_buffer.GetElementType().GetSize();

		Size   destIndex = destination.m_index * typeSize;
		Size sourceIndex = m_index * typeSize;

		UInt8*   destPtr = (UInt8*)(destination.m_buffer.m_address);
		UInt8* sourcePtr = (UInt8*)(m_buffer.m_address);

		Size destSize = destination.Count() * typeSize;

		memcpy(destPtr + destIndex.ToRawValue(), sourcePtr + sourceIndex.ToRawValue(), destSize.ToRawValue());
	}
};

template<typename T>
class BufferSpan
{
private:
	SharedBufferRef<T> m_buffer;
	Size               m_index;
	Size               m_count;
public:
	BufferSpan(const SharedBufferRef<T>& buffer) : BufferSpan(buffer, 0U, buffer.Count()) {}

	BufferSpan(const SharedBufferRef<T>& buffer, Size index, Size count) : m_buffer(buffer), m_index(index), m_count(count) {}

	Size Index() const { return m_index; }
	Size Count() const { return m_count; }

	BufferSpan<T> Slice(Size index, Size count) const { return BufferSpan<T>(m_buffer, index + m_index, count); }

	void Fill(UInt8 value) requires SameAs<T, UInt8> { memset(m_buffer.m_address, value.ToRawValue(), m_count.ToRawValue()) }

	void Fill(const T& value)
	{
		for(Size i = 0; i < m_count; i++)
			memcpy(m_buffer.m_address, &value, sizeof(T));
	}

	void CopyTo(BufferSpan<T> destination) const
	{
		memcpy(destination.m_buffer.m_address + destination.m_index.ToRawValue(), 
			m_buffer.m_address + m_index.ToRawValue(), destination.Count().ToRawValue() * sizeof(T));
	}
};