#pragma once

#include "../String.hpp"
#include "../Delegate.hpp"
#include "../Exception.hpp"

using DefaultConstructor = Function<void, void*>;
using CopyConstructor    = Function<void, const void*, void*>;

using Destructor = Function<void, void*>;

using BinaryMathOperation = Function<void, const void*, const void*, void*>;
using BinaryComparisonOperation = Function<Boolean, const void*, const void*>;

class TypeInfo
{
private:
	template<typename T>
	class TypeStore
	{
	private:
		static const TypeInfo* s_type;
	public:
		static void Construct(void* value) requires DefaultConstructible<T> { new(value) T(); }

		static void CopyConstruct(const void* source, void* dest) { new(dest) T(*(T*)source); }

		static void Destroy(void* value) requires Destructible<T> { ((T*)value)->~T(); }

		static void Add(const void* left, const void* right, void* dest) requires Addable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue + rightValue);
		}

		static void Subtract(const void* left, const void* right, void* dest) requires Subtractable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue - rightValue);
		}

		static void Multiply(const void* left, const void* right, void* dest) requires Multiplicable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue * rightValue);
		}

		static void Divide(const void* left, const void* right, void* dest) requires Divisible<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue / rightValue);
		}

		static void Modulus(const void* left, const void* right, void* dest) requires Modable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue % rightValue);
		}

		static void BitwiseAnd(const void* left, const void* right, void* dest) requires BitwiseAnd<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue & rightValue);
		}

		static void BitwiseOr(const void* left, const void* right, void* dest) requires BitwiseOr<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue | rightValue);
		}

		static void BitwiseXor(const void* left, const void* right, void* dest) requires BitwiseXor<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			new(dest) T(leftValue ^ rightValue);
		}

		static Boolean SmallerThan(const void* left, const void* right) requires Comparable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			return leftValue < rightValue;
		}

		static Boolean GreaterThan(const void* left, const void* right) requires Comparable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			return leftValue > rightValue;
		}

		static Boolean SmallerOrEqualThan(const void* left, const void* right) requires Comparable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			return leftValue <= rightValue;
		}

		static Boolean GreaterOrEqualThan(const void* left, const void* right) requires Comparable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			return leftValue >= rightValue;
		}

		static Boolean Equal(const void* left, const void* right) requires Comparable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			return leftValue == rightValue;
		}

		static Boolean NotEqual(const void* left, const void* right) requires Comparable<T>
		{
			T  leftValue = *(T*)left;
			T rightValue = *(T*)right;

			return leftValue != rightValue;
		}
	};

	String m_name;
	Size   m_size;

	DefaultConstructor m_defaultConstructor;
	CopyConstructor    m_copyConstructor;

	Destructor m_destructor;

	BinaryMathOperation m_adder;
	BinaryMathOperation m_subtractor;
	BinaryMathOperation m_multiplier;
	BinaryMathOperation m_divider;
	BinaryMathOperation m_modular;

	BinaryMathOperation m_bitAndOperation;
	BinaryMathOperation m_bitOrOperation;
	BinaryMathOperation m_bitXorOperation;

	BinaryComparisonOperation m_smallerThanOperation;
	BinaryComparisonOperation m_greaterThanOperation;

	BinaryComparisonOperation m_smallerOrEqualThanOperation;
	BinaryComparisonOperation m_greaterOrEqualThanOperation;

	BinaryComparisonOperation m_equalOperation;
	BinaryComparisonOperation m_notEqualOperation;

	template<typename T>
	TypeInfo() :
		m_name(typeid(T)),
		m_size(sizeof(T)),
		m_defaultConstructor(TypeStore<T>::Construct),
		m_copyConstructor(TypeStore<T>::CopyConstruct),
		m_destructor(TypeStore<T>::Destroy),
		m_adder(TypeStore<T>::Add),
		m_subtractor(TypeStore<T>::Subtract), 
		m_multiplier(TypeStore<T>::Multiply), 
		m_divider(TypeStore<T>::Divide), 
		m_modular(TypeStore<T>::Modulus), 
		m_bitAndOperation(TypeStore<T>::BitwiseAnd), 
		m_bitOrOperation(TypeStore<T>::BitwiseOr), 
		m_bitXorOperation(TypeStore<T>::BitwiseXor),
		m_smallerThanOperation(TypeStore<T>::SmallerThan),
		m_greaterThanOperation(TypeStore<T>::GreaterThan),
		m_smallerOrEqualThanOperation(TypeStore<T>::SmallerOrEqualThan),
		m_greaterOrEqualThanOperation(TypeStore<T>::GreaterOrThan),
		m_equalOperation(TypeStore<T>::Equal),
		m_notEqualOperation(TypeStore<T>::NotEqual) {}

	TypeInfo(const TypeInfo&) = delete;
	
	TypeInfo& operator=(const TypeInfo&) = delete;
public:
	String GetName() const { return m_name; }
	Size   GetSize() const { return m_size; }

	DefaultConstructor GetDefaultConstructor() const { return m_defaultConstructor; }
	CopyConstructor    GetCopyConstructor()    const { return m_copyConstructor;    }

	Destructor GetDestructor() const { return m_destructor; }

	friend class Reflect;
};

class Reflect
{
public:
	template<typename T>
	static const TypeInfo& Type();
};

template<typename T>
const TypeInfo* TypeInfo::TypeStore<T>::s_type(new TypeInfo());

template<typename T>
inline const TypeInfo& Reflect::Type() { return TypeInfo::TypeStore<T>::s_type; }

class InvalidCastException : public Exception
{
public:
	InvalidCastException(const TypeInfo& sourceType, const TypeInfo& targetType) : 
		Exception("Invalid cast from type \'" + sourceType.GetName() + "\' to type \'" + targetType.GetName() + ".") {}
};