#pragma once

#include "../String.hpp"

class IVector
{
};

template<Number T, size_t D>
class Vector : public IVector
{
private:
	StackBlock<T, D> m_values;
public:
	Vector(T value = T::Zero)
	{
		for(Size i = 0U; i < D; i++)
			m_values[i] = value;
	}

	template<ConvertibleTo<T>... Args>
	Vector(Args... args) requires Contains<Args..., D> : m_values { args... } {}

	Size Dimensions() const { return D; }

	      T& operator[](Size index)       { return m_values[index]; }
	const T& operator[](Size index) const { return m_values[index]; }

	T LengthSquared() const
	{
		T result = T::Zero;
		for(Size i = 0; i < D; i++)
			result += m_values[i] * m_values[i];

		return result;
	}

	T Length() const { return LengthSquared().Sqrt(); }

	T Dot(const Vector<T, D>& other) const
	{
		T result = T::Zero;
		for(Size i = 0; i < D; i++)
			result += m_values[i] * other[i];
	}

	Vector<T, D> Normalize() const
	{
		Vector<T, D> result;
		for(Size i = 0; i < D; i++)
			result[i] = m_values[i] / Length();

		return result;
	}

	friend Vector<T, D> operator+(const Vector<T, D>& left, const Vector<T, D>& right)
	{
		Vector<T, D> result;
		for(Size i = 0U; i < D; i++)
			result[i] = left[i] + right[i];

		return result;
	}

	friend Vector<T, D> operator-(const Vector<T, D>& left, const Vector<T, D>& right)
	{
		Vector<T, D> result;
		for(Size i = 0U; i < D; i++)
			result[i] = left[i] - right[i];

		return result;
	}

	friend Vector<T, D> operator*(const Vector<T, D>& left, const Vector<T, D>& right)
	{
		Vector<T, D> result;
		for(Size i = 0U; i < D; i++)
			result[i] = left[i] * right[i];

		return result;
	}

	friend Vector<T, D> operator/(const Vector<T, D>& left, const Vector<T, D>& right)
	{
		Vector<T, D> result;
		for(Size i = 0; i < D; i++)
			result[i] = left[i] / right[i];

		return result;
	}

	friend Vector<T, D> operator+(const Vector<T, D>& left, T right) { return left + Vector<T, D>(right); }
	friend Vector<T, D> operator-(const Vector<T, D>& left, T right) { return left - Vector<T, D>(right); }
	friend Vector<T, D> operator*(const Vector<T, D>& left, T right) { return left * Vector<T, D>(right); }
	friend Vector<T, D> operator/(const Vector<T, D>& left, T right) { return left / Vector<T, D>(right); }

	friend Vector<T, D> operator+(T left, const Vector<T, D>& right) { return Vector<T, D>(left) + right; }
	friend Vector<T, D> operator-(T left, const Vector<T, D>& right) { return Vector<T, D>(left) - right; }
	friend Vector<T, D> operator*(T left, const Vector<T, D>& right) { return Vector<T, D>(left) * right; }
	friend Vector<T, D> operator/(T left, const Vector<T, D>& right) { return Vector<T, D>(left) / right; }

	friend Boolean operator==(const Vector<T, D>& left, const Vector<T, D>& right)
	{
		for(Size i = 0U; i < D; i++)
		{
			if(left[i] != right[i])
				return false;
		}
		return true;
	}

	friend Boolean operator!=(const Vector<T, D>& left, const Vector<T, D>& right) { return !(left == right); }


	Vector<T, D>& operator+=(const Vector<T, D>& other)
	{
		for(Size i = 0U; i < D; i++)
			m_values[i] += other[i];

		return *this;
	}

	Vector<T, D>& operator-=(const Vector<T, D>& other)
	{
		for(Size i = 0; i < D; i++)
			m_values[i] -= other[i];

		return *this;
	}

	Vector<T, D>& operator*=(const Vector<T, D>& other)
	{
		for(Size i = 0; i < D; i++)
			m_values[i] *= other[i];

		return *this;
	}

	Vector<T, D>& operator/=(const Vector<T, D>& other)
	{
		for(Size i = 0; i < D; i++)
			m_values[i] /= other[i];

		return *this;
	}

	Vector<T, D>& operator+=(T other) { return (*this) += Vector<T, D>(other); }
	Vector<T, D>& operator-=(T other) { return (*this) -= Vector<T, D>(other); }
	Vector<T, D>& operator*=(T other) { return (*this) *= Vector<T, D>(other); }
	Vector<T, D>& operator/=(T other) { return (*this) /= Vector<T, D>(other); }

	Vector<T, D> operator+() const requires SignedNumber<T> { return  *this;          }
	Vector<T, D> operator-() const requires SignedNumber<T> { return (*this) * T(-1); }
	
	String ToString() const requires Printable<T>
	{
		MutableString result;
		result += MutableString("Vector") += MutableString(Size(D).ToString()) += MutableString("(");

		for(T value : m_values)
			result += MutableString(value.ToString()) += MutableString(", ");

		result.TrimEnd(2U);
		result += ")";

		return result.ToString();
	}
};

template<Number T>
class Vector2 : public Vector<T, 2>
{
public:
	Vector2(T value = T::Zero) : Vector<T, 2>(value) {}
	Vector2(T x, T y)          : Vector<T, 2>(x, y ) {}

	Vector2(const Vector<T, 2>& other) : Vector<T, 2>(other) {}

	T GetX() const { return (*this)[0U]; }
	T GetY() const { return (*this)[1U]; }

	void SetX(T x) { (*this)[0U] = x; }
	void SetY(T y) { (*this)[1U] = y; }

	T Cross(const Vector2& other) const { return GetX() * other.GetY() + GetY() * other.GetX(); }

	Vector2 Rotate(T angle) const requires FloatingPoint<T> 
	{
		return Vector2(angle.Cos() * GetX() + angle.Sin() * GetY(), angle.Sin() * GetX() + angle.Cos() * GetY());
	}
};

template<Number T>
class Vector3 : public Vector<T, 3>
{
public:
	Vector3(T value = T::Zero) : Vector<T, 3>(value  ) {}
	Vector3(T x, T y, T z)     : Vector<T, 3>(x, y, z) {}

	Vector3(const Vector2<T>& xy, T z) : Vector3(xy.GetX(), xy.GetY(), z) {}
	Vector3(T x, const Vector2<T>& yz) : Vector3(x, yz.GetX(), yz.GetY()) {}

	Vector3(const Vector<T, 3>& other) : Vector<T, 3>(other) {}

	T GetX() const { return (*this)[0U]; }
	T GetY() const { return (*this)[1U]; }
	T GetZ() const { return (*this)[2U]; }

	void SetX(T x) { (*this)[0U] = x; }
	void SetY(T y) { (*this)[1U] = y; }
	void SetZ(T z) { (*this)[2U] = z; }

	Vector2<T> GetXY() const { return Vector2<T>(GetX(), GetY()); }
	Vector2<T> GetXZ() const { return Vector2<T>(GetX(), GetZ()); }

	Vector2<T> GetYX() const { return Vector2<T>(GetY(), GetX()); }
	Vector2<T> GetYZ() const { return Vector2<T>(GetY(), GetZ()); }

	Vector2<T> GetZX() const { return Vector2<T>(GetZ(), GetX()); }
	Vector2<T> GetZY() const { return Vector2<T>(GetZ(), GetY()); }

	Vector3<T> Cross(const Vector3<T>& other) const
	{
		float x = GetY() * other.GetZ() - GetZ() * other.GetY();
		float y = GetZ() * other.GetX() - GetX() * other.GetZ();
		float z = GetX() * other.GetY() - GetY() * other.GetX();
		return Vector3<T>(x, y, z);
	}

	Vector3<T> Rotate(const Vector3<T>& axis, T angle) const requires FloatingPoint<T>
	{
		T rad      = angle.ToRadians();
        T sinAngle = -rad.Sin();
        T cosAngle = -rad.Cos();

        return Cross(axis * sinAngle) + ((*this) * cosAngle + (axis * Dot(axis * (1 - cosAngle))));
	}

	//Vector3<T> Rotate(const Quaternion<T>& rotation);
};

template<Number T>
class Vector4 : public Vector<T, 4>
{
public:
	Vector4(T value = T::Zero ) : Vector<T, 4>(value     ) {}
	Vector4(T x, T y, T z, T w) : Vector<T, 4>(x, y, z, w) {}

	Vector4(const Vector2<T>& xy, T z, T w) : Vector4(xy.GetX(), xy.GetY(), z, w) {}
	Vector4(T x, const Vector2<T>& yz, T w) : Vector4(x, yz.GetX(), yz.GetY(), w) {}
	Vector4(T x, T y, const Vector2<T>& zw) : Vector4(x, y, zw.GetX(), zw.GetY()) {}

	Vector4(const Vector2<T>& xy, const Vector2<T>& zw) : Vector4(xy.GetX(), xy.GetY(), zw.GetX(), zw.GetY()) {}

	Vector4(const Vector3<T>& xyz, T w) : Vector4(xyz.GetX(), xyz.GetY(), xyz.GetZ(), w) {}
	Vector4(T x, const Vector3<T>& yzw) : Vector4(x, yzw.GetX(), yzw.GetY(), yzw.GetZ()) {}

	Vector4(const Vector<T, 4>& other) : Vector<T, 4>(other) {}

	T GetX() const { return (*this)[0U]; }
	T GetY() const { return (*this)[1U]; }
	T GetZ() const { return (*this)[2U]; }
	T GetW() const { return (*this)[3U]; }

	void SetX(T x) { (*this)[0U] = x; }
	void SetY(T y) { (*this)[1U] = y; }
	void SetZ(T z) { (*this)[2U] = z; }
	void SetW(T w) { (*this)[3U] = w; }


	Vector2<T> GetXY() const { return Vector2<T>(GetX(), GetY()); }
	Vector2<T> GetXZ() const { return Vector2<T>(GetX(), GetZ()); }
	Vector2<T> GetXW() const { return Vector2<T>(GetX(), GetW()); }

	Vector2<T> GetYX() const { return Vector2<T>(GetY(), GetX()); }
	Vector2<T> GetYZ() const { return Vector2<T>(GetY(), GetZ()); }
	Vector2<T> GetYW() const { return Vector2<T>(GetY(), GetW()); }

	Vector2<T> GetZX() const { return Vector2<T>(GetZ(), GetX()); }
	Vector2<T> GetZY() const { return Vector2<T>(GetZ(), GetY()); }
	Vector2<T> GetZW() const { return Vector2<T>(GetZ(), GetW()); }
	
	Vector2<T> GetWX() const { return Vector2<T>(GetW(), GetX()); }
	Vector2<T> GetWY() const { return Vector2<T>(GetW(), GetY()); }
	Vector2<T> GetWZ() const { return Vector2<T>(GetW(), GetZ()); }


	Vector3<T> GetXYZ() const { return Vector3<T>(GetX(), GetY(), GetZ()); }
	Vector3<T> GetXYW() const { return Vector3<T>(GetX(), GetY(), GetW()); }

	Vector3<T> GetXZY() const { return Vector3<T>(GetX(), GetZ(), GetY()); }
	Vector3<T> GetXZW() const { return Vector3<T>(GetX(), GetZ(), GetW()); }

	Vector3<T> GetXWY() const { return Vector3<T>(GetX(), GetW(), GetY()); }
	Vector3<T> GetXWZ() const { return Vector3<T>(GetX(), GetW(), GetZ()); }


	Vector3<T> GetYXZ() const { return Vector3<T>(GetY(), GetX(), GetZ()); }
	Vector3<T> GetYXW() const { return Vector3<T>(GetY(), GetX(), GetW()); }

	Vector3<T> GetYZX() const { return Vector3<T>(GetY(), GetZ(), GetX()); }
	Vector3<T> GetYZW() const { return Vector3<T>(GetY(), GetZ(), GetW()); }

	Vector3<T> GetYWX() const { return Vector3<T>(GetY(), GetW(), GetX()); }
	Vector3<T> GetYWZ() const { return Vector3<T>(GetY(), GetW(), GetZ()); }


	Vector3<T> GetZXY() const { return Vector3<T>(GetZ(), GetX(), GetY()); }
	Vector3<T> GetZXW() const { return Vector3<T>(GetZ(), GetX(), GetW()); }

	Vector3<T> GetZYX() const { return Vector3<T>(GetZ(), GetY(), GetX()); }
	Vector3<T> GetZYW() const { return Vector3<T>(GetZ(), GetY(), GetW()); }

	Vector3<T> GetZWX() const { return Vector3<T>(GetZ(), GetW(), GetX()); }
	Vector3<T> GetZWY() const { return Vector3<T>(GetZ(), GetW(), GetY()); }


	Vector3<T> GetWXY() const { return Vector3<T>(GetW(), GetX(), GetY()); }
	Vector3<T> GetWXZ() const { return Vector3<T>(GetW(), GetX(), GetZ()); }

	Vector3<T> GetWYX() const { return Vector3<T>(GetW(), GetY(), GetX()); }
	Vector3<T> GetWYZ() const { return Vector3<T>(GetW(), GetY(), GetZ()); }

	Vector3<T> GetWZX() const { return Vector3<T>(GetW(), GetZ(), GetX()); }
	Vector3<T> GetWZY() const { return Vector3<T>(GetW(), GetZ(), GetY()); }
};

using Vector2UI = Vector2<UInt32>;
using Vector3UI = Vector3<UInt32>;
using Vector4UI = Vector4<UInt32>;

using Vector2SI = Vector2<SInt32>;
using Vector3SI = Vector3<SInt32>;
using Vector4SI = Vector4<SInt32>;

using Vector2F32 = Vector2<Float32>;
using Vector3F32 = Vector3<Float32>;
using Vector4F32 = Vector4<Float32>;

using Vector2F64 = Vector2<Float64>;
using Vector3F64 = Vector3<Float64>;
using Vector4F64 = Vector4<Float64>;
