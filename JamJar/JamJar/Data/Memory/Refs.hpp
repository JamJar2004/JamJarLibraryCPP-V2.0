#pragma once

#include "../../Numerics.hpp"

template<typename T>
class SharedRef;

template<typename T>
class NullableRef;

template<typename T>
class WeakRef;

class SharedDynamicRef;

class RefCounter
{
private:
	Size m_useCount;
	Size m_weakCount;
public:
	RefCounter(Size useCount, Size weakCount) : m_useCount(useCount), m_weakCount(weakCount) {}

	template<typename T>
	friend class SharedRef;

	template<typename T>
	friend class NullableRef;

	template<typename T>
	friend class WeakRef;

	template<typename T>
	friend class NullableWeakRef;
};

template<typename T>
class SharedRef
{
private:
	T*          m_address;
	RefCounter* m_refCount;

	void AddRef() { ++m_refCount->m_useCount; }

	void RemRef()
	{
		if(--m_refCount->m_useCount == 0U)
		{
			delete m_address;
			if(m_refCount->m_weakCount == 0U)
				delete m_refCount;
		}
	}
	
	SharedRef(T* address, RefCounter* refCount) : m_address(address), m_refCount(refCount) {}
public:
	template<typename... Args>
	SharedRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_address(new T(args...)), m_refCount(new RefCounter(1U, 0U)) {}

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

	friend Boolean operator==(const SharedRef<T>& left, const SharedRef<T>& right) { return left.m_address == right.m_address; }
	friend Boolean operator!=(const SharedRef<T>& left, const SharedRef<T>& right) { return left.m_address != right.m_address; }

	String ToString() const requires Printable<T>;

	template<typename T2>
	friend class UniqueRef;

	template<typename T2>
	friend class SharedRef;

	template<typename T2>
	friend class NullableRef;

	template<typename T2>
	friend class WeakRef;

	template<typename T2>
	friend class NullableWeakRef;

	friend class SharedDynamicRef;
};

template<typename T, typename... Args>
SharedRef<T> New(Args&&... args) requires ConstructibleFrom<T, Args...> { return SharedRef<T>(args...); }

template<typename T>
class NullableRef
{
private:
	T*          m_address;
	RefCounter* m_refCount;

	void AddRef() { ++m_refCount->m_useCount; }

	void RemRef()
	{
		if(--m_refCount->m_useCount == 0U)
		{
			delete m_address;
			if(m_refCount->m_weakCount == 0U)
				delete m_refCount;
		}
	}
public:
	NullableRef() : NullableRef(nullptr) {}

	NullableRef(std::nullptr_t) : m_address(nullptr), m_refCount(nullptr) {}

	template<typename T, typename... Args>
	NullableRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_address(new T(args...)), m_refCount(new RefCounter(1U, 0U)) {}

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

	explicit operator SharedRef<T>() const;

	T&       operator *() const;
	T* const operator->() const;

	Boolean IsNull() const { return m_address == nullptr; }

	friend Boolean operator==(const NullableRef<T>& left, const NullableRef<T>& right) { return left.m_address == right.m_address; }
	friend Boolean operator!=(const NullableRef<T>& left, const NullableRef<T>& right) { return left.m_address != right.m_address; }

	String ToString() const requires Printable<T>;

	template<typename T2>
	friend class NullableWeakRef;
};

template<typename T>
class WeakRef
{
private:
	T*          m_address;
	RefCounter* m_refCount;

	void AddRef() { ++m_refCount->m_weakCount; }

	void RemRef()
	{
		if(--m_refCount->m_weakCount == 0U && m_refCount->m_useCount == 0U)
			delete m_refCount;
	}
public:
	WeakRef(const SharedRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	WeakRef(const WeakRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	WeakRef& operator=(const WeakRef<T>& other)
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		AddRef();
	}

	operator NullableRef<T>()
	{
		if(m_refCount->m_useCount == 0U)
			return nullptr;

		++m_refCount->m_useCount;
		return SharedRef<T>(m_address, m_refCount);
	}

	String ToString() const requires Printable<T>;
};

template<typename T>
class NullableWeakRef
{
private:
	T*          m_address;
	RefCounter* m_refCount;

	void AddRef() { ++m_refCount->m_weakCount; }

	void RemRef()
	{
		if(--m_refCount->m_weakCount == 0U && m_refCount->m_useCount == 0U)
			delete m_refCount;
	}
public:
	NullableWeakRef() : m_address(nullptr), m_refCount(nullptr) {}

	NullableWeakRef(std::nullptr_t) : m_address(nullptr), m_refCount(nullptr) {}

	NullableWeakRef(const SharedRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	NullableWeakRef(const NullableRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount)
	{
		if(m_refCount)
			AddRef();
	}

	NullableWeakRef(const WeakRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount) { AddRef(); }

	NullableWeakRef(const NullableWeakRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount)
	{
		if(m_refCount)
			AddRef();
	}

	NullableWeakRef& operator=(const WeakRef<T>& other)
	{
		if(m_refCount)
			RemRef();

		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		AddRef();
	}

	NullableWeakRef& operator=(const NullableWeakRef<T>& other)
	{
		if(m_refCount)
			RemRef();

		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		
		if(m_refCount)
			AddRef();

		return *this;
	}

	operator NullableRef<T>() const
	{
		if(!m_address)
			return nullptr;

		if(m_refCount->m_useCount == 0U)
			return nullptr;

		++m_refCount->m_useCount;
		return SharedRef<T>(m_address, m_refCount);
	}

	String ToString() const requires Printable<T>;
};