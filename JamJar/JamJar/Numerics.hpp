#pragma once

#include "Concepts.hpp"
#include "HashCode.hpp"
#include <limits>

template<std::signed_integral T>
class SignedInteger;

template<std::floating_point T>
class Float;

template<std::unsigned_integral T>
class UnsignedInteger
{
private:
	T m_value;
public:
	static const UnsignedInteger<T> Minimum;
	static const UnsignedInteger<T> Maximum;

	static const UnsignedInteger<T> Zero;
	static const UnsignedInteger<T> One;
	
	UnsignedInteger()                                : m_value(0)             {}
	UnsignedInteger(const UnsignedInteger<T>& other) : m_value(other.m_value) {}

	template<std::unsigned_integral T2>
	UnsignedInteger(T2 value) requires GreaterOrEqualSize<T, T2> : m_value((T)value) {}
	
	template<std::unsigned_integral T2>
	explicit UnsignedInteger(T2 value) requires SmallerSize<T, T2> : m_value((T)value) {}

	template<std::signed_integral T2>
	explicit UnsignedInteger(T2 value) : m_value((T)value) {}

	template<std::floating_point T2>
	explicit UnsignedInteger(T2 value) : m_value((T)value) {}

	T ToRawValue() const { return m_value; }

	template<std::unsigned_integral T2>
	operator UnsignedInteger<T2>() const requires SmallerOrEqualSize<T, T2> { return UnsignedInteger<T2>((T2)m_value); }

	template<std::signed_integral T2>
	operator SignedInteger<T2>() const requires SmallerSize<T, T2> { return SignedInteger<T2>((T2)m_value); }

	template<std::floating_point T2>
	operator Float<T2>() const { return Float<T2>((T2)m_value); }

	template<std::unsigned_integral T2>
	explicit operator UnsignedInteger<T2>() const requires GreaterSize<T, T2> { return UnsignedInteger<T2>((T2)m_value); }

	template<std::signed_integral T2>
	explicit operator SignedInteger<T2>() const requires GreaterOrEqualSize<T, T2> { return SignedInteger<T2>((T2)m_value); }

	friend UnsignedInteger<T> operator+(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value + right.m_value; }
	friend UnsignedInteger<T> operator-(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value - right.m_value; }
	friend UnsignedInteger<T> operator*(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value * right.m_value; }
	friend UnsignedInteger<T> operator/(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value / right.m_value; }
	friend UnsignedInteger<T> operator%(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value % right.m_value; }

	friend UnsignedInteger<T> operator&(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value & right.m_value; }
	friend UnsignedInteger<T> operator|(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value | right.m_value; }
	friend UnsignedInteger<T> operator^(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value ^ right.m_value; }

	friend UnsignedInteger<T> operator<<(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value << right.m_value; }
	friend UnsignedInteger<T> operator>>(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value >> right.m_value; }

	friend Boolean operator< (UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value <  right.m_value; }
	friend Boolean operator> (UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value >  right.m_value; }
	friend Boolean operator<=(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value <= right.m_value; }
	friend Boolean operator>=(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value >= right.m_value; }
	friend Boolean operator==(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value == right.m_value; }
	friend Boolean operator!=(UnsignedInteger<T> left, UnsignedInteger<T> right) { return left.m_value != right.m_value; }

	UnsignedInteger<T>& operator+=(UnsignedInteger<T> other) { m_value += other.m_value; return *this; }
	UnsignedInteger<T>& operator-=(UnsignedInteger<T> other) { m_value -= other.m_value; return *this; }
	UnsignedInteger<T>& operator*=(UnsignedInteger<T> other) { m_value *= other.m_value; return *this; }
	UnsignedInteger<T>& operator/=(UnsignedInteger<T> other) { m_value /= other.m_value; return *this; }
	UnsignedInteger<T>& operator%=(UnsignedInteger<T> other) { m_value %= other.m_value; return *this; }

	UnsignedInteger<T>& operator&=(UnsignedInteger<T> other) { m_value &= other.m_value; return *this; }
	UnsignedInteger<T>& operator|=(UnsignedInteger<T> other) { m_value |= other.m_value; return *this; }
	UnsignedInteger<T>& operator^=(UnsignedInteger<T> other) { m_value ^= other.m_value; return *this; }

	UnsignedInteger<T>& operator<<=(UnsignedInteger<T> other) { m_value <<= other.m_value; return *this; }
	UnsignedInteger<T>& operator>>=(UnsignedInteger<T> other) { m_value >>= other.m_value; return *this; }

	UnsignedInteger<T> operator~() const { return UnsignedInteger<T>(~m_value); }

	UnsignedInteger<T>& operator++() { ++m_value; return *this; }
	UnsignedInteger<T>& operator--() { --m_value; return *this; }

	UnsignedInteger<T> operator++(int) 
	{ 
		UnsignedInteger<T> result = *this;
		++(*this);
		return result;
	}

	UnsignedInteger<T> operator--(int)
	{ 
		UnsignedInteger<T> result = *this;
		--(*this);
		return result;
	}

	UnsignedInteger<T> ToRadians() const requires GreaterSize<T, unsigned char>; //{ return UnsignedInteger<T>((*this) * (Float32::PI / 180)); }
	UnsignedInteger<T> ToDegrees() const requires GreaterSize<T, unsigned char>; //{ return UnsignedInteger<T>((*this) * (180 / Float32::PI)); }

	UnsignedInteger<T> Sqrt() const { return UnsignedInteger<T>(sqrtf(m_value)); }

	UnsignedInteger<T> Min(const UnsignedInteger<T>& other) const { return m_value < other.m_value ? *this : other; }
	UnsignedInteger<T> Max(const UnsignedInteger<T>& other) const { return m_value > other.m_value ? *this : other; }

	UnsignedInteger<T> Clamp(const UnsignedInteger<T>& min, const UnsignedInteger<T>& max) const { return min.Max(this->Min(max)); }

	template<std::floating_point T2>
	UnsignedInteger<T> Pow(Float<T2> power) const { return UnsignedInteger<T>(powf(m_value, power.ToRawValue())); }

	HashCode GetHashCode() const { return HashCode(size_t(m_value)); }

	String ToString() const;
};

template<std::unsigned_integral T>
const UnsignedInteger<T> UnsignedInteger<T>::Minimum(0);

template<std::unsigned_integral T>
const UnsignedInteger<T> UnsignedInteger<T>::Maximum((1ULL << (sizeof(T) * 8ULL)) - 1ULL);

template<std::unsigned_integral T>
const UnsignedInteger<T> UnsignedInteger<T>::Zero(0);

template<std::unsigned_integral T>
const UnsignedInteger<T> UnsignedInteger<T>::One(1);

template<std::signed_integral T>
class SignedInteger
{
private:
	T m_value;
public:
	static const SignedInteger<T> Minimum;
	static const SignedInteger<T> Maximum;

	static const SignedInteger<T> Zero;
	static const SignedInteger<T> One;

	SignedInteger()                              : m_value(0)             {}
	SignedInteger(const SignedInteger<T>& other) : m_value(other.m_value) {}

	template<std::signed_integral T2>
	SignedInteger(T2 value) requires GreaterOrEqualSize<T, T2> : m_value((T)value) {}

	template<std::unsigned_integral T2>
	SignedInteger(T2 value) requires GreaterSize<T, T2> : m_value((T)value) {}

	template<std::signed_integral T2>
	explicit SignedInteger(T2 value) requires SmallerSize<T, T2> : m_value((T)value) {}

	template<std::unsigned_integral T2>
	explicit SignedInteger(T2 value) requires SmallerOrEqualSize<T, T2> : m_value((T)value) {}

	template<std::floating_point T2>
	explicit SignedInteger(T2 value) : m_value((T)value) {}

	T ToRawValue() const { return m_value; }
	
	template<std::signed_integral T2>
	operator SignedInteger<T2>() const requires SmallerOrEqualSize<T, T2> { return SignedInteger<T2>((T2)m_value); }

	template<std::floating_point T2>
	operator Float<T2>() const { return Float<T2>((T2)m_value); }

	template<std::signed_integral T2>
	explicit operator SignedInteger<T2>() const requires GreaterSize<T, T2> { return SignedInteger<T2>((T2)m_value); }

	template<std::unsigned_integral T2>
	explicit operator UnsignedInteger<T2>() const { return UnsignedInteger<T2>((T2)m_value); }

	friend SignedInteger<T> operator+(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value + right.m_value; }
	friend SignedInteger<T> operator-(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value - right.m_value; }
	friend SignedInteger<T> operator*(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value * right.m_value; }
	friend SignedInteger<T> operator/(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value / right.m_value; }
	friend SignedInteger<T> operator%(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value % right.m_value; }

	friend SignedInteger<T> operator&(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value & right.m_value; }
	friend SignedInteger<T> operator|(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value | right.m_value; }
	friend SignedInteger<T> operator^(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value ^ right.m_value; }

	friend SignedInteger<T> operator<<(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value << right.m_value; }
	friend SignedInteger<T> operator>>(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value >> right.m_value; }

	friend Boolean operator< (SignedInteger<T> left, SignedInteger<T> right) { return left.m_value <  right.m_value; }
	friend Boolean operator> (SignedInteger<T> left, SignedInteger<T> right) { return left.m_value >  right.m_value; }
	friend Boolean operator<=(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value <= right.m_value; }
	friend Boolean operator>=(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value >= right.m_value; }
	friend Boolean operator==(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value == right.m_value; }
	friend Boolean operator!=(SignedInteger<T> left, SignedInteger<T> right) { return left.m_value != right.m_value; }


	SignedInteger<T>& operator+=(SignedInteger<T> other) { m_value += other.m_value; return *this; }
	SignedInteger<T>& operator-=(SignedInteger<T> other) { m_value -= other.m_value; return *this; }
	SignedInteger<T>& operator*=(SignedInteger<T> other) { m_value *= other.m_value; return *this; }
	SignedInteger<T>& operator/=(SignedInteger<T> other) { m_value /= other.m_value; return *this; }
	SignedInteger<T>& operator%=(SignedInteger<T> other) { m_value %= other.m_value; return *this; }

	SignedInteger<T>& operator&=(SignedInteger<T> other) { m_value &= other.m_value; return *this; }
	SignedInteger<T>& operator|=(SignedInteger<T> other) { m_value |= other.m_value; return *this; }
	SignedInteger<T>& operator^=(SignedInteger<T> other) { m_value ^= other.m_value; return *this; }

	SignedInteger<T>& operator<<=(SignedInteger<T> other) { m_value <<= other.m_value; return *this; }
	SignedInteger<T>& operator>>=(SignedInteger<T> other) { m_value >>= other.m_value; return *this; }


	SignedInteger<T> operator+() const { return SignedInteger<T>(+m_value); }
	SignedInteger<T> operator-() const { return SignedInteger<T>(-m_value); }

	SignedInteger<T> operator~() const { return SignedInteger<T>(~m_value); }

	SignedInteger<T>& operator++() { ++m_value; return *this; }
	SignedInteger<T>& operator--() { --m_value; return *this; }

	SignedInteger<T> operator++(int)
	{
		SignedInteger<T> result = *this;
		++(*this);
		return result;
	}

	SignedInteger<T> operator--(int)
	{
		SignedInteger<T> result = *this;
		--(*this);
		return result;
	}

	SignedInteger<T> Abs() const
	{
		if(m_value < 0)
			return SignedInteger<T>(-m_value);

		return m_value;
	}

	SignedInteger<T> ToRadians() const requires GreaterSize<T, char>; //{ return SignedInteger<T>((*this) * (Float32::PI / 180)); }
	SignedInteger<T> ToDegrees() const requires GreaterSize<T, char>; //{ return SignedInteger<T>((*this) * (180 / Float32::PI)); }

	SignedInteger<T> Sqrt() const { return SignedInteger<T>(sqrtf(m_value)); }

	SignedInteger<T> Min(const SignedInteger<T>& other) const { return m_value < other.m_value ? *this : other; }
	SignedInteger<T> Max(const SignedInteger<T>& other) const { return m_value > other.m_value ? *this : other; }

	SignedInteger<T> Clamp(const SignedInteger<T>& min, const SignedInteger<T>& max) const { return min.Max(this->Min(max)); }

	template<std::floating_point T2>
	SignedInteger<T> Pow(Float<T2> power) const { return SignedInteger<T>(powf(m_value, power.ToRawValue())); }

	HashCode GetHashCode() const { return HashCode(size_t(m_value)); }

	String ToString() const;
};

template<std::signed_integral T>
const SignedInteger<T> SignedInteger<T>::Minimum(1LL << (sizeof(T) * 8ULL - 1LL));

template<std::signed_integral T>
const SignedInteger<T> SignedInteger<T>::Maximum((1LL << (sizeof(T) * 8ULL - 1LL)) - 1LL);

template<std::signed_integral T>
const SignedInteger<T> SignedInteger<T>::Zero(0);

template<std::signed_integral T>
const SignedInteger<T> SignedInteger<T>::One(1);

template<std::floating_point T>
class Float
{
private:
	T m_value;
public:
	static const Float<T> Minimum;
	static const Float<T> Maximum;

	static const Float<T> Zero;
	static const Float<T> One;

	static const Float<T> Epsilon;
	static const Float<T> PositiveInfinity;
	static const Float<T> NegativeInfinity;
	static const Float<T> NaN;

	static const Float<T> PI;

	Float()                      : m_value(0)             {}
	Float(const Float<T>& other) : m_value(other.m_value) {}

	template<std::floating_point T2>
	Float(T2 value) requires GreaterOrEqualSize<T, T2> : m_value(value) {}

	template<std::integral T2>
	Float(T2 value) : m_value(value) {}

	template<std::floating_point T2>
	explicit Float(T2 value) requires SmallerSize<T, T2> : m_value(value) {}

	T ToRawValue() const { return m_value; }

	template<std::floating_point T2>
	operator Float<T2>() const requires SmallerOrEqualSize<T, T2> { return Float<T2>((T2)m_value); }

	template<std::floating_point T2>
	explicit operator Float<T2>() const requires GreaterSize<T, T2> { return Float<T2>((T2)m_value); }

	template<std::unsigned_integral T2>
	explicit operator UnsignedInteger<T2>() const { return UnsignedInteger<T2>((T2)m_value); }

	template<std::signed_integral T2>
	explicit operator SignedInteger<T2>() const { return SignedInteger<T2>((T2)m_value); }

	Boolean IsInfinity()         const { return isinf(m_value);                }
	Boolean IsPositiveInfinity() const { return isinf(m_value) && m_value > 0; }
	Boolean IsNegativeInfinity() const { return isinf(m_value) && m_value < 0; }
	Boolean IsNaN()              const { return isnan(m_value);                }

	friend Float<T> operator+(Float<T> left, Float<T> right) { return left.m_value + right.m_value; }
	friend Float<T> operator-(Float<T> left, Float<T> right) { return left.m_value - right.m_value; }
	friend Float<T> operator*(Float<T> left, Float<T> right) { return left.m_value * right.m_value; }
	friend Float<T> operator/(Float<T> left, Float<T> right) { return left.m_value / right.m_value; }

	friend Float<T> operator%(Float<T> left, Float<T> right);

	friend Boolean operator< (Float<T> left, Float<T> right) { return left.m_value <  right.m_value; }
	friend Boolean operator> (Float<T> left, Float<T> right) { return left.m_value >  right.m_value; }
	friend Boolean operator<=(Float<T> left, Float<T> right) { return left.m_value <= right.m_value; }
	friend Boolean operator>=(Float<T> left, Float<T> right) { return left.m_value >= right.m_value; }
	friend Boolean operator==(Float<T> left, Float<T> right) { return left.m_value == right.m_value; }
	friend Boolean operator!=(Float<T> left, Float<T> right) { return left.m_value != right.m_value; }

	Float<T>& operator+=(Float<T> other) { m_value += other.m_value; return *this; }
	Float<T>& operator-=(Float<T> other) { m_value -= other.m_value; return *this; }
	Float<T>& operator*=(Float<T> other) { m_value *= other.m_value; return *this; }
	Float<T>& operator/=(Float<T> other) { m_value /= other.m_value; return *this; }
	Float<T>& operator%=(Float<T> other) { m_value %= other.m_value; return *this; }

	Float<T> operator+() const { return SignedInteger<T>(+m_value); }
	Float<T> operator-() const { return SignedInteger<T>(-m_value); }

	Float<T>& operator++() { ++m_value; return *this; }
	Float<T>& operator--() { --m_value; return *this; }

	Float<T> operator++(int)
	{
		Float<T> result = *this;
		++(*this);
		return result;
	}

	Float<T> operator--(int)
	{
		Float<T> result = *this;
		--(*this);
		return result;
	}

	Float<T> Min(const Float<T>& other) const { return m_value < other.m_value ? *this : other; }
	Float<T> Max(const Float<T>& other) const { return m_value > other.m_value ? *this : other; }

	Float<T> Clamp(const Float<T>& min, const Float<T>& max) const { return min.Max(this->Min(max)); }

	Float<T> Abs() const
	{
		if(m_value < 0)
			return SignedInteger<T>(-m_value);

		return m_value;
	}

	Float<T> Sqrt() const;
	Float<T> Pow(Float<T> power) const;

	Float<T> ToRadians() const { return m_value * (PI / 180); }
	Float<T> ToDegrees() const { return m_value * (180 / PI); }
	
	Float<T> Floor();
	Float<T> Ceiling();
	Float<T> Truncate();
	Float<T> Round();

	Float<T> Sin();
	Float<T> Cos();
	Float<T> Tan();

	Float<T> SinH();
	Float<T> CosH();
	Float<T> TanH();

	Float<T> ASin();
	Float<T> ACos();
	Float<T> ATan();

	Float<T> ASinH();
	Float<T> ACosH();
	Float<T> ATanH();

	Float<T> ATan2(Float<T> x);

	HashCode GetHashCode() const
	{
		double value = m_value;
		uint64_t hash = *(uint64_t*)&value;
		return HashCode(size_t(hash));
	}

	String ToString() const;
};

template<>
const Float<float> Float<float>::Minimum(-FLT_MAX);

template<>
const Float<float> Float<float>::Maximum(+FLT_MAX);

template<>
const Float<float> Float<float>::Zero(0.0f);

template<>
const Float<float> Float<float>::One(1.0f);

template<>
const Float<float> Float<float>::Epsilon(+FLT_MIN);

template<>
const Float<double> Float<double>::Minimum(-DBL_MAX);

template<>
const Float<double> Float<double>::Maximum(+DBL_MAX);

template<>
const Float<double> Float<double>::Epsilon(+DBL_MIN);

template<>
const Float<double> Float<double>::Zero(0.0);

template<>
const Float<double> Float<double>::One(1.0);


template<std::floating_point T>
const Float<T> Float<T>::NaN(std::numeric_limits<T>::signaling_NaN());

template<std::floating_point T>
const Float<T> Float<T>::PositiveInfinity(+std::numeric_limits<T>::infinity());

template<std::floating_point T>
const Float<T> Float<T>::NegativeInfinity(-std::numeric_limits<T>::infinity());

template<std::floating_point T>
const Float<T> Float<T>::PI((T)3.1415926535897932384626433832795);

using UInt8  = UnsignedInteger<uint8_t>;
using UInt16 = UnsignedInteger<uint16_t>;
using UInt32 = UnsignedInteger<uint32_t>;
using UInt64 = UnsignedInteger<uint64_t>;

using Size = UnsignedInteger<size_t>;

using SInt8 =  SignedInteger<int8_t>;
using SInt16 = SignedInteger<int16_t>;
using SInt32 = SignedInteger<int32_t>;
using SInt64 = SignedInteger<int64_t>;

using Float32 = Float<float>;
using Float64 = Float<double>;

template<std::unsigned_integral T>
UnsignedInteger<T> UnsignedInteger<T>::ToRadians() const requires GreaterSize<T, unsigned char> { return UnsignedInteger<T>((*this) * (Float32::PI / 180)); }

template<std::unsigned_integral T>
UnsignedInteger<T> UnsignedInteger<T>::ToDegrees() const requires GreaterSize<T, unsigned char> { return UnsignedInteger<T>((*this) * (180 / Float32::PI)); }

template<std::signed_integral T>
SignedInteger<T> SignedInteger<T>::ToRadians() const requires GreaterSize<T, char> { return SignedInteger<T>((*this) * (Float32::PI / 180)); }

template<std::signed_integral T>
SignedInteger<T> SignedInteger<T>::ToDegrees() const requires GreaterSize<T, char> { return SignedInteger<T>((*this) * (180 / Float32::PI)); }



//class UInt8
//{
//private:
//	uint8_t m_value;
//public:
//	UInt8(uint8_t value = 0)  : m_value(value)         {}
//	UInt8(const UInt8& other) : m_value(other.m_value) {}
//
//	friend UInt8 operator+(UInt8 left, UInt8 right) { return left.m_value + right.m_value; }
//	friend UInt8 operator-(UInt8 left, UInt8 right) { return left.m_value - right.m_value; }
//	friend UInt8 operator*(UInt8 left, UInt8 right) { return left.m_value * right.m_value; }
//	friend UInt8 operator/(UInt8 left, UInt8 right) { return left.m_value / right.m_value; }
//	friend UInt8 operator%(UInt8 left, UInt8 right) { return left.m_value % right.m_value; }
//
//	friend UInt8 operator&(UInt8 left, UInt8 right) { return left.m_value & right.m_value; }
//	friend UInt8 operator|(UInt8 left, UInt8 right) { return left.m_value | right.m_value; }
//	friend UInt8 operator^(UInt8 left, UInt8 right) { return left.m_value ^ right.m_value; }
//
//	friend Boolean operator< (UInt8 left, UInt8 right) { return left.m_value <  right.m_value; }
//	friend Boolean operator> (UInt8 left, UInt8 right) { return left.m_value >  right.m_value; }
//	friend Boolean operator<=(UInt8 left, UInt8 right) { return left.m_value <= right.m_value; }
//	friend Boolean operator>=(UInt8 left, UInt8 right) { return left.m_value >= right.m_value; }
//	friend Boolean operator==(UInt8 left, UInt8 right) { return left.m_value == right.m_value; }
//	friend Boolean operator!=(UInt8 left, UInt8 right) { return left.m_value != right.m_value; }
//
//	UInt8 operator~() const { return UInt8(~m_value); }
//
//	UInt8& operator++() { ++m_value; return *this; }
//	UInt8& operator--() { --m_value; return *this; }
//};