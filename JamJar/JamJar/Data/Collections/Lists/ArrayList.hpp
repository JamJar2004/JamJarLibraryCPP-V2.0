#pragma once

#include "../List.hpp"
#include "../../Memory/Buffer.hpp"

template<typename T>
class ArrayList : public IList<T>
{
private:
	BufferRef<T> m_buffer;
	Size         m_count;

	void ReAllocate(Size count);
public:
	ArrayList(Size initialCapacity = 4U) : m_buffer(initialCapacity), m_count(0U) {}

	virtual Size Count() const override { return m_count; }

	virtual       T& operator[](Size index)       override { return *m_buffer[index]; }
	virtual const T& operator[](Size index) const override { return *m_buffer[index]; }

	virtual void Add     (const T& item)               override;
	virtual void AddRange(const ICollection<T>& items) override;

	virtual void Insert     (Size index, const T& item)               override;
	virtual void InsertRange(Size index, const ICollection<T>& items) override;

	virtual void RemoveAt   (Size index)             override;
	virtual void RemoveRange(Size index, Size count) override;

	virtual void Clear() override;

	SInt64 IndexOf(const T& item) const requires Equatable<T>
	{
		for(Size i = 0U; i < Count(); i++)
		{
			if((*this)[i] == item)
				return SInt64(i);
		}

		return -1;
	}

	virtual Boolean Remove(const T& item) override;

	virtual SharedRef<IList<T>> SubList(Size index, Size count) const override;

	ArrayRef<T> ToArray() const requires CopyConstructible<T>
	{
		T* array = (T*)malloc(sizeof(T) * m_count.ToRawValue());

		for(Size i = 0U; i < m_count; i++)
			new(array + i.ToRawValue()) T((*this)[i]);

		return ArrayRef<T>(array, m_count);
	}

	virtual SharedRef<Iterator<T>> Start() override { return New<ArrayIterator<T>>(m_buffer.m_address                       ); }
	virtual SharedRef<Iterator<T>> End()   override { return New<ArrayIterator<T>>(m_buffer.m_address + m_count.ToRawValue()); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ArrayIterator<const T>>(m_buffer.m_address                       ); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ArrayIterator<const T>>(m_buffer.m_address + m_count.ToRawValue()); }
};

template<typename T>
inline void ArrayList<T>::ReAllocate(Size count)
{
	BufferRef<T> newBuffer = BufferRef<T>(m_buffer.Count() * 2U + count);
	m_buffer.CopyTo(newBuffer, m_count);
	m_buffer = newBuffer;
}

template<typename T>
inline void ArrayList<T>::Add(const T& item)
{
	if(m_count + 1U >= m_buffer.Count())
		ReAllocate(1U);

	UnsafeRef<T> ptr = m_buffer[m_count++];
	ptr.Initialize(item);

	++m_count;
}

template<typename T>
inline void ArrayList<T>::AddRange(const ICollection<T>& items) { InsertRange(m_count, items); }

template<typename T>
inline void ArrayList<T>::Insert(Size index, const T& item) { InsertRange(index, StackArray<T, 1U>(item)); }

template<typename T>
inline void ArrayList<T>::InsertRange(Size index, const ICollection<T>& items)
{
	if(m_count + items.Count() >= m_buffer.Count())
		ReAllocate(items.Count());

	UnsafeRef<T> source = m_buffer[index];
	UnsafeRef<T> dest   = m_buffer[index + items.Count()];

	source.CopyTo(dest, m_count - index);

	for(const T& item : items)
	{
		source.Initialize(item);
		++source;
	}

	m_count += items.Count();
}

template<typename T>
inline void ArrayList<T>::RemoveAt(Size index) { RemoveRange(index, 1U); }

template<typename T>
inline void ArrayList<T>::RemoveRange(Size index, Size count)
{
	Size rangeEnd = index + count;

	UnsafeRef<T> source = m_buffer[rangeEnd];
	UnsafeRef<T> dest   = m_buffer[index];

	for(Size i = 0U; i < count; i++)
		(dest + i).Destroy();

	source.CopyTo(dest, m_count - rangeEnd);

	m_count -= count;
}

template<typename T>
inline void ArrayList<T>::Clear()
{
	for(Size i = 0U; i < m_count; i++)
		(((UnsafeRef<T>)m_buffer) + i).Destroy();

	m_count = 0U;
}

template<typename T>
inline Boolean ArrayList<T>::Remove(const T& item)
{
	SInt64 index = IndexOf(item);
	if(index == -1)
		return false;

	RemoveAt(Size(index));
	return true;
}

template<typename T>
inline SharedRef<IList<T>> ArrayList<T>::SubList(Size index, Size count) const
{
	SharedRef<ArrayList<T>> result = New<ArrayList<T>>(count);
	for(Size i = 0U; i < count; i++)
		result->Add((*this)[i + index]);

	return result;
}
