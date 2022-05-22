#pragma once

#include "Buffer.hpp"
#include "Refs.hpp"

#include <iterator>

template<typename T, size_t C>
class StackArray
{
private:
	T m_elements[C];
public:
	StackArray() requires DefaultConstructible<T> {}

	StackArray(const T& item) requires CopyConstructible<T>
	{
		for(Size i = 0; i < C; i++)
			m_elements[i.ToRawValue()] = item;
	}

	template<ConvertibleTo<T>... Args>
	StackArray(Args&&... args) : m_elements { args... } {}

	Size Count() const { return C; }

	      T& operator[](Size index)       { return m_elements[index]; }
	const T& operator[](Size index) const { return m_elements[index]; }
};

enum class AllocType
{
	StackArray,
	HeapArray,
	Buffer,
};

template<typename T>
class ArrayRef
{
private:
	T*        m_array;
	Size      m_count;
	Size*     m_refCount;
	AllocType m_allocType;

	void AddRef() { ++(*m_refCount); }

	Boolean RemRef()
	{
		--(*m_refCount);
		return *m_refCount == 0U;
	}

	void Delete()
	{
		switch(m_allocType)
		{
			case AllocType::Buffer:
				for(Size i = 0; i < m_count; i++)
					m_array[i].~T();

				break;
			case AllocType::HeapArray:
				delete[] m_array;
		}
	}
public:
	ArrayRef(Size count) requires DefaultConstructible<T> : 
		m_array(new T[count.ToRawValue()]), m_count(count), m_refCount(new Size(1U)), m_allocType(AllocType::HeapArray) {}

	template<size_t C>
	ArrayRef(const StackArray<T, C>& stackArray) : m_array(stackArray.m_elements), m_count(C), m_refCount(nullptr), m_allocType(AllocType::StackArray) {}

	ArrayRef(const BufferRef<T>& bufferRef) requires DefaultConstructible<T> :
		m_array(bufferRef.m_buffer), m_count(bufferRef.m_count), m_refCount(bufferRef.m_refCount), m_allocType(AllocType::Buffer) 
	{
		if(!bufferRef.m_isInitialized)
		{
			for(Size i = 0; i < m_count; i++)
				new(m_array + i.ToRawValue()) T();

			(*bufferRef.m_isInitialized) = true;
		}
	}

	ArrayRef(const BufferRef<T>& bufferRef, const T& defaultValue) requires CopyConstructible<T> :
		m_array(bufferRef.m_buffer), m_count(bufferRef.m_count), m_refCount(bufferRef.m_refCount), m_allocType(AllocType::Buffer) 
	{
		if(!bufferRef.m_isInitialized)
		{
			for(Size i = 0; i < m_count; i++)
				new(m_array + i.ToRawValue()) T(defaultValue);

			(*bufferRef.m_isInitialized) = true;
		}
	}

	ArrayRef(const ArrayRef<T>& other) : m_array(other.m_array), m_count(other.m_count), m_refCount(other.m_refCount), m_allocType(other.m_allocType) 
	{
		if(m_refCount)
			AddRef();
	}

	ArrayRef<T> operator=(const ArrayRef<T>& other) 
	{
		if(m_refCount && RemRef())
			Delete();

		m_array     = other.m_array;
		m_count     = other.m_count;
		m_refCount  = other.m_refCount;
		m_allocType = other.m_allocType;

		if(m_refCount)
			AddRef();
	}

	~ArrayRef()
	{
		if(m_refCount && RemRef())
			Delete();
	}

	      T& operator[](Size index)       { return m_array[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_array[index.ToRawValue()]; }
};

template<typename T>
class ArraySpan 
{
private:
	ArrayRef<T> m_array;
	Size        m_index;
	Size        m_count;
public:
	ArraySpan(const ArrayRef<T>& array) : ArraySpan(array, 0, array->Count()) {}

	ArraySpan(const ArrayRef<T>& array, Size index, Size count) : m_array(array), m_index(index), m_count(count) {}

	Size Index() const { return m_index; }
	Size Count() const { return m_count; }

	      T& operator[](Size index)       { return m_array[index + m_index]; }
	const T& operator[](Size index) const { return m_array[index + m_index]; }

	ArraySpan<T> Slice(Size index, Size count) const { return ArraySpan<T>(m_array, index, count); }

	ArrayRef<T> ToArray() const requires CopyAssignable<T>
	{
		ArrayRef<T> result = ArrayRef<T>(m_count);
		for(Size i = 0U; i < m_count; i++)
			result[i] = m_array[i + m_index];
	}

	void Fill(const T& value) requires CopyAssignable<T> 
	{
		for(Size i = 0; i < m_count; i++)
			m_array[i + m_index] = value;
	}

	void CopyTo(ArraySpan<T> destination, Size sourceIndex, Size destIndex, Size count) const
	{
		for(Size i = 0U; i < count; i++)
			destination[i + destIndex] = m_array[m_index + i + sourceIndex];
	}

	friend Boolean operator==(const ArraySpan<T>& left, const ArraySpan<T>& right) requires Equatable<T>
	{
		if(left.Count() != right.Count())
			return false;

		for(Size i = 0; i < Count(); i++)
		{
			if(left[i] != right[i])
				return false;
		}

		return true;
	}

	friend Boolean operator!=(const ArraySpan<T>& left, const ArraySpan<T>& right) requires Equatable<T>
	{
		if(left.Count() != right.Count())
			return true;

		for(Size i = 0; i < Count(); i++)
		{
			if(left[i] != right[i])
				return true;
		}

		return false;
	}
};