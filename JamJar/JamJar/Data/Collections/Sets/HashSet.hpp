#pragma once

#include "../Set.hpp"
#include "../../Memory/Array.hpp"

template<Hashable T>
class HashSet : public Set<T>
{
private:
	class Node
	{
	private:
		T m_value;

		NullableRef<Node> m_next;
	public:
		Node(const T& value) : m_value(value), m_next(nullptr) {}

		const T& GetValue() const { return m_value; }

		NullableRef<Node> GetNext() const { return m_next; }

		void SetNext(NullableRef<Node> next) { m_next = next; }
	};

	ArrayRef<NullableRef<Node>> m_buckets;
	
	Size m_count;

	NullableRef<Node>& GetBucket(HashCode hashCode)
	{
		Size index = hashCode.GetValue();
		return m_buckets[index];
	}
public:
	HashSet(Size initialCapacity = 16U) : m_buckets(initialCapacity), m_count(0U) {}

	virtual Size Count() const { return m_count; }

	virtual Boolean    Add(const T& item) override;
	virtual Boolean Remove(const T& item) override;

	virtual Size    AddRange(const ICollection<T>& items) override;
	virtual Size RemoveRange(const ICollection<T>& items) override;
};

template<Hashable T>
inline Boolean HashSet<T>::Add(const T& item)
{
	NullableRef<Node>& first = GetBucket(item.GetHashCode());
	if(first == nullptr)
	{
		first = New<Node>(item);
		return true;
	}

	for(NullableRef<Node> node = first; node != nullptr; node = node->GetNext())
	{
		if(node->GetValue())
	}
}

template<Hashable T>
inline Boolean HashSet<T>::Remove(const T& item)
{
	
}
