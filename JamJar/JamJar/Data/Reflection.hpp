#pragma once

#include "../Exception.hpp"
#include "../Delegate.hpp"

#include <typeinfo>

class TypeInfo
{
private:
	static Size s_lastID;

	Size m_ID;

	String m_name;
	Size   m_size;

	template<typename T>
	class TypeStore
	{
	private:
		static TypeInfo s_type;
	public:
		friend class Reflect;
	};
public:
	TypeInfo(Size ID, const String& name, Size size) : m_ID(ID), m_name(name) {}

	const String& GetName() const { return m_name; }
	      Size    GetSize() const { return m_size; }

	friend Boolean operator==(const TypeInfo& left, const TypeInfo& right) { return left.m_ID == right.m_ID; }
	friend Boolean operator!=(const TypeInfo& left, const TypeInfo& right) { return left.m_ID != right.m_ID; }

	friend class Reflect;
};

Size TypeInfo::s_lastID(0U);

template<typename T>
TypeInfo TypeInfo::TypeStore<T>::s_type(s_lastID++, typeid(T).name(), sizeof(T));

class Reflect
{
public:
	template<typename T>
	static const TypeInfo& GetType() { return TypeInfo::TypeStore<T>::s_type; }
};

class InvalidCastException : public Exception
{
public:
	InvalidCastException(const TypeInfo& sourceType, const TypeInfo& targetType) : 
		Exception("Cannot cast from type: '" + sourceType.GetName() + "' to type '" + targetType.GetName() + "'.") {}
};

