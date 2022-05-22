#pragma once

#include "../Exception.hpp"
#include "../Delegate.hpp"

#include <typeinfo>

using CopyConstructor = Function<void, const void*, void*>;
using Destructor      = Function<void, void*>;

using       MathOperation = Function<void, const void*, const void*, void*>;
using ComparisonOperation = Function<Boolean, const void*, const void*>;
using      UnaryOperation = Function<void*, void*>;

class TypeInfo
{
private:
	static Size s_lastID;

	template<typename T>
	class TypeStore
	{
	public:
		static const TypeInfo* s_type;

		static void Copy(const void* source, void* dest) requires CopyConstructible<T>;
		static void Destroy(void* address) requires Destructible<T>;
	};

	Size m_ID;

	String m_name;
	Size   m_size;

	CopyConstructor m_copyConstructor;
	Destructor      m_destructor;
public:
	template<typename T>
	TypeInfo(Size ID) : m_ID(ID), m_name(typeid(T).name()), m_size(sizeof(T)), m_copyConstructor(TypeStore<T>::Copy) {}
	
	const String& GetName() const { return m_name; }
	      Size    GetSize() const { return m_size; }

	CopyConstructor GetCopyConstructor() const { return m_copyConstructor; }
	Destructor      GetDestructor()      const { return m_destructor;      }

	Boolean operator==(const TypeInfo& other) const { return m_ID == other.m_ID; }
	Boolean operator!=(const TypeInfo& other) const { return m_ID != other.m_ID; }

	friend class Reflection;
};

Size TypeInfo::s_lastID(0U);

template<typename T>
const TypeInfo* TypeInfo::TypeStore<T>::s_type(new TypeInfo<T>(s_lastID++));

class Reflection
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

template<typename T>
inline void TypeInfo::TypeStore<T>::Copy(const void* source, void* dest) requires CopyConstructible<T>
{
	new(dest) T(*(T*)source);
}

template<typename T>
inline void TypeInfo::TypeStore<T>::Destroy(void* address) requires Destructible<T>
{
	((T*)address)->~T();
}
