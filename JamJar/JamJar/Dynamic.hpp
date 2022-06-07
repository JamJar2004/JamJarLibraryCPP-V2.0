#pragma once

#include "Data/Reflection.hpp"

class Dynamic
{
private:
	      void*     m_address;
	const TypeInfo& m_type;
public:
	template<typename T>
	Dynamic(const T& value) requires CopyConstructible<T> : m_address(malloc(sizeof(T))), m_type(Reflect::GetType<T>()) { new(m_address) T(value); }

	Dynamic(const Dynamic& other);

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
};

