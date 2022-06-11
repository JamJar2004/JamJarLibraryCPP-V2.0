#pragma once

#include "../Map.hpp"
#include "../../Memory/Array.hpp"

#include "../../../Exception.hpp"
#include "../../Reflection.hpp"

class KeyNotFoundException : public Exception
{
public:
	template<Printable K>
	KeyNotFoundException(const K& key) : Exception("Key " + key + " not found.") {}

	template<typename K>
	KeyNotFoundException(const K& key) : Exception("Key of type " + Reflect::GetType<K>() + ", not found.") {}
};

template<typename T>
concept HashableEquatable = Hashable<T> && Equatable<T>;


template<HashableEquatable K, CopyAssignable V>
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

	template<typename T>
	class HashIterator : public Iterator<T>
	{
	private:
		const HashMap<K, V>& m_map;

		Size                   m_bucketIndex;
		NullableRef<HashEntry> m_current;
	public:
		HashIterator(const HashMap<K, V>& map, Boolean isEnd);

		virtual void MoveNext() override;

		Entry<K, V>& CurrentEntry() const;

		virtual Boolean Equals(const Iterator<T>& other) const override;
	};

	class KeyIterator : public HashIterator<const K>
	{
	public:
		KeyIterator(const HashMap<K, V>& map, Boolean isEnd) : HashIterator<const K>(map, isEnd) {}

		virtual const K& Current() const override { return this->CurrentEntry().GetKey(); }
	};

	class ValueIterator : public HashIterator<V>
	{
	public:
		ValueIterator(const HashMap<K, V>& map, Boolean isEnd) : HashIterator<V>(map, isEnd) {}

		virtual V& Current() const override { return this->CurrentEntry().GetValue(); }
	};

	class ConstValueIterator : public HashIterator<const V>
	{
	public:
		ConstValueIterator(const HashMap<K, V>& map, Boolean isEnd) : HashIterator<const V>(map, isEnd) {}

		virtual const V& Current() const override { return this->CurrentEntry().GetValue(); }
	};

	class EntryIterator : public HashIterator<Entry<K, V>>
	{
	public:
		EntryIterator(const HashMap<K, V>& map, Boolean isEnd) : HashIterator<Entry<K, V>>(map, isEnd) {}

		virtual Entry<K, V>& Current() const override { return this->CurrentEntry(); }
	};

	class ConstEntryIterator : public HashIterator<const Entry<K, V>>
	{
	public:
		ConstEntryIterator(const HashMap<K, V>& map, Boolean isEnd) : HashIterator<const Entry<K, V>>(map, isEnd) {}

		virtual const Entry<K, V>& Current() const override { return this->CurrentEntry(); }
	};

	template<typename T>
	class HashCollection : public ICollection<T>
	{
	protected:
		HashMap<K, V>& m_map;
	public:
		HashCollection(HashMap<K, V>& map) : m_map(map) {}

		virtual Size Count() const override { return m_map.Count(); }
	};

	class KeyCollection : public HashCollection<const K>
	{
	public:
		KeyCollection(HashMap<K, V>& map) : HashCollection<const K>(map) {}

		virtual SharedRef<Iterator<const K>> Start() override { return New<KeyIterator>(this->m_map, false); }
		virtual SharedRef<Iterator<const K>> End()   override { return New<KeyIterator>(this->m_map, true ); }

		virtual SharedRef<Iterator<const K>> Start() const override { return New<KeyIterator>(this->m_map, false); }
		virtual SharedRef<Iterator<const K>> End()   const override { return New<KeyIterator>(this->m_map, true ); }
	};

	class ValueCollection : public HashCollection<V>
	{
	public:
		ValueCollection(HashMap<K, V>& map) : HashCollection<V>(map) {}

		virtual SharedRef<Iterator<V>> Start() override { return New<ValueIterator>(this->m_map, false); }
		virtual SharedRef<Iterator<V>> End()   override { return New<ValueIterator>(this->m_map, true ); }

		virtual SharedRef<Iterator<const V>> Start() const override { return New<ConstValueIterator>(this->m_map, false); }
		virtual SharedRef<Iterator<const V>> End()   const override { return New<ConstValueIterator>(this->m_map, true ); }
	};

	const Float32 m_loadFactor;

	ArrayRef<NullableRef<HashEntry>> m_buckets;

	Size m_count;

	KeyCollection   m_keys;
	ValueCollection m_values;

	NullableRef<HashEntry>& GetBucket(HashCode hashCode)
	{
		Size index = hashCode.GetValue();
		return m_buckets[index % m_buckets.Count()];
	}

	const NullableRef<HashEntry>& GetBucket(HashCode hashCode) const
	{
		Size index = hashCode.GetValue();
		return m_buckets[index % m_buckets.Count()];
	}

	//void ReAllocate();

	NullableRef<HashEntry> Get(const K& key) const;
public:
	HashMap(Size initialCapacity = 16U, Float32 loadFactor = 0.75f) : m_buckets(initialCapacity), m_loadFactor(loadFactor), m_keys(*this), m_values(*this) {}

	virtual Size Count() const override { return m_count; }

	virtual       V& operator[](const K& key)       override;
	virtual const V& operator[](const K& key) const override;

	virtual Boolean    Add(const K& key, const V& value) override;
	virtual Boolean Remove(const K& key                ) override;

	virtual Size    AddRange(const IMap<K, V>& map     ) override;
	virtual Size RemoveRange(const ICollection<K>& keys) override;

	Boolean TryGet(const K& key, V& outValue) const requires CopyAssignable<V>;

	virtual const ICollection<const K>& GetKeys()   const override { return m_keys;   }
	virtual const ICollection<      V>& GetValues() const override { return m_values; }

	virtual void Clear() override;

	virtual SharedRef<Iterator<Entry<K, V>>> Start() override { return New<EntryIterator>(*this, false); }
	virtual SharedRef<Iterator<Entry<K, V>>> End()   override { return New<EntryIterator>(*this, true ); }

	virtual SharedRef<Iterator<const Entry<K, V>>> Start() const override { return New<ConstEntryIterator>(*this, false); }
	virtual SharedRef<Iterator<const Entry<K, V>>> End()   const override { return New<ConstEntryIterator>(*this, true ); }
};

template<HashableEquatable K, CopyAssignable V>
inline NullableRef<typename HashMap<K, V>::HashEntry> HashMap<K, V>::Get(const K& key) const
{
	HashCode hashCode = key.GetHashCode();

	const NullableRef<HashEntry>& first = GetBucket(hashCode);

	for(NullableRef<HashEntry> entry = first; entry != nullptr; entry = entry->GetNext())
	{
		if(entry->GetHashCode() == hashCode && entry->GetKey() == key)
			return entry;
	}

	return nullptr;
}

template<HashableEquatable K, CopyAssignable V>
inline V& HashMap<K, V>::operator[](const K& key)
{
	NullableRef<HashEntry> entry = Get(key);
	if(entry == nullptr)
		KeyNotFoundException(key).Throw();

	return entry->GetValue();
}

template<HashableEquatable K, CopyAssignable V>
inline const V& HashMap<K, V>::operator[](const K& key) const
{
	NullableRef<HashEntry> entry = Get(key);
	if(entry == nullptr)
		KeyNotFoundException(key).Throw();

	return entry->GetValue();
}

template<HashableEquatable K, CopyAssignable V>
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

	NullableRef<HashEntry> newEntry = New<HashEntry>(key, value, hashCode);

	if(last == nullptr)
		first = newEntry;
	else
		last->SetNext(newEntry);

	++m_count;
	return true;
}

template<HashableEquatable K, CopyAssignable V>
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

template<HashableEquatable K, CopyAssignable V>
inline Size HashMap<K, V>::AddRange(const IMap<K, V>& map)
{
	Size result = 0U;
	for(const Entry<K, V>& entry : map)
	{ 
		if(Add(entry.GetKey(), entry.GetValue()))
			++result;
	}

	return result;
}

template<HashableEquatable K, CopyAssignable V>
inline Size HashMap<K, V>::RemoveRange(const ICollection<K>& keys)
{
	Size result = 0U;
	for(const K& key : keys)
	{ 
		if(Remove(key))
			++result;
	}

	return result;
}

template<HashableEquatable K, CopyAssignable V>
inline Boolean HashMap<K, V>::TryGet(const K& key, V& outValue) const requires CopyAssignable<V>
{
	NullableRef<HashEntry> entry = Get(key);
	if(entry == nullptr)
		return false;

	outValue = entry->GetValue();
	return true;
}

template<HashableEquatable K, CopyAssignable V>
inline void HashMap<K, V>::Clear()
{
	for(Size i = 0U; i < m_buckets.Count(); i++)
		m_buckets[i] = nullptr;

	m_count = 0U;
}

template<HashableEquatable K, CopyAssignable V>
template<typename T>
inline HashMap<K, V>::HashIterator<T>::HashIterator(const HashMap<K, V>& map, Boolean isEnd) : 
	m_map(map), m_bucketIndex(0U), m_current(isEnd ? nullptr : map.m_buckets[0U])
{
	if(isEnd)
		return;

	while(m_current == nullptr && m_bucketIndex < map.m_buckets.Count() - 1U)
	{
		++m_bucketIndex;
		m_current = map.m_buckets[m_bucketIndex];
	}
}

template<HashableEquatable K, CopyAssignable V>
template<typename T>
inline void HashMap<K, V>::HashIterator<T>::MoveNext()
{
	m_current = m_current->GetNext();

	while(m_current == nullptr && m_bucketIndex < m_map.m_buckets.Count() - 1U)
	{ 
		++m_bucketIndex;
		m_current = m_map.m_buckets[m_bucketIndex];
	}
}

template<HashableEquatable K, CopyAssignable V>
template<typename T>
inline Entry<K, V>& HashMap<K, V>::HashIterator<T>::CurrentEntry() const { return *m_current; }

template<HashableEquatable K, CopyAssignable V>
template<typename T>
inline Boolean HashMap<K, V>::HashIterator<T>::Equals(const Iterator<T>& other) const
{
	const HashIterator& otherIterator = (const HashIterator&)other;
	return m_current == otherIterator.m_current;
}
