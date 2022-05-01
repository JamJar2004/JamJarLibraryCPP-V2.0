#pragma once

#include "ICollection.hpp"

template<typename K, typename V>
class Entry
{
private:
	K m_key;
	V m_value;
public:
	Entry(const K& key, const V& value) : m_key(key), m_value(value) {}

	const K& GetKey()   const { return m_key;   }
	const V& GetValue() const { return m_value; }

	void SetValue(const V& value) { m_value = value; }
};

template<typename K, typename V>
class IMap
{
public:
	Size Count();

	Boolean Place(const K& key, const V& value);
	Boolean Remove(const K& key);

	Size PlaceRange(const IMap<K, V>& map);
	Size RemoveRange(const ICollection<K>& keys);

	void Clear();

	SharedRef<ICollection<K>>           GetKeys()    const;
	SharedRef<ICollection<V>>           GetValues()  const;
	SharedRef<ICollection<Entry<K, V>>> GetEntries() const;
};