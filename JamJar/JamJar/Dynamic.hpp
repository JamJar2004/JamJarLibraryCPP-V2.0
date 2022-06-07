#pragma once

#include "Data/Reflection.hpp"

class Dynamic
{
private:
	      void*     m_address;
	const TypeInfo& m_type;
public:
	template<typename T>
	Dynamic(const T& value) requires CopyConstructible<T> : m_address(new T(value)), m_type(Reflect::GetType<T>()) {}

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
};
