#pragma once

#include "../Collections/Collection.hpp"

#include <iterator>

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:
	T*      m_address;
	Boolean m_isReverse;
public:
	ArrayIterator(T* address, Boolean isReverse = false) : m_address(address), m_isReverse(isReverse) {}

	virtual void MoveNext() override 
	{
		if(!m_isReverse)
			++m_address;
		else
			--m_address;
	}

	virtual T& Current() const override { return *m_address; }

	virtual Boolean Equals(const Iterator<T>& other) const override { return m_address == ((const ArrayIterator<T>&)other).m_address; }
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
		for(Size i = 0U; i < this->Count(); i++)
		{
			if((*this)[i] == item)
				return SInt64(i);
		}

		return -1;
	}

	Boolean Contains(const T& item) const requires Equatable<T> { return IndexOf(item) != -1; }

	void Fill(const T& item) requires CopyAssignable<T>
	{
		for(Size i = 0U; i < this->Count(); i++)
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

	StackArray(const T& item) requires CopyConstructible<T> : m_elements { item } {}

	template<ConvertibleTo<T>... Args>
	StackArray(Args&&... args) requires Contains<Args..., C> : m_elements { args... } {}

	virtual Size Count() const override { return C; }

	virtual       T& operator[](Size index)       override { return m_elements[index.ToRawValue()]; }
	virtual const T& operator[](Size index) const override { return m_elements[index.ToRawValue()]; }

	virtual SharedRef<Iterator<T>> Start() override { return New<ArrayIterator<T>>(m_elements    ); }
	virtual SharedRef<Iterator<T>> End()   override { return New<ArrayIterator<T>>(m_elements + C); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ArrayIterator<const T>>(m_elements    ); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ArrayIterator<const T>>(m_elements + C); }
};

template<typename T>
class ArrayList;

template<typename T>
class ArraySpan;

template<typename T>
class ArrayRef : public Array<T>
{
private:
	T*    m_address;
	Size* m_refCount;
	Size  m_count;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		--(*m_refCount);
		if(*m_refCount == 0U)
		{
			for(Size i = 0U; i < m_count; i++)
				(m_address + i.ToRawValue())->~T();

			free(m_address);
			delete m_refCount;
		}
	}

	ArrayRef(T* address, Size count) : m_address(address), m_refCount(new Size(1U)), m_count(count) {}
public:
	ArrayRef(Size count) requires DefaultConstructible<T> : 
		m_address((T*)malloc(sizeof(T) * count.ToRawValue())), m_refCount(new Size(1U)), m_count(count)
	{
		for(Size i = 0U; i < count; i++)
			new(m_address + i.ToRawValue()) T();
	}

	ArrayRef(Size count, const T& item) requires CopyConstructible<T> : 
		m_address((T*)malloc(sizeof(T)* count.ToRawValue())), m_refCount(new Size(1U)), m_count(count)
	{
		for(Size i = 0U; i < count; i++)
			new(m_address + i.ToRawValue()) T(item);
	}

	ArrayRef(const ArrayRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount), m_count(other.m_count) { AddRef(); }

	~ArrayRef() { RemRef(); }

	ArrayRef<T>& operator=(const ArrayRef<T>& other)
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		m_count    = other.m_count;
		AddRef();

		return *this;
	}

	virtual Size Count() const override { return m_count; }

	virtual       T& operator[](Size index)       override { return m_address[index.ToRawValue()]; }
	virtual const T& operator[](Size index) const override { return m_address[index.ToRawValue()]; }

	      ArraySpan<T> AsSpan()       { return *this; }
	const ArraySpan<T> AsSpan() const { return *this; }

	virtual SharedRef<Iterator<T>> Start() override { return New<ArrayIterator<T>>(m_address                       ); }
	virtual SharedRef<Iterator<T>> End()   override { return New<ArrayIterator<T>>(m_address + m_count.ToRawValue()); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ArrayIterator<const T>>(m_address); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ArrayIterator<const T>>(m_address + m_count.ToRawValue()); }

	friend class ArrayList<T>;
	friend class ArraySpan<T>;
};

template<typename T>
class ArraySpan : public Array<T>
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

	ArraySpan<T> Slice(Size index, Size count) const { return ArraySpan<T>(m_array, index + m_index, count); }

	ArrayRef<T> ToArray() const requires CopyConstructible<T>
	{
		T* array = (T*)malloc(m_array.Count().ToRawValue() * sizeof(T));

		for(Size i = 0U; i < m_count; i++)
			new(array + i.ToRawValue()) T(m_array[i + m_index]);

		return ArrayRef<T>(array, m_count);
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

	virtual SharedRef<Iterator<T>> Start() override { return New<ArrayIterator<T>>(&m_array[m_index          ]); }
	virtual SharedRef<Iterator<T>> End()   override { return New<ArrayIterator<T>>(&m_array[m_index + m_count]); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ArrayIterator<const T>>(&m_array[m_index]); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ArrayIterator<const T>>(&m_array[m_index + m_count]); }
};