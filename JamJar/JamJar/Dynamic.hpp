#pragma once

#include "Data/Reflection.hpp"

class Dynamic
{
private:
	      void*     m_address;
	const TypeInfo& m_type;
public:
	template<typename T>
	Dynamic(const T& value) requires CopyConstructible<T> : m_address(new T(value)), m_type(Reflection::GetType<T>()) {}

	Dynamic(const Dynamic& other);
	~Dynamic();

	const TypeInfo& GetType() const { return m_type; }

	template<typename T>
	operator T() const
	{
		if(m_type != Reflection::GetType<T>())
			InvalidCastException(m_type, Reflection::GetType<T>()).Throw();

		return *(T*)m_address; 
	}
};