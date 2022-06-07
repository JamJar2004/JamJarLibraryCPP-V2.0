#pragma once

#include "../Exception.hpp"
#include "../Delegate.hpp"

#include <typeinfo>

using CopyConstructor = Function<void, const void*, void*>;
using Destructor      = Function<void, void*>;

using CopyAssigner = Function<void, const void*, void*>;

using Comparer = Function<Boolean, const void*, const void*>;

class TypeInfo
{
private:
	static Size s_lastID;

	Size m_ID;

	String m_name;
	Size   m_size;

	CopyConstructor m_copyConstructor;
	Destructor      m_destructor;

	CopyAssigner m_copyAssigner;

	Comparer m_equator;
	Comparer m_notEquator;

	template<typename T>
	class TypeStore
	{
	private:
		static TypeInfo s_type;
	public:
		friend class Reflect;

		static void Copy(const void* source, void* dest) requires CopyConstructible<T> { new(dest) T(*(const T*)source);                                      }
		static void Copy(const void* source, void* dest)                               { Exception("Type " + s_type + " is not copy constructible.").Throw(); }

		static void Destroy(void* address) requires Destructible<T> { ((T*)address)->~T(); }
		static void Destroy(void* address)                          {                      }

		static void Assign(const void* source, void* dest) requires CopyAssignable<T>
		{
			T& destValue = *(T*)dest;
			destValue = *(T*)source;
		}

		static void Assign(const void* source, void* dest) { Exception("Type " + s_type + " is not copy assignable.").Throw(); }
		
		static Boolean Equal(const void* left, const void* right) requires Equatable<T>
		{
			T& leftValue  = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue == rightValue;
		}

		static Boolean Equal(const void* left, const void* right) { Exception("Type " + s_type + " is not equatable.").Throw(); return false; }

		static Boolean NotEqual(const void* left, const void* right) requires Equatable<T>
		{
			T& leftValue  = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue != rightValue;
		}

		static Boolean NotEqual(const void* left, const void* right) { Exception("Type " + s_type + " is not equatable.").Throw(); return false; }
	};
public:
	TypeInfo(
		Size ID, 
		const String& name,
		Size size, 
		CopyConstructor copyConstructor, 
		Destructor destructor,
		CopyAssigner copyAssigner, 
		Comparer equator,
		Comparer notEquator) :

		m_ID(ID), 
		m_name(name),
		m_size(size), 
		m_copyConstructor(copyConstructor), 
		m_destructor(destructor), 
		m_copyAssigner(copyAssigner),
		m_equator(equator),
		m_notEquator(notEquator) {}

	const String& GetName() const { return m_name; }
	      Size    GetSize() const { return m_size; }

	CopyConstructor GetCopyConstructor() const { return m_copyConstructor; }
	Destructor      GetDestructor()      const { return m_destructor;      }

	CopyAssigner GetCopyAssigner() const { return m_copyAssigner; }

	Comparer GetEquator()    const { return m_equator;    }
	Comparer GetNotEquator() const { return m_notEquator; }

	friend Boolean operator==(const TypeInfo& left, const TypeInfo& right) { return left.m_ID == right.m_ID; }
	friend Boolean operator!=(const TypeInfo& left, const TypeInfo& right) { return left.m_ID != right.m_ID; }

	String ToString() const { return m_name; }

	friend class Reflect;
};

template<typename T>
TypeInfo TypeInfo::TypeStore<T>::s_type(s_lastID++, typeid(T).name(), sizeof(T), Copy, Destroy, Assign, Equal, NotEqual);

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

