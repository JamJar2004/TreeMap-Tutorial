#pragma once

template<typename K, typename V>
class Entry
{
private:
	K m_key;
	V m_value;
public:
	Entry(const K& key, const V& value) : m_key(key), m_value(value) {}

	const K& GetKey()   { return m_key;   }
	      V& GetValue() { return m_value; }

	void SetValue(const V& value) { m_value = value; }
};

template<typename K, typename V>
class TreeMap
{
private:
public:
	inline size_t Count() { return 0 }

	V& operator[](const K& key);

	bool Place(const K& key, const V& value);
	bool Remove(const K& key);

	bool ContainsKey(const V& value);

	void Clear();

	KeyCollection<K, V>   GetKeys();
	ValueCollection<K, V> GetValues();

	EntryCollection<K, V> GetEntries();
};

template<typename K, typename V>
class TreeIterator
{
private:
public:
	TreeIterator() {}
};

template<typename K, typename V>
class KeyIterator
{
private:
public:
};

template<typename K, typename V>
class ValueIterator
{
private:
public:
};

template<typename K, typename V>
class EntryIterator
{
private:
public:
};

template<typename K, typename V>
class KeyCollection
{
private:
public:
};

template<typename K, typename V>
class ValueCollection
{
private:
public:
};

template<typename K, typename V>
class EntryCollection
{
private:
public:
};