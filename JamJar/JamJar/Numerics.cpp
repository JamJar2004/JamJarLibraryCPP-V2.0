#include "Numerics.hpp"

HashCode::HashCode(UnsignedInteger<size_t> value) : m_value(value.ToRawValue()) {}

Float<float>  operator%(Float<float>  left, Float<float>  right) { return fmodf(left.m_value, right.m_value); }
Float<double> operator%(Float<double> left, Float<double> right) { return fmod (left.m_value, right.m_value); }

template<>
Float<float> Float<float>::Sqrt() const { return Float<float>(sqrtf(m_value)); }

template<>
Float<float> Float<float>::Pow(Float<float> power) const { return Float<float>(powf(m_value, power.m_value)); }

template<>
Float<float> Float<float>::Floor() { return Float<float>(floorf(m_value)); }
template<>
Float<float> Float<float>::Ceiling() { return Float<float>(ceilf(m_value)); }
template<>
Float<float> Float<float>::Truncate() { return Float<float>(truncf(m_value)); }
template<>
Float<float> Float<float>::Round() { return Float<float>(roundf(m_value)); }

template<>
Float<float> Float<float>::Sin() { return Float<float>(sinf(m_value)); }
template<>
Float<float> Float<float>::Cos() { return Float<float>(cosf(m_value)); }
template<>
Float<float> Float<float>::Tan() { return Float<float>(tanf(m_value)); }

template<>
Float<float> Float<float>::SinH() { return Float<float>(sinhf(m_value)); }
template<>
Float<float> Float<float>::CosH() { return Float<float>(coshf(m_value)); }
template<>
Float<float> Float<float>::TanH() { return Float<float>(tanhf(m_value)); }

template<>
Float<float> Float<float>::ASin() { return Float<float>(asinf(m_value)); }
template<>
Float<float> Float<float>::ACos() { return Float<float>(acosf(m_value)); }
template<>
Float<float> Float<float>::ATan() { return Float<float>(atanf(m_value)); }

template<>
Float<float> Float<float>::ASinH() { return Float<float>(asinhf(m_value)); }
template<>
Float<float> Float<float>::ACosH() { return Float<float>(acoshf(m_value)); }
template<>
Float<float> Float<float>::ATanH() { return Float<float>(atanhf(m_value)); }

template<>
Float<float> Float<float>::ATan2(Float<float> x) { return Float<float>(atan2f(m_value, x.ToRawValue())); }

template<>
Float<double> Float<double>::Sqrt() const { return Float<double>(sqrt(m_value)); }

template<>
Float<double> Float<double>::Pow(Float<double> power) const { return Float<double>(pow(m_value, power.m_value)); }


template<>
Float<double> Float<double>::Floor() { return Float<double>(floor(m_value)); }
template<>
Float<double> Float<double>::Ceiling() { return Float<double>(ceil(m_value)); }
template<>
Float<double> Float<double>::Truncate() { return Float<double>(trunc(m_value)); }
template<>
Float<double> Float<double>::Round() { return Float<double>(round(m_value)); }


template<>
Float<double> Float<double>::Sin() { return Float<double>(sin(m_value)); }
template<>
Float<double> Float<double>::Cos() { return Float<double>(cos(m_value)); }
template<>
Float<double> Float<double>::Tan() { return Float<double>(tan(m_value)); }

template<>
Float<double> Float<double>::SinH() { return Float<double>(sinh(m_value)); }
template<>
Float<double> Float<double>::CosH() { return Float<double>(cosh(m_value)); }
template<>
Float<double> Float<double>::TanH() { return Float<double>(tanh(m_value)); }

template<>
Float<double> Float<double>::ASin() { return Float<double>(asin(m_value)); }
template<>
Float<double> Float<double>::ACos() { return Float<double>(acos(m_value)); }
template<>
Float<double> Float<double>::ATan() { return Float<double>(atan(m_value)); }

template<>
Float<double> Float<double>::ASinH() { return Float<double>(asinh(m_value)); }
template<>
Float<double> Float<double>::ACosH() { return Float<double>(acosh(m_value)); }
template<>
Float<double> Float<double>::ATanH() { return Float<double>(atanh(m_value)); }

template<>
Float<double> Float<double>::ATan2(Float<double> x) { return Float<double>(atan2(m_value, x.ToRawValue())); }
