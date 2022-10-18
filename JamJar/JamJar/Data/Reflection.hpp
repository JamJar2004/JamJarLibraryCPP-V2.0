#pragma once

#include "../Exception.hpp"
#include "../Delegate.hpp"

#include <typeinfo>

using DefaultConstructor = Function<void, void*>;
using CopyConstructor    = Function<void, const void*, void*>;
using Destructor         = Function<void, void*>;

using CopyAssigner = Function<void, const void*, void*>;
using MoveAssigner = Function<void, const void*, void*>;

using MathOperation = Function<void, void*, const void*, const void*>;

using Comparer = Function<Boolean, const void*, const void*>;

using Hasher = Function<HashCode, const void*>;

class TypeInfo
{
private:
	static Size s_lastID;

	Size m_ID;

	String m_name;
	Size   m_size;

	DefaultConstructor m_defaultConstructor;
	CopyConstructor    m_copyConstructor;
	CopyConstructor    m_moveConstructor;
	Destructor         m_destructor;

	CopyAssigner m_copyAssigner;
	MoveAssigner m_moveAssigner;

	MathOperation m_addOperation;
	MathOperation m_subOperation;
	MathOperation m_mulOperation;
	MathOperation m_divOperation;
	MathOperation m_modOperation;

	Comparer m_smaller;
	Comparer m_greater;
	Comparer m_smallerOrEqual;
	Comparer m_greaterOrEqual;
	Comparer m_equator;
	Comparer m_notEquator;

	Hasher m_hasher;

	template<typename T>
	class TypeStore
	{
	private:
		static TypeInfo s_type;
	public:
		friend class Reflect;

		template<DefaultConstructible T>
		static void Initialize(void* dest) { new(dest) T(); }

		template<typename T>
		static void Initialize(void* dest) { Exception("Type " + s_type + " is not default constructible.").Throw(); }

		template<CopyConstructible T>
		static void Copy(const void* source, void* dest) { new(dest) T(*(const T*)source); }

		template<typename T>
		static void Copy(const void* source, void* dest) { Exception("Type " + s_type + " is not copy constructible.").Throw(); }

		template<MoveConstructible T>
		static void Move(const void* source, void* dest) { new(dest) T(std::move(*(const T*)source)); }

		template<typename T>
		static void Move(const void* source, void* dest) { Copy<T>(source, dest); }

		template<Destructible T>
		static void Destroy(void* address) { ((T*)address)->~T(); }
		
		template<typename T>
		static void Destroy(void* address) {}

		template<CopyAssignable T>
		static void CopyAssign(const void* source, void* dest)
		{
			T& destValue = *(T*)dest;
			destValue = *(T*)source;
		}

		template<typename T>
		static void CopyAssign(const void* source, void* dest) { Exception("Type " + s_type + " is not copy assignable.").Throw(); }

		template<MoveAssignable T>
		static void MoveAssign(const void* source, void* dest)
		{
			T& destValue = *(T*)dest;
			destValue = std::move(*(T*)source);
		}

		template<typename T>
		static void MoveAssign(const void* source, void* dest) { CopyAssign<T>(source, dest); }
		
		template<Addable T>
		static void Add(void* dest, const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			new(dest) T(leftValue + rightValue);
		}

		template<Subtractable T>
		static void Subtract(void* dest, const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			new(dest) T(leftValue - rightValue);
		}

		template<Multiplicable T>
		static void Multiply(void* dest, const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			new(dest) T(leftValue * rightValue);
		}

		template<Divisible T>
		static void Divide(void* dest, const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			new(dest) T(leftValue / rightValue);
		}

		template<Modable T>
		static void Modulate(void* dest, const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			new(dest) T(leftValue % rightValue);
		}

		template<typename T>
		static void Add(void* dest, const void* left, const void* right)      { Exception("Type " + s_type + " is not addable.").Throw(); }

		template<typename T>
		static void Subtract(void* dest, const void* left, const void* right) { Exception("Type " + s_type + " is not subtractable.").Throw(); }

		template<typename T>
		static void Multiply(void* dest, const void* left, const void* right) { Exception("Type " + s_type + " is not multiplicable.").Throw(); }

		template<typename T>
		static void Divide(void* dest, const void* left, const void* right)   { Exception("Type " + s_type + " is not divisible.").Throw(); }

		template<typename T>
		static void Modulate(void* dest, const void* left, const void* right) { Exception("Type " + s_type + " is not modable.").Throw(); }

		template<Comparable T>
		static Boolean Smaller(const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue < rightValue;
		}

		template<Comparable T>
		static Boolean SmallerOrEqual(const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue <= rightValue;
		}

		template<Comparable T>
		static Boolean Greater(const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue > rightValue;
		}

		template<Comparable T>
		static Boolean GreaterOrEqual(const void* left, const void* right)
		{
			T& leftValue = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue >= rightValue;
		}

		template<Equatable T>
		static Boolean Equal(const void* left, const void* right)
		{
			T& leftValue  = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue == rightValue;
		}

		template<Equatable T>
		static Boolean NotEqual(const void* left, const void* right)
		{
			T& leftValue  = *(T*)left;
			T& rightValue = *(T*)right;

			return leftValue != rightValue;
		}

		template<typename T>
		static Boolean Smaller(const void* left, const void* right) { Exception("Type " + s_type + " is not comparable.").Throw(); return false; }

		template<typename T>
		static Boolean SmallerOrEqual(const void* left, const void* right) { Exception("Type " + s_type + " is not comparable.").Throw(); return false; }

		template<typename T>
		static Boolean Greater(const void* left, const void* right) { Exception("Type " + s_type + " is not comparable.").Throw(); return false; }

		template<typename T>
		static Boolean GreaterOrEqual(const void* left, const void* right) { Exception("Type " + s_type + " is not comparable.").Throw(); return false; }

		template<typename T>
		static Boolean Equal(const void* left, const void* right) { Exception("Type " + s_type + " is not equatable.").Throw(); return false; }

		template<typename T>
		static Boolean NotEqual(const void* left, const void* right) { Exception("Type " + s_type + " is not equatable.").Throw(); return false; }

		template<Hashable T>
		static HashCode GetHashCode(const void* value) { return ((T*)value)->GetHashCode(); }

		template<typename T>
		static HashCode GetHashCode(const void* value) { Exception("Type " + s_type + " is not hashable.").Throw(); return HashCode(); }
	};
public:
	TypeInfo(
		Size ID,
		const String& name,
		Size size,
		DefaultConstructor defaultConstructor,
		CopyConstructor copyConstructor,
		CopyConstructor moveConstructor,
		Destructor destructor,
		CopyAssigner copyAssigner,
		MoveAssigner moveAssigner,
		MathOperation addOperation,
		MathOperation subOperation,
		MathOperation mulOperation,
		MathOperation divOperation,
		MathOperation modOperation,
		Comparer smaller,
		Comparer greater,
		Comparer smallerOrEqual,
		Comparer greaterOrEqual,
		Comparer equator,
		Comparer notEquator,
		Hasher hasher) :

		m_ID(ID),
		m_name(name),
		m_size(size),
		m_defaultConstructor(defaultConstructor),
		m_copyConstructor(copyConstructor),
		m_moveConstructor(moveConstructor),
		m_destructor(destructor), 
		m_copyAssigner(copyAssigner),
		m_moveAssigner(moveAssigner),
		m_addOperation(addOperation),
		m_subOperation(subOperation),
		m_mulOperation(mulOperation),
		m_divOperation(divOperation),
		m_modOperation(modOperation),
		m_smaller(smaller),
		m_greater(greater),
		m_smallerOrEqual(smallerOrEqual),
		m_greaterOrEqual(greaterOrEqual),
		m_equator(equator),
		m_notEquator(notEquator),
		m_hasher(hasher) {}

	const String& GetName() const { return m_name; }
	      Size    GetSize() const { return m_size; }

	DefaultConstructor GetDefaultConstructor() const { return m_defaultConstructor; }
	CopyConstructor    GetCopyConstructor()    const { return m_copyConstructor;    }
	CopyConstructor    GetMoveConstructor()    const { return m_moveConstructor;    }
	Destructor         GetDestructor()         const { return m_destructor;         }

	CopyAssigner GetCopyAssigner() const { return m_copyAssigner; }
	MoveAssigner GetMoveAssigner() const { return m_moveAssigner; }

	MathOperation GetAdditionOperation()       const { return m_addOperation; }
	MathOperation GetSubtractionOperation()    const { return m_subOperation; }
	MathOperation GetMultiplicationOperation() const { return m_mulOperation; }
	MathOperation GetDivisionOperation()       const { return m_divOperation; }
	MathOperation GetModulusOperation()        const { return m_modOperation; }

	Comparer GetSmaller()        const { return m_smaller;        }
	Comparer GetGreater()        const { return m_greater;        }
	Comparer GetSmallerOrEqual() const { return m_smallerOrEqual; }
	Comparer GetGreaterOrEqual() const { return m_greaterOrEqual; }
	Comparer GetEquator()        const { return m_equator;        }
	Comparer GetNotEquator()     const { return m_notEquator;     }

	Hasher GetHasher() const { return m_hasher; }

	friend Boolean operator==(const TypeInfo& left, const TypeInfo& right) { return left.m_ID == right.m_ID; }
	friend Boolean operator!=(const TypeInfo& left, const TypeInfo& right) { return left.m_ID != right.m_ID; }

	HashCode GetHashCode() const { return m_ID; }

	String ToString() const { return m_name; }

	friend class Reflect;
};

template<typename T>
TypeInfo TypeInfo::TypeStore<T>::s_type(s_lastID++, typeid(T).name(), sizeof(T),
	Initialize<T>,
	Copy<T>,
	Move<T>,
	Destroy<T>,
	CopyAssign<T>,
	MoveAssign<T>,
	Add<T>, 
	Subtract<T>,
	Multiply<T>,
	Divide<T>, 
	Modulate<T>,
	Smaller<T>, 
	Greater<T>,
	SmallerOrEqual<T>,
	GreaterOrEqual<T>,
	Equal<T>, 
	NotEqual<T>,
	GetHashCode<T>);


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
