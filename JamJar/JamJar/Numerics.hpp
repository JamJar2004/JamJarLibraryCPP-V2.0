#pragma once

#include "Concepts.hpp"

template<std::signed_integral T>
class SignedInteger;

template<std::unsigned_integral T>
class UnsignedInteger
{
private:
	T m_value;
public:
	static const UnsignedInteger<T> Minimum;
	static const UnsignedInteger<T> Maximum;

	UnsignedInteger(T value = 0)                     : m_value(value)         {}
	UnsignedInteger(const UnsignedInteger<T>& other) : m_value(other.m_value) {}

	template<GreaterOrEqualSize<T> T2>
	operator UnsignedInteger<T2>() const { return UnsignedInteger<T2>((T2)m_value); }

	template<SmallerOrEqualSize<T> T2>
	explicit operator UnsignedInteger<T2>() const { return UnsignedInteger<T2>((T2)m_value); }

	template<GreaterOrEqualSize<T> T2>
	operator SignedInteger<T2>() const { return SignedInteger<T2>((T2)m_value); }


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

	UnsignedInteger<T> operator~() const { return UnsignedInteger<T>(~m_value); }

	UnsignedInteger<T>& operator++() { ++m_value; return *this; }
	UnsignedInteger<T>& operator--() { --m_value; return *this; }
};

template<std::signed_integral T>
class SignedInteger
{
private:
	T m_value;
public:
	static const SignedInteger<T> Minimum;
	static const SignedInteger<T> Maximum;

	SignedInteger(T value = 0)                   : m_value(value)         {}
	SignedInteger(const SignedInteger<T>& other) : m_value(other.m_value) {}

	template<GreaterOrEqualSize<T> T2>
	operator SignedInteger<T2>() const { return SignedInteger<T2>((T2)m_value); }

	template<SmallerOrEqualSize<T> T2>
	explicit operator SignedInteger<T2>() const { return SignedInteger<T2>((T2)m_value); }

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

	SignedInteger<T> operator+() const { return SignedInteger<T>(+m_value); }
	SignedInteger<T> operator-() const { return SignedInteger<T>(-m_value); }

	SignedInteger<T> operator~() const { return SignedInteger<T>(~m_value); }

	SignedInteger<T>& operator++() { ++m_value; return *this; }
	SignedInteger<T>& operator--() { --m_value; return *this; }
};


using UInt8  = UnsignedInteger<uint8_t>;
using UInt16 = UnsignedInteger<uint16_t>;
using UInt32 = UnsignedInteger<uint32_t>;
using UInt64 = UnsignedInteger<uint64_t>;

using Size = UnsignedInteger<size_t>;

using SInt8 =  SignedInteger<int8_t>;
using SInt16 = SignedInteger<int16_t>;
using SInt32 = SignedInteger<int32_t>;
using SInt64 = SignedInteger<int64_t>;



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