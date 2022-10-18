#pragma once

#include <concepts>

class Boolean;

template<typename T1, typename T2>
concept SameAs = std::is_same_v<T1, T2>;

template<typename Derived, typename Base>
concept Inherits = std::is_base_of_v<Base, Derived>;

template<typename T>
concept Abstract = std::is_abstract_v<T>;

template<typename T>
concept Defined = !std::is_abstract_v<T>;

template<typename T>
concept DefaultConstructible = std::is_default_constructible_v<T>;

template<typename T>
concept CopyConstructible = std::is_copy_constructible_v<T>;

template<typename T>
concept MoveConstructible = std::is_move_constructible_v<T>;

template<typename T, typename... Args>
concept ConstructibleFrom = std::is_constructible_v<T, Args...>;

template<typename T>
concept Destructible = std::is_destructible_v<T>;

template<typename T>
concept CopyAssignable = std::is_copy_assignable_v<T>;

template<typename T>
concept MoveAssignable = std::is_move_assignable_v<T>;

template<typename To, typename From>
concept AssignableFrom = std::is_assignable_v<To, From>;

template<typename From, typename To>
concept ConvertibleTo = std::is_convertible_v<From, To>;

template<typename... Types, size_t Size>
concept Contains = sizeof...(Types) == Size;

template<typename T1, typename T2>
concept SmallerSize = sizeof(T1) < sizeof(T2);

template<typename T1, typename T2>
concept GreaterSize = sizeof(T1) > sizeof(T2);

template<typename T1, typename T2>
concept SmallerOrEqualSize = sizeof(T1) <= sizeof(T2);

template<typename T1, typename T2>
concept GreaterOrEqualSize = sizeof(T1) >= sizeof(T2);

template<typename T1, typename T2>
concept SameSize = sizeof(T1) == sizeof(T2);

template<typename T>
concept Equatable = requires(T left, T right)
{
	{ left == right } -> ConvertibleTo<Boolean>;
	{ left != right } -> ConvertibleTo<Boolean>;
};

template<typename T>
concept Comparable = Equatable<T> && requires(T left, T right)
{
	{ left <  right } -> ConvertibleTo<Boolean>;
	{ left >  right } -> ConvertibleTo<Boolean>;
	{ left <= right } -> ConvertibleTo<Boolean>;
	{ left >= right } -> ConvertibleTo<Boolean>;
};

template<typename T>
concept Addable       = requires(T left, T right) { { left + right } -> SameAs<T>; };
template<typename T>
concept Subtractable  = requires(T left, T right) { { left - right } -> SameAs<T>; };
template<typename T>
concept Multiplicable = requires(T left, T right) { { left * right } -> SameAs<T>; };
template<typename T>
concept Divisible     = requires(T left, T right) { { left / right } -> SameAs<T>; };
template<typename T>
concept Modable       = requires(T left, T right) { { left % right } -> SameAs<T>; };
//
//template<typename T, typename T2>
//concept Addable       = requires(T left, T2 right) { { left + right } -> SameAs<T>; };
//template<typename T, typename T2>
//concept Subtractable  = requires(T left, T2 right) { { left - right } -> SameAs<T>; };
//template<typename T, typename T2>
//concept Multiplicable = requires(T left, T2 right) { { left * right } -> SameAs<T>; };
//template<typename T, typename T2>
//concept Divisible     = requires(T left, T2 right) { { left / right } -> SameAs<T>; };
//template<typename T, typename T2>
//concept Modable       = requires(T left, T2 right) { { left % right } -> SameAs<T>; };

template<typename T>
concept BitwiseAnd = requires(T left, T right) { { left & right } -> SameAs<T>; };
template<typename T>
concept BitwiseOr  = requires(T left, T right) { { left | right } -> SameAs<T>; };
template<typename T>
concept BitwiseXor = requires(T left, T right) { { left ^ right } -> SameAs<T>; };

template<typename T>
concept LeftShiftable  = requires(T left, T right) { { left << right } -> SameAs<T>; };
template<typename T>
concept RightShiftable = requires(T left, T right) { { left >> right } -> SameAs<T>; };

template<typename T>
concept BitwiseNot = requires(T obj) { { ~obj } -> SameAs<T>; };

template<typename T>
concept Signed = requires(T obj) 
{
	{ +obj } -> SameAs<T>;
	{ -obj } -> SameAs<T>;
};

template<typename T>
concept PreIncrementable = requires(T obj) { { ++obj } -> SameAs<T&>; };
template<typename T>
concept PreDecrementable = requires(T obj) { { --obj } -> SameAs<T&>; };

template<typename T>
concept PostIncrementable = requires(T obj) { { obj++ } -> SameAs<T>; };
template<typename T>
concept PostDecrementable = requires(T obj) { { obj++ } -> SameAs<T>; };

template<typename T>
concept Incrementable = PreIncrementable<T> && PostIncrementable<T>;
template<typename T>
concept Decrementable = PreDecrementable<T> && PostDecrementable<T>;

template<typename T, typename I>
concept ConstIndexable = requires(T obj, I index) { { obj[index] } -> SameAs<const T&>; };

template<typename T, typename I>
concept Indexable = ConstIndexable<T, I> && requires(T obj, I index) { { obj[index] } -> SameAs<T&>; };

template<typename T>
concept Pointer = std::is_pointer_v<T>;

template<typename T>
concept Number = Addable<T> && Subtractable<T> && Multiplicable<T> && Divisible<T> && Modable<T> && Incrementable<T> && Decrementable<T> && Comparable<T> && 
requires
{
	{ T::Minimum };
	{ T::Maximum };

	{ T::Zero };
	{ T::One  };
} && 
requires(T obj)
{
	{ obj.Sqrt() } -> SameAs<T>;
};

template<typename T>
concept SignedNumber = Number<T> && Signed<T> && requires(T obj)
{
	{ obj.Abs() } -> SameAs<T>;
};

template<typename T>
concept Integral = Number<T> && BitwiseAnd<T> && BitwiseOr<T> && BitwiseXor<T> && BitwiseNot<T> && LeftShiftable<T> && RightShiftable<T>;

template<typename T>
concept UnsignedIntegral = Integral<T> && !SignedNumber<T>;

template<typename T>
concept SignedIntegral = Integral<T> && SignedNumber<T>;

template<typename T>
concept FloatingPoint = SignedNumber<T> && requires
{
	{ T::Epsilon		  };
	{ T::PositiveInfinity };
	{ T::NegativeInfinity };
	{ T::NaN			  };

	{ T::PI				  };
} &&
requires(T obj)
{
	{ obj.IsInfinity()         } -> ConvertibleTo<Boolean>;
	{ obj.IsPositiveInfinity() } -> ConvertibleTo<Boolean>;
	{ obj.IsNegativeInfinity() } -> ConvertibleTo<Boolean>;
	{ obj.IsNaN()              } -> ConvertibleTo<Boolean>;

	{ obj.Floor()    } -> SameAs<T>;
	{ obj.Ceiling()  } -> SameAs<T>;
	{ obj.Truncate() } -> SameAs<T>;
	{ obj.Round()    } -> SameAs<T>;

	{ obj.Sin() } -> SameAs<T>;
	{ obj.Cos() } -> SameAs<T>;
	{ obj.Tan() } -> SameAs<T>;

	{ obj.ASin() } -> SameAs<T>;
	{ obj.ACos() } -> SameAs<T>;
	{ obj.ATan() } -> SameAs<T>;

	{ obj.SinH() } -> SameAs<T>;
	{ obj.CosH() } -> SameAs<T>;
	{ obj.TanH() } -> SameAs<T>;

	{ obj.ASinH() } -> SameAs<T>;
	{ obj.ACosH() } -> SameAs<T>;
	{ obj.ATanH() } -> SameAs<T>;
} &&
requires(T obj, T arg)
{
	{ obj.ATan2(arg) } -> SameAs<T>;
};

class String;

template<typename T>
concept Printable = requires(T obj)
{
	{ obj.ToString() } -> SameAs<String>;
};

class HashCode;

template<typename T>
concept Hashable = requires(T obj)
{
	{ obj.GetHashCode() } -> SameAs<HashCode>;
};

template<typename T>
concept Iterator = Incrementable<T> && Decrementable<T> && Equatable<T>;

template<typename T>
concept Iterable = requires(T obj)
{
	{ obj.begin() } -> Iterator;
	{ obj.end()   } -> Iterator;
};