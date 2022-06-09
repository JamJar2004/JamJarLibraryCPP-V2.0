#pragma once

#include "../Map.hpp"
#include "../../Memory/Array.hpp"

template<typename T>
concept HashableEquatable = Hashable<T> && Equatable<T>;

template<HashableEquatable K, typename V>
class HashMap : public IMap<K, V>
{
private:
	class HashEntry : public Entry<K, V>
	{
	private:
		HashCode m_hashCode;

		NullableRef<HashEntry> m_next;
	public:
		HashEntry(const K& key, const V& value, HashCode hashCode) : Entry<K, V>(key, value), m_hashCode(hashCode), m_next(nullptr) {}

		HashCode GetHashCode() const { return m_hashCode; }

		NullableRef<HashEntry> GetNext() const { return m_next; }

		void SetNext(NullableRef<HashEntry> next) { m_next = next; }
	};

	ArrayRef<NullableRef<HashEntry>> m_buckets;

	Size m_count;

	NullableRef<HashEntry>& GetBucket(HashCode hashCode)
	{
		Size index = hashCode.GetValue();
		return m_buckets[index % m_buckets.Count()];
	}
public:
	HashMap(Size initialCapacity = 16U) : m_buckets(initialCapacity) {}

	virtual Size Count() const override { return m_count; }

	virtual       V& operator[](const K& key)       override;
	virtual const V& operator[](const K& key) const override;

	virtual Boolean    Add(const K& key, const V& value) override;
	virtual Boolean Remove(const K& key                ) override;

	virtual Size    AddRange(const IMap<K, V>& map     ) override;
	virtual Size RemoveRange(const ICollection<K>& keys) override;

	virtual Boolean TryGet(const K& key, V& outValue) override;

	virtual const ICollection<const K> GetKeys()   const override;
	virtual const ICollection<      V> GetValues() const override;

	virtual void Clear() override;
};

template<HashableEquatable K, typename V>
inline Boolean HashMap<K, V>::Add(const K& key, const V& value)
{
	HashCode hashCode = key.GetHashCode();

	NullableRef<HashEntry>& first = GetBucket(hashCode);

	NullableRef<HashEntry> last = nullptr;
	for(NullableRef<HashEntry> entry = first; entry != nullptr; entry = entry->GetNext())
	{
		if(entry->GetHashCode() == hashCode && entry->GetKey() == key)
		{
			entry->SetValue(value);
			return false;
		}

		last = entry;
	}

	HashEntry newEntry = New<HashEntry>(key, value, hashCode);

	if(last == nullptr)
		first = newEntry;
	else
		last->SetNext(newEntry);

	++m_count;
	return true;
}

template<HashableEquatable K, typename V>
inline Boolean HashMap<K, V>::Remove(const K& key)
{
	HashCode hashCode = key.GetHashCode();

	NullableRef<HashEntry>& first = GetBucket(hashCode);

	NullableRef<HashEntry> last = nullptr;
	for(NullableRef<HashEntry> entry = first; entry != nullptr; entry = entry->GetNext())
	{
		if(entry->GetHashCode() == hashCode && entry->GetKey() == key)
		{
			if(last == nullptr)
				first = nullptr;
			else
				last->SetNext(entry->GetNext());

			--m_count;
			return true;
		}

		last = entry;
	}

	return false;
}
