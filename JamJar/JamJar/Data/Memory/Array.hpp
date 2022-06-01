#pragma once

#include "Buffer.hpp"
#include "../Collections/Collection.hpp"

#include <iterator>

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:
	T* m_address;
public:
	ArrayIterator(T* address) : m_address(address) {}

	virtual void MoveNext() override { ++m_address; }

	virtual T& Current() { return *m_address; }

	virtual Boolean Equals(const Iterator<T>& other) { return m_address == ((ArrayIterator<T>)other).m_address; }
};

template<typename T>
class IArray : public ICollection<T>
{
public:
	virtual       T& operator[](Size index)       = 0;
	virtual const T& operator[](Size index) const = 0;
};

template<typename T>
class Array : public IArray<T>
{
public:
	SInt64 IndexOf(const T& item) const requires Equatable<T>
	{
		for(Size i = 0; i < Count(); i++)
		{
			if((*this)[i] == item)
				return SInt64(i);
		}

		return -1;
	}

	Boolean Contains(const T& item) const requires Equatable<T> { return IndexOf(item) != -1; }

	void Fill(const T& item) requires CopyAssignable<T>
	{
		for(Size i = 0U; i < m_count; i++)
			(*this)[i] = item;
	}
};

template<typename T, size_t C>
class StackArray : public Array<T>
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
	StackArray(Args&&... args) requires Contains<Args, C> : m_elements { args... } {}

	virtual Size Count() const override { return C; }

	virtual       T& operator[](Size index)       override { return m_elements[index]; }
	virtual const T& operator[](Size index) const override { return m_elements[index]; }

	virtual SharedRef<Iterator<T>> Start() { return ArrayIterator<T>(m_elements    ); }
	virtual SharedRef<Iterator<T>> End()   { return ArrayIterator<T>(m_elements + C); }
};

template<typename T>
class ArrayRef : public Array<T>
{
private:
	BufferRef<T> m_buffer;
	Size*        m_refCount;

	void AddRef() { ++(*m_refCount); }

	void RemRef() 
	{
		--(*m_refCount);
		if(*m_refCount == 0U)
		{
			for(Size i = 0U; i < m_buffer.Count(); i++)
				(m_buffer.m_address + i.ToRawValue())->~T();
		}
	}
public:
	ArrayRef(Size count) requires DefaultConstructible<T> : ArrayRef(BufferRef(count)) {}

	ArrayRef(Size count, const T& item) requires CopyConstructible<T> : ArrayRef(BufferRef(count), item) {}

	ArrayRef(const BufferRef<T>& buffer) requires DefaultConstructible<T> : m_buffer(buffer)
	{
		for(Size i = 0U; i < buffer.Count(); i++)
			new(buffer.m_address + i.ToRawValue()) T();
	}

	ArrayRef(const BufferRef<T>& buffer, const T& item) requires CopyConstructible<T> : m_buffer(buffer)
	{
		for(Size i = 0U; i < buffer.Count(); i++)
			new(buffer.m_address + i.ToRawValue()) T(item);
	}

	ArrayRef(const ArrayRef<T>& other) : m_buffer(other.m_buffer), m_refCount(other.m_refCount) { AddRef(); }

	~ArrayRef() { RemRef(); }

	ArrayRef<T> operator=(const ArrayRef<T>& other) 
	{
		RemRef();
		m_buffer   = other.m_buffer;
		m_refCount = other.m_refCount;
		AddRef();
	}

	virtual Size Count() const override { return m_buffer.Count(); }

	virtual       T& operator[](Size index)       override { return m_buffer.m_address[index.ToRawValue()]; }
	virtual const T& operator[](Size index) const override { return m_buffer.m_address[index.ToRawValue()]; }
};

template<typename T>
class ArraySpan 
{
private:
	ArrayRef<T> m_array;
	Size        m_index;
	Size        m_count;
public:
	ArraySpan(const ArrayRef<T>& array) : ArraySpan(array, 0U, array.Count()) {}

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

		return result;
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

		for(Size i = 0U; i < left.Count(); i++)
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

		for(Size i = 0U; i < left.Count(); i++)
		{
			if(left[i] != right[i])
				return true;
		}

		return false;
	}
};