#pragma once

#include "../List.hpp"

template<typename T>
class ArrayList : public IList<T>
{
private:
	BufferRef<T> m_buffer;
public:
	ArrayList(Size initialCapacity = 4U) : m_buffer(initialCapacity) {}

	virtual Size Count() const override { return m_buffer.Count(); }

	virtual void Add     (const T& item)               override;
	virtual void AddRange(const ICollection<T>& items) override;

	virtual void Insert     (Size index, const T& item)               override;
	virtual void InsertRange(Size index, const ICollection<T>& items) override;

	virtual void RemoveAt   (Size index)             override;
	virtual void RemoveRange(Size index, Size count) override;

	virtual Boolean Remove(const T& item) = 0;

	virtual SharedRef<IList<T>> SubList(Size index, Size count) const override;

	virtual ArrayRef<T> ToArray() const override;
};