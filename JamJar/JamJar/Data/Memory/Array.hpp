#pragma once

#include <iterator>

template<typename T>
class HeapArray;

template<typename T>
class ArraySpan;

template<typename T>
class ArrayRef;

template<typename T>
class SharedArrayRef;

template<typename T>
class SharedArraySpan;

template<typename T, size_t C>
class StackArray
{
private:
	T m_elements[C];
public:
	using Iterator = T*;
	using ConstIterator = T const*;

	StackArray() requires DefaultConstructible<T> {}

	StackArray(const T& item) requires CopyConstructible<T> : m_elements { item } {}

	template<ConvertibleTo<T>... Args>
	StackArray(Args&&... args) requires Contains<Args..., C> : m_elements { args... } {}

	Size Count() const { return C; }

	      T& operator[](Size index)       { return m_elements[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_elements[index.ToRawValue()]; }

	      ArraySpan<T> AsSpan()       { return ArraySpan<T>(*this); }
	const ArraySpan<T> AsSpan() const { return ArraySpan<T>(*this); }

		  ArraySpan<T> AsSpan(Size index, Size count)       { return ArraySpan<T>(*this, index, count); }
	const ArraySpan<T> AsSpan(Size index, Size count) const { return ArraySpan<T>(*this, index, count); }

	friend Boolean operator==(const StackArray<T, C>& left, const StackArray<T, C>& right) requires Equatable<T>
	{
		for(Size i = 0U; i < left.Count(); i++)
		{
			if(left[i] != right[i])
				return false;
		}

		return true;
	}

	friend Boolean operator!=(const StackArray<T, C>& left, const StackArray<T, C>& right) requires Equatable<T>
	{
		for(Size i = 0U; i < left.Count(); i++)
		{
			if(left[i] != right[i])
				return true;
		}

		return false;
	}

	HashCode GetHashCode() const requires Hashable<T> 
	{
		HashCode result;
		for(Size i = 0U; i < C; i++)
			result &= m_elements[i.ToRawValue()].GetHashCode();

		return result;
	}

	Iterator begin() { return m_elements;     }
	Iterator end()   { return m_elements + C; }

	ConstIterator begin() const { return m_elements;     }
	ConstIterator end()   const { return m_elements + C; }

	friend class ArrayRef<T>;
};

template<typename T>
class HeapArray
{
private:
	T*    m_address;
	Size* m_refCount;
	Size  m_count;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		Size& refCount = *m_refCount;
		--refCount;

		if(refCount == 0U)
		{
			for(Size i = 0U; i < m_count; i++)
				(m_address + i.ToRawValue())->~T();

			free(m_address);
			delete m_refCount;
		}
	}
public:
	static const HeapArray<T> Empty;

	template<SameAs<T>... Args>
	HeapArray(Args&&... args) : m_address(new T[] { (T)args... }), m_refCount(new Size(1U)), m_count(sizeof...(args)) {}

	HeapArray(Size count) requires DefaultConstructible<T> : m_address((T*)malloc(sizeof(T)* count.ToRawValue())), m_refCount(new Size(1U)), m_count(count)
	{
		for(Size i = 0U; i < m_count; i++)
			new(m_address + i.ToRawValue()) T();
	}

	HeapArray(Size count, const T& item) requires CopyConstructible<T> : 
		m_address((T*)malloc(sizeof(T)* count.ToRawValue())), m_refCount(new Size(1U)), m_count(count)
	{
		for(Size i = 0U; i < m_count; i++)
			new(m_address + i.ToRawValue()) T(item);
	}

	HeapArray(const HeapArray<T>& other) requires CopyConstructible<T> : 
		m_address((T*)malloc(sizeof(T)* other.m_count.ToRawValue())), m_refCount(new Size(1U)), m_count(other.m_count)
	{
		for(Size i = 0U; i < m_count; i++)
			new(m_address + i.ToRawValue()) T(other[i]);
	}

	template<size_t C>
	HeapArray(const StackArray<T, C>& other) :
		m_address((T*)malloc(sizeof(T) * C)), m_refCount(new Size(1U)), m_count(C)
	{
		for(Size i = 0U; i < m_count; i++)
			new(m_address + i.ToRawValue()) T(other[i]);
	}

	HeapArray(const ArrayRef<T>& other) :
		m_address((T*)malloc(sizeof(T) * other.m_count.ToRawValue())), m_refCount(new Size(1U)), m_count(other.m_count)
	{
		for(Size i = 0U; i < m_count; i++)
			new(m_address + i.ToRawValue()) T(other[i]);
	}

	~HeapArray() { RemRef(); }

	Size Count() const { return m_count; }

    	  T& operator[](Size index)       { return m_address[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_address[index.ToRawValue()]; }

	      SharedArraySpan<T> AsSpan()       { return SharedArraySpan<T>(*this); }
	const SharedArraySpan<T> AsSpan() const { return SharedArraySpan<T>(*this); }

		  SharedArraySpan<T> AsSpan(Size index, Size count)       { return SharedArraySpan<T>(*this, index, count); }
	const SharedArraySpan<T> AsSpan(Size index, Size count) const { return SharedArraySpan<T>(*this, index, count); }

	friend class ArrayRef<T>;
	friend class SharedArrayRef<T>;
	friend class DynamicArray;
};

template<typename T>
const HeapArray<T> HeapArray<T>::Empty(0U);

template<typename T>
class ArrayRef
{
private:
	T*   m_address;
	Size m_count;
public:
	template<size_t C>
	ArrayRef(const StackArray<T, C>& other) : m_address((T*)other.m_elements), m_count(C) {}

	ArrayRef(const SharedArrayRef<T>& other) : m_address(other.m_address), m_count(other.m_count) {}

	Size Count() const { return m_count; }

		  T& operator[](Size index)       { return m_address[index.ToRawValue()]; }
	const T& operator[](Size index) const { return m_address[index.ToRawValue()]; }

	      ArraySpan<T> AsSpan()       { return ArraySpan<T>(*this); }
	const ArraySpan<T> AsSpan() const { return ArraySpan<T>(*this); }

		  ArraySpan<T> AsSpan(Size index, Size count)       { return ArraySpan<T>(*this, index, count); }
	const ArraySpan<T> AsSpan(Size index, Size count) const { return ArraySpan<T>(*this, index, count); }

	friend class DynamicArray;
};

template<typename T>
class ArraySpan
{
private:
	ArrayRef<T> m_array;
	Size        m_index;
	Size        m_count;
public:
	ArraySpan(const ArrayRef<T>& array) : m_array(array), m_index(0U), m_count(array.Count()) {}

	ArraySpan(const ArrayRef<T>& array, Size index, Size count) : m_array(array), m_index(index), m_count(count) {}

	ArraySpan(const SharedArraySpan<T>& other) : m_array(other.m_array), m_index(other.m_index), m_count(other.m_count) {}

	Size Count() const { return m_count; }

		  T& operator[](Size index)       { return m_array[m_index + index]; }
	const T& operator[](Size index) const { return m_array[m_index + index]; }

	void Fill(const T& value) requires CopyAssignable<T>
	{
		for(Size i = 0U; i < m_count; i++)
			m_array[i + m_index] = value;
	}

	ArraySpan<T> Slice(Size index, Size count) const { return m_array.AsSpan(m_index + index, count); }

	void CopyTo(ArraySpan<T> dest) const requires CopyAssignable<T>
	{
		for(Size i = 0U; i < m_count; i++)
			dest[i] = m_array[i + m_index];
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

template<typename T>
class SharedArrayRef
{
private:
	T*    m_address;
	Size* m_refCount;
	Size  m_count;

	void AddRef() { ++(*m_refCount); }

	void RemRef()
	{
		Size& refCount = *m_refCount;
		--refCount;

		if(refCount == 0U)
		{
			delete[] m_address;
			delete m_refCount;
		}
	}

	SharedArrayRef(T* address, Size count) : m_address(address), m_refCount(new Size(1U)), m_count(count) {}
public:
	SharedArrayRef(const HeapArray<T>& array) : m_address(array.m_address), m_refCount(array.m_refCount), m_count(array.m_count) { AddRef(); }

	SharedArrayRef(const SharedArrayRef<T>& other) : m_address(other.m_address), m_refCount(other.m_refCount), m_count(other.m_count) { AddRef(); }

	~SharedArrayRef() { RemRef(); }

	SharedArrayRef<T>& operator=(const SharedArrayRef<T>& other)
	{
		RemRef();
		m_address  = other.m_address;
		m_refCount = other.m_refCount;
		m_count    = other.m_count;
		AddRef();

		return *this;
	}

	Size Count() const { return m_count; }

	virtual       T& operator[](Size index)       { return m_address[index.ToRawValue()]; }
	virtual const T& operator[](Size index) const { return m_address[index.ToRawValue()]; }

	      SharedArraySpan<T> AsSpan()       { return *this; }
	const SharedArraySpan<T> AsSpan() const { return *this; }

		  SharedArraySpan<T> AsSpan(Size index, Size count)       { return SharedArraySpan<T>(*this, index, count); }
	const SharedArraySpan<T> AsSpan(Size index, Size count) const { return SharedArraySpan<T>(*this, index, count); }

	friend class SharedArraySpan<T>;
	friend class ArrayRef<T>;
};

template<typename T>
class SharedArraySpan
{
private:
	SharedArrayRef<T> m_array;
	Size              m_index;
	Size              m_count;
public:
	SharedArraySpan(const SharedArrayRef<T>& array) : SharedArraySpan(array, 0U, array.Count()) {}

	SharedArraySpan(const SharedArrayRef<T>& array, Size index, Size count) : m_array(array), m_index(index), m_count(count) {}

	Size Index() const { return m_index; }
	Size Count() const { return m_count; }

	      T& operator[](Size index)       { return m_array[index + m_index]; }
	const T& operator[](Size index) const { return m_array[index + m_index]; }

	SharedArraySpan<T> Slice(Size index, Size count) const { return SharedArraySpan<T>(m_array, index + m_index, count); }

	SharedArrayRef<T> ToArray() const requires CopyConstructible<T>
	{
		T* array = (T*)malloc(m_array.Count().ToRawValue() * sizeof(T));

		for(Size i = 0U; i < m_count; i++)
			new(array + i.ToRawValue()) T(m_array[i + m_index]);

		return SharedArrayRef<T>(array, m_count);
	}

	void Fill(const T& value) requires CopyAssignable<T> 
	{
		for(Size i = 0U; i < m_count; i++)
			m_array[i + m_index] = value;
	}

	void CopyTo(ArraySpan<T> destination) const requires CopyAssignable<T>
	{
		for(Size i = 0U; i < destination.Count(); i++)
			destination[i] = m_array[m_index + i];
	}

	friend Boolean operator==(const SharedArraySpan<T>& left, const SharedArraySpan<T>& right) requires Equatable<T>
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

	friend Boolean operator!=(const SharedArraySpan<T>& left, const SharedArraySpan<T>& right) requires Equatable<T>
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

	friend class ArraySpan<T>;
};