#pragma once

#include "../../Memory/Span.hpp"

template<typename T>
class ArrayList 
{
private:
	BufferSpan<T> m_buffer;
public:
	ArrayList(Size initialCapacity = 4U) : m_buffer(BufferRef<T>(initialCapacity), 0U, 0U) {}

	Size Count() const { return m_buffer.Count(); }

	void Add   (const T& item);
	void Insert(const T& item, Size index);

	void RemoveAt   (Size index);
	void RemoveRange(Size index, Size count);

	void Clear();

	Boolean Remove(const T& item) requires Equatable<T>;

	ArrayRef<T> ToArray() const;
};