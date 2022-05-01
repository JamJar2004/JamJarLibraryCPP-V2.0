#pragma once

#include "../Memory/Refs.hpp"

template<typename T, size_t C>
class StackArray
{
private:
	T m_elements[C];
public:
	template<ConvertibleTo... Args, std::enable_if<sizeof...(Args) == C>>
	StackArray(Args&&... args) : m_elements { args... } {}

	Size Count() const { return C; }

          T& operator[](Size index)       { return m_elements[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_elements[index.ToRawValue()]; }

	NullableRef<Size> IndexOf(const T& item) const requires Equatable<T>
	{
		for(Size i = 0; i < C; i++)
		{
			if(m_elements[i] == item)
				return New<Size>(i);
		}

		return nullptr;
	}

	Boolean Contains(const T& item) const requires Equatable<T> { return IndexOf(item); }
};