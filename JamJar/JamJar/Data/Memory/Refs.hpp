#pragma once

#include "../../Numerics.hpp"

template<typename T>
class NullableRef;

template<typename T>
class SharedRef
{
private:
	T*    m_address;
	Size* m_refCount;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		--(*m_refCount);
		if(*m_refCount == 0U)
		{
			delete m_address;
			delete m_refCount;
		}
	}
	
	SharedRef(T* address, Size* refCount) : m_address(address), m_refCount(refCount) {}
public:
	template<typename... Args>
	SharedRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_address(new T(args...)), m_refCount(new Size(1U)) {}

	SharedRef(const SharedRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	template<Inherits<T> T2>
	SharedRef(const SharedRef<T2>& other) : m_address((T*)other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	~SharedRef() { RemRef(); }

	template<Inherits<T> T2>
	explicit operator SharedRef<T2>() { return SharedRef<T2>((T2*)m_address, m_refCount); }

	SharedRef<T>& operator=(const SharedRef<T>& other)
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		AddRef();

		return *this;
	}

	T&       operator *() const { return *m_address; }
	T* const operator->() const { return  m_address; }

	template<typename T2>
	friend class SharedRef;

	template<typename T2>
	friend class NullableRef;
};

template<typename T, typename... Args>
SharedRef<T> New(Args&&... args) requires ConstructibleFrom<T, Args...> { return SharedRef<T>(args...); }

template<typename T>
class NullableRef
{
private:
	T*    m_address;
	Size* m_refCount;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		--(*m_refCount);
		if(*m_refCount == 0U)
		{
			delete m_address;
			delete m_refCount;
		}
	}
public:
	NullableRef(std::nullptr_t) : m_address(nullptr), m_refCount(nullptr) {}

	template<typename T, typename... Args>
	NullableRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_address(new T(args...)), m_refCount(new Size(1U)) {}

	NullableRef(const SharedRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	template<Inherits<T> T2>
	NullableRef(const SharedRef<T2>& other) : m_address((T*)other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	NullableRef(const NullableRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) 
	{
		if(m_refCount)
			AddRef(); 
	}

	template<Inherits<T> T2>
	NullableRef(const NullableRef<T2>& other) : m_address((T*)other.m_address), m_refCount(other.m_refCount)
	{
		if(m_refCount)
			AddRef();
	}

	~NullableRef() 
	{
		if(m_refCount)
			RemRef();
	}

	NullableRef<T>& operator=(const SharedRef<T>& other)
	{
		if(m_refCount)
			RemRef();

		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		
		AddRef();
		return *this;
	}

	NullableRef<T>& operator=(const NullableRef<T>& other)
	{
		if(m_refCount)
			RemRef();

		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		
		if(m_refCount)
			AddRef();

		return *this;
	}

	operator SharedRef<T>() const;

	T&       operator *() const;
	T* const operator->() const;
};