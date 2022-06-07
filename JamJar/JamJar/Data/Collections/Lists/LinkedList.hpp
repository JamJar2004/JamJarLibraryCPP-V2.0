#pragma once

#include "../List.hpp"


template<typename T>
class LinkedList : public IList<T>
{
private:
	Size m_count;
public:
	LinkedList() : m_count(0U) {}

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