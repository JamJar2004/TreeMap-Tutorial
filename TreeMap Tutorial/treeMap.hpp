#pragma once

#include <queue>

template<typename K, typename V>
class Entry
{
private:
	K m_key;
	V m_value;

	Entry<K, V>* m_left;
	Entry<K, V>* m_right;

	Entry<K, V>* m_parent;
public:
	Entry(const K& key, const V& value, Entry<K, V>* parent) 
		: m_key(key), m_value(value), m_left(nullptr), m_right(nullptr), m_parent(parent) {}

	const K& GetKey()   { return m_key;   }
	      V& GetValue() { return m_value; }

	Entry<K, V>* GetLeft()  { return m_left;  }
	Entry<K, V>* GetRight() { return m_right; }

	Entry<K, V>* GetParent() { return m_parent; }

	void SetValue(const V& value) { m_value = value; }

	void SetLeft(Entry<K, V>* left)   { m_left  = left;  }
	void SetRight(Entry<K, V>* right) { m_right = right; }

	void SetParent(Entry<K, V>* parent) { m_parent = parent; }
};

template<typename T>
class IComparator
{
public:
	virtual int Compare(const T& left, const T& right) = 0;
};

template<typename K, typename V>
class KeyCollection;

template<typename K, typename V>
class ValueCollection;

template<typename K, typename V>
class EntryCollection;

template<typename K, typename V>
class TreeMap
{
private:
	IComparator<K>* m_comparator;

	Entry<K, V>* m_root;
	size_t       m_count;

	void PlaceInternal(Entry<K, V>* entry, Entry<K, V>* other);

	Entry<K, V>* Get(const K& key);
public:
	TreeMap(IComparator<K>& comparator) : m_root(nullptr), m_count(0), m_comparator(&comparator) {}

	inline size_t Count() { return m_count; }

	V& operator[](const K& key);

	bool Place(const K& key, const V& value);
	bool Remove(const K& key);

	bool ContainsKey(const K& key);

	void Clear();

	KeyCollection<K, V>   GetKeys(bool isEnd = false);
	ValueCollection<K, V> GetValues(bool isEnd = false);

	EntryCollection<K, V> GetEntries(bool isEnd = false);
};

template<typename K, typename V>
class TreeIterator
{
private:
	Entry<K, V>* m_currEntry;
public:
	TreeIterator() : m_currEntry(nullptr) {}
	TreeIterator(Entry<K, V>* root, bool isEnd) 
	{
		m_currEntry = root;

		if(!isEnd)
		{ 
			while(m_currEntry->GetLeft() != nullptr)
				m_currEntry = m_currEntry->GetLeft();
		}
		else
		{
			while(m_currEntry->GetRight() != nullptr)
				m_currEntry = m_currEntry->GetRight();
		}
	}

	Entry<K, V>* GetCurrentEntry() { return m_currEntry; }

	inline TreeIterator& operator++() 
	{
		if(m_currEntry->GetRight() != nullptr)
		{
			m_currEntry = m_currEntry->GetRight();
			while(m_currEntry->GetLeft() != nullptr)
				m_currEntry = m_currEntry->GetLeft();

			return *this;
		}

		while(true)
		{
			if(m_currEntry->GetParent() == nullptr)
			{
				m_currEntry = nullptr;
				return *this;
			}
			if(m_currEntry->GetParent()->GetLeft() == m_currEntry)
			{
				m_currEntry = m_currEntry->GetParent();
				return *this;
			}
			m_currEntry = m_currEntry->GetParent();
		}
	}

	inline TreeIterator& operator++(int) 
	{
		TreeIterator result = *this;
		++(*this);
		return result;
	}

	inline TreeIterator& operator--() 
	{
		Entry<K, V>* result = m_currEntry;

		if(m_currEntry->GetLeft() != nullptr)
		{
			m_currEntry = m_currEntry->GetLeft();
			while(m_currEntry->GetRight() != nullptr)
				m_currEntry = m_currEntry->GetRight();

			return result;
		}

		while(true)
		{
			if(m_currEntry->GetParent() == nullptr)
			{
				m_currEntry = nullptr;
				return result;
			}
			if(m_currEntry->GetParent()->GetRight() == m_currEntry)
			{
				m_currEntry = m_currEntry->GetParent();
				return result;
			}
			m_currEntry = m_currEntry->GetParent();
		}
	}

	inline TreeIterator& operator--(int) 
	{
		TreeIterator result = *this;
		--(*this);
		return result;
	}

	inline bool operator==(const TreeIterator& other) { return m_currEntry == other.m_currEntry; }
	inline bool operator!=(const TreeIterator& other) { return m_currEntry != other.m_currEntry; }
};

template<typename K, typename V>
class KeyIterator : public TreeIterator<K, V>
{
private:
public:
	KeyIterator()                              : TreeIterator<K, V>()            {}
	KeyIterator(Entry<K, V>* root, bool isEnd) : TreeIterator<K, V>(root, isEnd) {}

	K* operator->()
	{
		Entry<K, V>* currEntry = GetCurrentEntry();
		return &currEntry->GetKey();
	}

	const K& operator*()
	{
		Entry<K, V>* currEntry = GetCurrentEntry();
		return currEntry->GetKey();
	}
};

template<typename K, typename V>
class ValueIterator : public TreeIterator<K, V>
{
private:
public:
	ValueIterator() : TreeIterator<K, V>() {}
	ValueIterator(Entry<K, V>* root, bool isEnd) : TreeIterator<K, V>(root, isEnd) {}

	V* operator->()
	{
		Entry<K, V>* currEntry = GetCurrentEntry();
		return &currEntry->GetValue();
	}

	V& operator*()
	{
		Entry<K, V>* currEntry = GetCurrentEntry();
		return currEntry->GetValue();
	}
};

template<typename K, typename V>
class EntryIterator : public TreeIterator<K, V>
{
private:
public:
	EntryIterator() : TreeIterator<K, V>() {}
	EntryIterator(Entry<K, V>* root, bool isEnd) : TreeIterator<K, V>(root, isEnd) {}

	Entry<K, V>* operator->()
	{
		Entry<K, V>* currEntry = GetCurrentEntry();
		return currEntry;
	}

	Entry<K, V>& operator*()
	{
		Entry<K, V>* currEntry = GetCurrentEntry();
		return *currEntry;
	}
};

template<typename K, typename V>
class KeyCollection
{
private:
	Entry<K, V>* m_root;
	bool         m_isEnd;
public:
	KeyCollection(Entry<K, V>* root, bool isEnd) : m_root(root), m_isEnd(isEnd) {}

	inline KeyIterator<K, V> begin() { return KeyIterator<K, V>(m_root, m_isEnd); }
	inline KeyIterator<K, V> end()   { return KeyIterator<K, V>();                }
};

template<typename K, typename V>
class ValueCollection
{
private:
	Entry<K, V>* m_root;
	bool         m_isEnd;
public:
	ValueCollection(Entry<K, V>* root, bool isEnd) : m_root(root), m_isEnd(isEnd) {}

	inline ValueIterator<K, V> begin() { return ValueIterator<K, V>(m_root, m_isEnd); }
	inline ValueIterator<K, V> end()   { return ValueIterator<K, V>();                }
};

template<typename K, typename V>
class EntryCollection
{
private:
	Entry<K, V>* m_root;
	bool         m_isEnd;
public:
	EntryCollection(Entry<K, V>* root, bool isEnd) : m_root(root), m_isEnd(isEnd) {}

	inline EntryIterator<K, V> begin() { return EntryIterator<K, V>(m_root, m_isEnd); }
	inline EntryIterator<K, V> end()   { return EntryIterator<K, V>();                }
};

template<typename K, typename V>
KeyCollection<K, V> TreeMap<K, V>::GetKeys(bool isEnd)
{
	return KeyCollection<K, V>(m_root, isEnd);
}

template<typename K, typename V>
ValueCollection<K, V> TreeMap<K, V>::GetValues(bool isEnd)
{
	return ValueCollection<K, V>(m_root, isEnd);
}

template<typename K, typename V>
EntryCollection<K, V> TreeMap<K, V>::GetEntries(bool isEnd)
{
	return EntryCollection<K, V>(m_root, isEnd);
}

template<typename K, typename V>
bool TreeMap<K, V>::Place(const K& key, const V& value)
{
	Entry<K, V>* lastEntry = nullptr;
	Entry<K, V>* currEntry = m_root;

	bool right = false;

	while(currEntry != nullptr)
	{
		int comparison = m_comparator->Compare(key, currEntry->GetKey());
		if(comparison < 0)
		{
			lastEntry = currEntry;
			currEntry = currEntry->GetLeft();
			right = false;
		}
		else if(comparison > 0)
		{
			lastEntry = currEntry;
			currEntry = currEntry->GetRight();
			right = true;
		}
		else
		{
			currEntry->SetValue(value);
			return true;
		}
	}

	Entry<K, V>* newEntry = new Entry<K, V>(key, value, lastEntry);
	if(lastEntry == nullptr)
		m_root = newEntry;
	else
	{ 
		if(right)
			lastEntry->SetRight(newEntry);
		else
			lastEntry->SetLeft(newEntry);
	}
	m_count++;
	return false;
}

template<typename K, typename V>
Entry<K, V>* TreeMap<K, V>::Get(const K& key)
{
	Entry<K, V>* currEntry = m_root;

	while(currEntry != nullptr)
	{
		int comparison = m_comparator->Compare(key, currEntry->GetKey());
		if(comparison < 0)
		{
			currEntry = currEntry->GetLeft();
		}
		else if(comparison > 0)
		{
			currEntry = currEntry->GetRight();
		}
		else
		{
			return currEntry;
		}
	}

	return nullptr;
}

template<typename K, typename V>
V& TreeMap<K, V>::operator[](const K& key)
{
	Entry<K, V>* foundEntry = Get(key);
	if(foundEntry == nullptr)
	{
		V newValue;
		Place(key, newValue);
		foundEntry = Get(key);
	}
	return foundEntry->GetValue();
}

template<typename K, typename V>
bool TreeMap<K, V>::ContainsKey(const K& key)
{
	Entry<K, V>* foundEntry = Get(key);
	return foundEntry != nullptr;
}

template<typename K, typename V>
void TreeMap<K, V>::PlaceInternal(Entry<K, V>* entry, Entry<K, V>* other)
{
	Entry<K, V>* lastEntry = entry->GetParent();
	Entry<K, V>* currEntry = entry;

	bool right = false;

	while(currEntry != nullptr)
	{
		int comparison = m_comparator->Compare(other->GetKey(), currEntry->GetKey());
		if(comparison < 0)
		{
			lastEntry = currEntry;
			currEntry = currEntry->GetLeft();
			right = false;
		}
		else if(comparison > 0)
		{
			lastEntry = currEntry;
			currEntry = currEntry->GetRight();
			right = true;
		}
	}

	other->SetParent(lastEntry);
	if(lastEntry == nullptr)
		m_root = other;
	else
	{ 
		if(right)
			lastEntry->SetRight(other);
		else
			lastEntry->SetLeft(other);
	}
}

template<typename K, typename V>
bool TreeMap<K, V>::Remove(const K& key)
{
	Entry<K, V>* lastEntry = nullptr;
	Entry<K, V>* currEntry = m_root;

	bool right = false;

	while(currEntry != nullptr)
	{
		int comparison = m_comparator->Compare(key, currEntry->GetKey());
		if(comparison < 0)
		{
			lastEntry = currEntry;
			currEntry = currEntry->GetLeft();
			right = false;
		}
		else if(comparison > 0)
		{
			lastEntry = currEntry;
			currEntry = currEntry->GetRight();
			right = true;
		}
		else
		{
			Entry<K, V>* leftEntry  = currEntry->GetLeft();
			Entry<K, V>* rightEntry = currEntry->GetRight();

			delete currEntry;

			if(leftEntry != nullptr)
			{
				if(lastEntry == nullptr)
					m_root = leftEntry;
				else
				{ 
					if(right)
						lastEntry->SetRight(leftEntry);
					else
						lastEntry->SetLeft(leftEntry);
				}
				PlaceInternal(leftEntry, rightEntry);
				return true;
			}

			if(lastEntry == nullptr)
				m_root = rightEntry;
			else
			{
				if(right)
					lastEntry->SetRight(rightEntry);
				else
					lastEntry->SetLeft(rightEntry);
			}
			return true;
		}
	}

	return false;
}

template<typename K, typename V>
void TreeMap<K, V>::Clear()
{
	std::queue<Entry<K, V>*> entriesToDelete;
	entriesToDelete.push(m_root);

	while(!entriesToDelete.empty())
	{
		Entry<K, V>* currEntry = entriesToDelete.front();
		entriesToDelete.pop();

		if(currEntry->GetLeft() != nullptr)
			entriesToDelete.push(currEntry->GetLeft());

		if(currEntry->GetRight() != nullptr)
			entriesToDelete.push(currEntry->GetRight());

		delete currEntry;
	}

	m_count = 0;
	m_root  = nullptr;
}