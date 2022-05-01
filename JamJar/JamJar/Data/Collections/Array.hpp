#pragma once

#include "../Memory/Refs.hpp"

template<typename T>
class Array : public IArray<T>
{
public:
	virtual NullableRef<Size> IndexOf(const T& item) const override requires Equatable<T>
	{
		for(Size i = 0; i < Count(); i++)
		{
			if((*this)[i] == item)
				return New<Size>(i);
		}

		return nullptr;
	}

	virtual Boolean Contains(const T& item) const override requires Equatable<T> { return IndexOf(item); }

	virtual SharedRef<IArray<T>> Slice(Size index, Size count) const override;

	virtual void CopyTo(const IArray<T>& array, Size sourceIndex, Size destIndex, Size count) const override
	{
		for(Size i = 0; i < count; i++)
			array[i + destIndex] = (*this)[i + sourceIndex];
	}

	virtual void Fill(const T& item) override requires CopyAssignable<T>
	{
		for(Size i = 0; i < Count(); i++)
			(*this)[i] = item;
	}
};

template<typename T, size_t C>
class StackArray : public Array<T>
{
private:
	T m_elements[C];
public:
	template<ConvertibleTo... Args, std::enable_if<sizeof...(Args) == C>>
	StackArray(Args&&... args) : m_elements { args... } {}

	virtual Size Count() const override { return C; }

    virtual       T& operator[](Size index)       override { return m_elements[index.ToRawValue()]; }
	virtual const T& operator[](Size index) const override { return m_elements[index.ToRawValue()]; }
};

template<typename T>
class HeapArray : public Array<T>
{
private:
	T*   m_address;
	Size m_count;
public:
	HeapArray(Size count) : m_address(new T[count]), m_count(count) {}

	~HeapArray() { delete[] m_address; }

	virtual Size Count() const override { return m_count; }

	virtual       T& operator[](Size index)       override { return m_address[index.ToRawValue()]; }
	virtual const T& operator[](Size index) const override { return m_address[index.ToRawValue()]; }
};

template<typename T>
SharedRef<IArray<T>> Array<T>::Slice(Size index, Size count) const
{
	SharedRef<HeapArray<T>> result = New<HeapArray<T>>(count);
	for(Size i = 0; i < count; i++)
		result[i] = (*this)[i + index];

	return result;
}
