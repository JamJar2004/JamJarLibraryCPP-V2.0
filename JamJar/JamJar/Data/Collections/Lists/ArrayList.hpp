#pragma once

#include "../List.hpp"
#include "../../Memory/Buffer.hpp"

template<typename T>
class ArrayList : public IList<T>
{
private:
	BufferRef<T> m_buffer;
	UnsafeRef<T> m_end;

	void ReAllocate(Size count);
public:
	ArrayList(Size initialCapacity = 4U) : m_buffer(initialCapacity), m_end(m_buffer.First()) {}

	~ArrayList() { Clear(); }

	virtual Size Count() const override { return m_end - m_buffer.First(); }

	virtual       T& operator[](Size index)       override { return *m_buffer[index]; }
	virtual const T& operator[](Size index) const override { return *m_buffer[index]; }

	template<typename... Args>
	void Emplace(Args&&... args) requires ConstructibleFrom<T, Args...>
	{
		if(Count() + 1U >= m_buffer.Count())
			ReAllocate(1U);

		m_end.Initialize(args...);
		++m_end;
	}

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

	Boolean Remove(const T& item) requires Equatable<T>;

	virtual SharedRef<IList<T>> SubList(Size index, Size count) const override;

	ArrayRef<T> ToArray() const requires CopyConstructible<T>
	{
		T* array = (T*)malloc(sizeof(T) * Count().ToRawValue());

		for(Size i = 0U; i < Count(); i++)
			new(array + i.ToRawValue()) T((*this)[i]);

		return ArrayRef<T>(array, Count());
	}

	virtual SharedRef<Iterator<T>> Start() override { return New<ArrayIterator<T>>(m_buffer.m_address                       ); }
	virtual SharedRef<Iterator<T>> End()   override { return New<ArrayIterator<T>>(m_buffer.m_address + Count().ToRawValue()); }

	virtual SharedRef<Iterator<const T>> Start() const override { return New<ArrayIterator<const T>>(m_buffer.m_address                       ); }
	virtual SharedRef<Iterator<const T>> End()   const override { return New<ArrayIterator<const T>>(m_buffer.m_address + Count().ToRawValue()); }
};

template<typename T>
inline void ArrayList<T>::ReAllocate(Size count)
{
	BufferRef<T> newBuffer = BufferRef<T>(m_buffer.Count() * 2U + count);
	m_buffer.CopyTo(newBuffer, Count());
	m_end    = newBuffer[Count()];
	m_buffer = newBuffer;
}

template<typename T>
inline void ArrayList<T>::Add(const T& item)
{
	if(Count() + 1U >= m_buffer.Count())
		ReAllocate(1U);

	m_end.Initialize(item);
	++m_end;
}

template<typename T>
inline void ArrayList<T>::AddRange(const ICollection<T>& items) { InsertRange(Count(), items); }

template<typename T>
inline void ArrayList<T>::Insert(Size index, const T& item) { InsertRange(index, StackArray<T, 1U>(item)); }

template<typename T>
inline void ArrayList<T>::InsertRange(Size index, const ICollection<T>& items)
{
	if(Count() + items.Count() >= m_buffer.Count())
		ReAllocate(items.Count());

	UnsafeRef<T> source = m_buffer[index];
	UnsafeRef<T> dest   = m_buffer[index + items.Count()];

	source.CopyTo(dest, Count() - index);

	for(const T& item : items)
	{
		source.Initialize(item);
		++source;
	}

	m_end += items.Count();
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

	source.CopyTo(dest, Count() - rangeEnd);

	m_end -= count;
}

template<typename T>
inline void ArrayList<T>::Clear()
{
	while(m_end != m_buffer.First())
	{
		--m_end;
		m_end.Destroy();
	}
}

template<typename T>
inline Boolean ArrayList<T>::Remove(const T& item) requires Equatable<T>
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
