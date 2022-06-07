#pragma once

#include "../../Numerics.hpp"

template<typename T>
class SharedRef;

template<typename T>
class NullableRef;

template<typename T>
class BaseAllocation
{
protected:
	Size m_refCount;

	void    AddRef() { ++m_refCount;                          }
	Boolean RemRef() { --m_refCount; return m_refCount == 0U; }
public:
	BaseAllocation() {}

	virtual ~BaseAllocation() {}

	virtual       T& GetValue()       = 0;
	virtual const T& GetValue() const = 0;

	friend class SharedRef<T>;
	friend class NullableRef<T>;
};

template<typename T>
class Allocation : public BaseAllocation<T>
{
private:
	T m_value;
public:
	Allocation(const T& value) requires CopyConstructible<T> : m_value(value) {}

	template<typename... Args>
	Allocation(Args&&... args) requires ConstructibleFrom<T, Args...> : m_value(args...) {}

	      T& GetValue()       override { return m_value; }
	const T& GetValue() const override { return m_value; }

	friend class SharedRef<T>;
	friend class NullableRef<T>;
};

template<typename T>
class SharedRef
{
private:
	BaseAllocation<T>* m_allocation;

	void AddRef() { m_allocation->AddRef(); }
	
	void RemRef()
	{
		if(m_allocation->RemRef())
			delete m_allocation;
	}

	SharedRef(BaseAllocation<T>* allocation) : m_allocation(allocation) { AddRef(); }
public:
	SharedRef(const T& value) requires CopyConstructible<T> : SharedRef(new Allocation<T>()) {}

	template<typename... Args>
	SharedRef(Args&&... args) requires ConstructibleFrom<T, Args...> : SharedRef(new Allocation<T>(args...)) {}

	template<Inherits<T> T2>
	SharedRef(const SharedRef<T2>& other) : SharedRef((BaseAllocation<T>*)other.m_allocation) {}

	SharedRef(const SharedRef<T>& other) : SharedRef(other.m_allocation) {}

	~SharedRef() { RemRef(); }

	template<Inherits<T> T2>
	explicit operator SharedRef<T2>() const { return SharedRef<T2>(m_allocation); }

	SharedRef<T>& operator=(const SharedRef<T>& other) 
	{
		RemRef();
		m_allocation = other.m_allocation;
		AddRef();

		return *this;
	}

		  T& operator*()       { return m_allocation->GetValue(); }
	const T& operator*() const { return m_allocation->GetValue(); }

	      T* operator->()       { return &m_allocation->GetValue(); }
	const T* operator->() const { return &m_allocation->GetValue(); }

	template<typename T2>
	friend Boolean operator==(const SharedRef<T>& left, const SharedRef<T2>& right) { return left.m_allocation == right.m_allocation; }

	template<typename T2>
	friend Boolean operator!=(const SharedRef<T>& left, const SharedRef<T2>& right) { return left.m_allocation != right.m_allocation; }

	template<typename T2>
	friend class SharedRef;

	friend class NullableRef<T>;
};

template<typename T, typename... Args>
SharedRef<T> New(Args&&... args) requires ConstructibleFrom<T, Args...> { return SharedRef<T>(args...); }

template<typename T>
class NullableRef
{
private:
	BaseAllocation<T>* m_allocation;

	void AddRef()
	{ 
		if(m_allocation)
			m_allocation->AddRef(); 
	}

	void RemRef()
	{
		if(m_allocation)
			if(m_allocation->RemRef())
				delete m_allocation;
	}
public:
	NullableRef(std::nullptr_t) : m_allocation(nullptr) {}

	NullableRef(const T& value) requires CopyConstructible<T> : m_allocation(new Allocation<T>()) { m_allocation->AddRef(); }

	template<typename... Args>
	NullableRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_allocation(new Allocation<T>(args...)) { m_allocation->AddRef(); }

	NullableRef(const SharedRef<T>& other) : m_allocation(other.m_allocation) { m_allocation->AddRef(); }

	NullableRef(const NullableRef<T>& other) : m_allocation(other.m_allocation) { AddRef(); }

	~NullableRef() { m_allocation->RemRef(); }

	explicit operator SharedRef<T>() const;
	/*{
		if(!m_allocation)
			NullReferenceException().Throw();

		return SharedRef<T>(m_allocation); 
	}*/

	NullableRef<T>& operator=(const SharedRef<T>& other)
	{
		RemRef();
		m_allocation = other.m_allocation;
		AddRef();
	}

	NullableRef<T>& operator=(const NullableRef<T>& other)
	{
		RemRef();
		m_allocation = other.m_allocation;
		AddRef();
	}

	Boolean IsNull() { return !m_allocation; }

	T& operator*();
	//{
	//	if(!m_allocation)
	//		NullReferenceException().Throw();

	//	return m_allocation->GetValue();
	//}

	const T& operator*() const;
	//{
	//	if(!m_allocation)
	//		NullReferenceException().Throw();

	//	return m_allocation->GetValue();
	//}

	T* operator->();
	//{
	//	if(!m_allocation)
	//		NullReferenceException().Throw();

	//	return m_allocation->AsPointer();
	//}

	const T* operator->() const;
	//{ 
	//	if(!m_allocation)
	//		NullReferenceException().Throw();

	//	return m_allocation->AsPointer();
	//}

	template<typename T2>
	friend Boolean operator==(const NullableRef<T>& left, const NullableRef<T2>& right) { return left.m_allocation == right.m_allocation; }

	template<typename T2>
	friend Boolean operator!=(const NullableRef<T>& left, const NullableRef<T2>& right) { return left.m_allocation != right.m_allocation; }
};
