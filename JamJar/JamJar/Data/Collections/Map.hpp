#pragma once

#include "Collection.hpp"

template<typename K, typename V>
class Entry
{
private:
	const K m_key;
	      V m_value;
public:
	Entry(const K& key, const V& value) : m_key(key), m_value(value) {}

	const K& GetKey()   const { return m_key;   }
	const V& GetValue() const { return m_value; }

	V& GetValue() { return m_value; }

	void SetValue(const V& value) { m_value = value; }
};

template<typename K, typename V>
class IMap : public IResizableCollection<Entry<K, V>>
{
public:
	virtual       V& operator[](const K& key)       = 0;
	virtual const V& operator[](const K& key) const = 0;

	virtual Boolean    Add(const K& key, const V& value) = 0;
	virtual Boolean Remove(const K& key                ) = 0;

	virtual Size    AddRange(const IMap<K, V>& map     ) = 0;
	virtual Size RemoveRange(const ICollection<K>& keys) = 0;

	virtual Boolean TryGet(const K& key, V& outValue) = 0;

	virtual const ICollection<const K> GetKeys()   const = 0;
	virtual const ICollection<      V> GetValues() const = 0;
};

template<Comparable K, typename V>
class ISortedMap : public IMap<K, V>
{
public:
	virtual Entry<K, V>& GetClosestSmaller(const K& key) = 0;
	virtual Entry<K, V>& GetClosestGreater(const K& key) = 0;

	virtual const Entry<K, V>& GetClosestSmaller(const K& key) const = 0;
	virtual const Entry<K, V>& GetClosestGreater(const K& key) const = 0;
};