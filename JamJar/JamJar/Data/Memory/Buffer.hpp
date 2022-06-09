#pragma once

#include "Refs.hpp"

template<typename T>
class ArrayRef;

template<typename T>
class UnsafeRef;

template<typename T>
class ArrayList;

template<typename T>
class BufferRef
{
private:
	T*    m_address;
	Size* m_refCount;
	Size  m_count;

	void AddRef() { ++(*m_refCount); }

	void RemRef() 
	{
		--(*m_refCount);
		if(*m_refCount == 0U)
		{ 
			free(m_address);
			delete m_refCount;
		}
	}
public:
	BufferRef(Size count) : m_address((T*)malloc(sizeof(T) * count.ToRawValue())), m_refCount(new Size(1U)), m_count(count) {}

	BufferRef(const BufferRef<T>& other) : 
		m_address(other.m_address), m_refCount(other.m_refCount), m_count(other.m_count) { AddRef(); }

	~BufferRef() { RemRef(); }

	operator UnsafeRef<T>() const { return UnsafeRef<T>(m_address); }

	UnsafeRef<T> First() const { return UnsafeRef<T>(m_address                       ); }
	UnsafeRef<T> Last()  const { return UnsafeRef<T>(m_address + m_count.ToRawValue()); }

	      UnsafeRef<T> operator[](Size index)       { return UnsafeRef<T>(m_address + index.ToRawValue()); }
	const UnsafeRef<T> operator[](Size index) const { return UnsafeRef<T>(m_address + index.ToRawValue()); }

	Size Count() const { return m_count; }

	BufferRef<T>& operator=(const BufferRef<T>& other) 
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		m_count    = other.m_count;
		AddRef();

		return *this;
	}

	void CopyTo(const UnsafeRef<T>& other, Size count) const
	{
		if(count == 0U)
			return;

		memcpy(other.m_address, m_address, count.ToRawValue() * sizeof(T));
	}

	friend class ArrayRef<T>;
	friend class UnsafeRef<T>;
	friend class ArrayList<T>;
};

template<typename T>
class UnsafeRef
{
private:
	T* m_address;

	UnsafeRef(T* address) : m_address(address) {}
public:
	      T& operator*()       { return *m_address; }
	const T& operator*() const { return *m_address; }

	      T* operator->()       { return m_address; }
	const T* operator->() const { return m_address; }

	friend UnsafeRef<T> operator+(const UnsafeRef<T>& left, Size right) { return left.m_address + right.ToRawValue(); }
	friend UnsafeRef<T> operator-(const UnsafeRef<T>& left, Size right) { return left.m_address - right.ToRawValue(); }

	UnsafeRef<T>& operator+=(Size other) { m_address += other.ToRawValue(); return *this; }
	UnsafeRef<T>& operator-=(Size other) { m_address -= other.ToRawValue(); return *this; }

	friend Size operator-(const UnsafeRef<T>& left, const UnsafeRef<T>& right) { return Size(left.m_address - right.m_address); }

	UnsafeRef<T>& operator++() { ++m_address; return *this; }
	UnsafeRef<T>& operator--() { --m_address; return *this; }

	friend Boolean operator==(const UnsafeRef<T>& left, const UnsafeRef<T>& right) { return left.m_address == right.m_address; }
	friend Boolean operator!=(const UnsafeRef<T>& left, const UnsafeRef<T>& right) { return left.m_address != right.m_address; }

	T* ToUnsafePointer() const { return m_address; }

	template<typename... Args>
	void Initialize(Args&&... args) requires ConstructibleFrom<T, Args...> { new(m_address) T(args...); }

	void Initialize(const T& item) requires CopyConstructible<T> { new(m_address) T(item); }

	void Destroy() { m_address->~T(); }

	void CopyTo(const UnsafeRef<T>& other, Size count) const
	{
		if(count == 0U)
			return;

		memcpy(other.m_address, m_address, count.ToRawValue() * sizeof(T));
	}

	friend class BufferRef<T>;
};
