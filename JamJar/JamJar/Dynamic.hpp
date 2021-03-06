#pragma once

#include "Data/Reflection.hpp"

class NullType
{

};

class Dynamic
{
private:
	      void*     m_address;
	const TypeInfo& m_type;

	Dynamic(void* address, const TypeInfo& type) : m_address(address), m_type(type) {}
public:
	template<CopyConstructible T>
	Dynamic(const T& value) : m_address(malloc(sizeof(T))), m_type(Reflect::GetType<T>()) { new(m_address) T(value); }

	template<MoveConstructible T>
	Dynamic(T&& value) requires(!SameAs<T, Dynamic&>) : m_address(malloc(sizeof(T))), m_type(Reflect::GetType<T>()) { new(m_address) T(std::move(value)); }

	Dynamic(const Dynamic& other);
	Dynamic(Dynamic&& other) noexcept;

	~Dynamic();

	template<typename T>
	Dynamic& operator=(const T& other)
	{
		if(m_type != Reflect::GetType<T>())
		{
			InvalidCastException(Reflect::GetType<T>(), m_type).Throw();
			return *this;
		}

		m_type.GetCopyAssigner()(&other, m_address);
		return *this;
	}

	const TypeInfo& GetType() const { return m_type; }

	template<typename T>
	Boolean TryCast(T& result)
	{
		if(m_type != Reflect::GetType<T>())
			return false;
		
		result = *(T*)m_address;
		return true;
	}

	template<typename T>
	T Cast() const
	{
		if(m_type != Reflect::GetType<T>()) 
			InvalidCastException(m_type, Reflect::GetType<T>()).Throw();
		
		return *(T*)m_address;
	}

	template<typename T>
	operator T() const { return Cast<T>(); }

	friend Dynamic operator+(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Addition can only be done between dynamics of the same type.").Throw();
			return NullType();
		}

		Dynamic result = Dynamic(malloc(left.m_type.GetSize().ToRawValue()), left.m_type);
		result.m_type.GetAdditionOperation()(result.m_address, left.m_address, right.m_address);

		return result;
	}

	friend Dynamic operator-(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Subtraction can only be done between dynamics of the same type.").Throw();
			return NullType();
		}

		Dynamic result = Dynamic(malloc(left.m_type.GetSize().ToRawValue()), left.m_type);
		result.m_type.GetSubtractionOperation()(result.m_address, left.m_address, right.m_address);

		return result;
	}

	friend Dynamic operator*(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Multiplication can only be done between dynamics of the same type.").Throw();
			return NullType();
		}

		Dynamic result = Dynamic(malloc(left.m_type.GetSize().ToRawValue()), left.m_type);
		result.m_type.GetMultiplicationOperation()(result.m_address, left.m_address, right.m_address);

		return result;
	}

	friend Dynamic operator/(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Division can only be done between dynamics of the same type.").Throw();
			return NullType();
		}

		Dynamic result = Dynamic(malloc(left.m_type.GetSize().ToRawValue()), left.m_type);
		result.m_type.GetDivisionOperation()(result.m_address, left.m_address, right.m_address);

		return result;
	}

	friend Dynamic operator%(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Modulus can only be done between dynamics of the same type").Throw();
			return NullType();
		}

		Dynamic result = Dynamic(malloc(left.m_type.GetSize().ToRawValue()), left.m_type);
		result.m_type.GetModulusOperation()(result.m_address, left.m_address, right.m_address);

		return result;
	}

	friend Boolean operator<(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Comparisons can only be done between dynamics of the same type").Throw();
			return false;
		}

		const TypeInfo& type = left.m_type;
		return type.GetSmaller()(left.m_address, right.m_address);
	}

	friend Boolean operator>(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
		{
			Exception("Comparisons can only be done between dynamics of the same type").Throw();
			return false;
		}

		const TypeInfo& type = left.m_type;
		return type.GetGreater()(left.m_address, right.m_address);
	}

	friend Boolean operator<=(const Dynamic& left, const Dynamic& right)
	{
		if (left.m_type != right.m_type)
		{
			Exception("Comparisons can only be done between dynamics of the same type").Throw();
			return false;
		}

		const TypeInfo& type = left.m_type;
		return type.GetSmallerOrEqual()(left.m_address, right.m_address);
	}

	friend Boolean operator>=(const Dynamic& left, const Dynamic& right)
	{
		if (left.m_type != right.m_type)
		{
			Exception("Comparisons can only be done between dynamics of the same type").Throw();
			return false;
		}

		const TypeInfo& type = left.m_type;
		return type.GetGreaterOrEqual()(left.m_address, right.m_address);
	}

	friend Boolean operator==(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
			return false;

		const TypeInfo& type = left.m_type;

		return type.GetEquator()(left.m_address, right.m_address);
	}

	friend Boolean operator!=(const Dynamic& left, const Dynamic& right)
	{
		if(left.m_type != right.m_type)
			return true;

		const TypeInfo& type = left.m_type;

		return type.GetNotEquator()(left.m_address, right.m_address);
	}

	HashCode GetHashCode() const { return m_type.GetHasher()(m_address); }
};

class DynamicRef
{
private:
	void* m_address;
	Size* m_refCount;

	const TypeInfo& m_type;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		--(*m_refCount);
		if (*m_refCount == 0U)
		{
			delete m_address;
			delete m_refCount;
		}
	}
public:
	template<typename T>
	DynamicRef(const SharedRef<T>& other) : m_address(other.m_address), m_refCount(other.m_useCount), m_type(Reflect::GetType<T>()) { AddRef(); }

	DynamicRef(const DynamicRef& other) : m_address(other.m_address), m_refCount(other.m_refCount), m_type(other.m_type) { AddRef(); }

	template<typename T>
	operator SharedRef<T>() const 
	{
		if(m_type != Reflect::GetType<T>())
			InvalidCastException(m_type, Reflect::GetType<T>()).Throw();

		return SharedRef<T>((T*)m_address, m_refCount);
	}

	const TypeInfo& GetType() const { return m_type; }

	template<typename T>
	T& Get() const
	{
		if(m_type != Reflect::GetType<T>())
			InvalidCastException(m_type, Reflect::GetType<T>()).Throw();

		return *(T*)m_address;
	}
};