#pragma once

#include "../Collections/ICollection.hpp"


template<typename T>
class Span : public IArray<T>
{
private:
	SharedRef<IArray<T>> m_array;
	Size                 m_index;
	Size                 m_count;
public:
	Span(const SharedRef<IArray<T>>& array) : Span(array, 0, array->Count()) {}

	Span(const SharedRef<IArray<T>>& array, Size index, Size count) : m_array(array), m_index(index), m_count(count) {}

	virtual       T& operator[](Size index)       override { return m_array[m_index + index]; }
	virtual const T& operator[](Size index) const override { return m_array[m_index + index]; }

	virtual Size Count() const override { return m_count; }

	virtual NullableRef<Size> IndexOf(const T& item) const override requires Equatable<T>
	{
		for(Size i = m_index; i < m_index + m_count; i++)
		{
			if(m_array[i] == item)
				return i;
		}

		return nullptr;
	}

	virtual Boolean Contains(const T& item) const override requires Equatable<T> { return IndexOf(item); }
	
	virtual SharedRef<IArray<T>> Slice(Size index, Size count) const override
	{
		SharedRef<Span<T>> result = New<Span<T>>(m_array, m_index + index, count);
		return result;
	}

	virtual void Fill(const T& item) override requires CopyAssignable<T>
	{
		for(Size i = m_index; i < m_index + m_count; i++)
			m_array[i] = item;
	}

	virtual void CopyTo(const IArray<T>& array, Size sourceIndex, Size destIndex, Size count) const override
	{
		for(Size i = 0; i < count; i++)
			array[i + destIndex] = (*this)[i + sourceIndex];
	}

	virtual SharedRef<IArray<T>> ToArray() const
	{
		SharedRef<HeapArray<T>> result = New<HeapArray<T>>(m_count);
		for(Size i = 0; i < m_count; i++)
			result[i] = m_array[i + m_index];

		return result;
	}
};