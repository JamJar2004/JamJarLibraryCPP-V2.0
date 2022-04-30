#pragma once

#include "../../Numerics.hpp"

#include "../../Exception.hpp"

class BaseAllocation
{
private:
	Size m_refCount;
public:
	BaseAllocation() : m_refCount(1U) {}

	void AddRef() { ++m_refCount; }

	void RemRef() 
	{
		--m_refCount;
		if(m_refCount == 0U)
			delete this;
	}
};

template<typename T>
class Allocation : public BaseAllocation
{
private:
	T m_value;
public:
	Allocation(const T& value) : m_value(value) {}

	template<typename... Args>
	Allocation(Args&&... args) requires ConstructibleFrom<T, Args...> : m_value(args...) {}

	      T& GetValue()       { return m_value; }
	const T& GetValue() const { return m_value; }
};

template<typename T>
class NullableRef;

template<typename T>
class SharedRef
{
private:
	BaseAllocation* m_allocation;

	SharedRef(BaseAllocation* allocation) : m_allocation(allocation) { m_allocation->AddRef(); }
public:
	template<typename... Args>
	SharedRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_allocation(new Allocation<T>(args...)) {}

	SharedRef(const SharedRef<T>& other) : SharedRef(other.m_allocation) {}

	template<Inherits<T> T2>
	SharedRef(const SharedRef<T2>& other) : SharedRef(other.m_allocation) {}

	~SharedRef() { m_allocation->RemRef(); }

	template<Inherits<T> T2>
	explicit operator SharedRef<T2>() const { return SharedRef<T2>(m_allocation); }

	SharedRef<T>& operator=(const SharedRef<T>& other)
	{
		m_allocation->RemRef();
		m_allocation = other.m_allocation;
		m_allocation->AddRef();
	}

          T& operator*()       requires !SameAs<T, void> { return ((Allocation<T>*)m_allocation)->GetValue(); }
	const T& operator*() const requires !SameAs<T, void> { return ((Allocation<T>*)m_allocation)->GetValue(); }

	      T* operator->()       { return ((Allocation<T>*)m_allocation)->GetValue(); }
	const T* operator->() const { return ((Allocation<T>*)m_allocation)->GetValue(); }

	friend class NullableRef<T>;
};

template<typename T, typename... Args>
SharedRef<T> New(Args&&... args) requires ConstructibleFrom<T, Args...> { return SharedRef<T>(args...); }

template<typename T>
class NullableRef
{
private:
	BaseAllocation* m_allocation;
public:
	NullableRef() : m_allocation(nullptr) {}

	NullableRef(const SharedRef<T>& other) : m_allocation(other.m_allocation) { m_allocation->AddRef(); }

	~NullableRef() 
	{
		if(m_allocation)
			m_allocation->RemRef();
	}

	operator Boolean() const { return m_allocation; }

	NullableRef<T>& operator=(const NullableRef<T>& other)
	{
		if(m_allocation)
			m_allocation->RemRef();

		m_allocation = other.m_allocation;
		
		if(m_allocation)
			m_allocation->AddRef();
	}

	T& operator*() requires !SameAs<T, void> 
	{ 
		if(!m_allocation)
			NullReferenceException().Throw();

		return ((Allocation<T>*)m_allocation)->GetValue(); 
	}

	const T& operator*() const requires !SameAs<T, void> 
	{ 
		if(!m_allocation)
			NullReferenceException().Throw();

		return ((Allocation<T>*)m_allocation)->GetValue();
	}

	T* operator->()
	{
		if(!m_allocation)
			NullReferenceException().Throw();

		return ((Allocation<T>*)m_allocation)->GetValue();
	}

	const T* operator->() const
	{ 
		if(!m_allocation)
			NullReferenceException().Throw();

		return ((Allocation<T>*)m_allocation)->GetValue(); 
	}
};