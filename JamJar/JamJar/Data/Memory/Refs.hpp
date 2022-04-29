#pragma once

#include "../../Numerics.hpp"

class BaseAllocation
{
private:
	Size m_refCount;
public:
	BaseAllocation() : m_refCount(1) {}

	void AddRef() { ++m_refCount; }
	void RemRef() { --m_refCount; }
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
};

template<typename T>
class SharedRef
{
protected:
	BaseAllocation* m_allocation;
public:
	template<typename... Args>
	SharedRef(Args&&... args) requires ConstructibleFrom<T, Args...> : m_allocation(new Allocation<T>(args...)) {}

	SharedRef(const SharedRef<T>& other) : m_allocation(other.m_allocation) { m_allocation->AddRef(); }

	~SharedRef() { m_allocation->RemRef(); }

	SharedRef<T>& operator=(const SharedRef<T>& other)
	{
		m_allocation->RemRef();
		m_allocation = other.m_allocation;
		m_allocation->AddRef();
	}
};

template<typename T, typename... Args>
SharedRef<T> New(Args&&... args) requires ConstructibleFrom<T, Args...> { return SharedRef<T>(args...); }