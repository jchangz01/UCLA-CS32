#include <iostream>
#include "newMap.h"

using namespace std;

Map::Map()
{
	m_maxSize = DEFAULT_MAX_ITEMS;
	m_data = new Data[m_maxSize];
	m_size = 0;
	// Create an empty map (i.e., one with no key/value pairs)
}

Map::Map(int maxsize)
{
	if (maxsize <= 0)
	{
		cout << "ERROR!, map size must be greater than 0." << endl;
		exit(1);
	}
	m_maxSize = maxsize;
	m_data = new Data[m_maxSize];
	m_size = 0;
}

Map::Map(const Map& src)
{
	m_maxSize = src.m_maxSize;
	m_data = new Data[m_maxSize];
	m_size = src.m_size;
	for (int i = 0; i < m_size; i++)
	{
		m_data[i].m_key = src.m_data[i].m_key;
		m_data[i].m_value = src.m_data[i].m_value;
	}
}

Map&Map::operator=(const Map& src)
{
	if (&src == this)
	{
		return *this;
	}
	delete[] m_data;
	m_maxSize = src.m_maxSize;
	m_data = new Data[m_maxSize];
	m_size = src.m_size;
	for (int i = 0; i < m_size; i++)
	{
		m_data[i].m_key = src.m_data[i].m_key;
		m_data[i].m_value = src.m_data[i].m_value;
	}
	return *this;
}

Map::~Map()
{
	delete [] m_data;
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
	if (m_size == 0)
		return true;
	else
		return false;
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_data[i].m_key == key)
			return false;
	}

	if (m_size >= m_maxSize)
		return false;
	else
	{
		m_data[m_size].m_key = key;
		m_data[m_size].m_value = value;
		m_size++;
		return true;
	}
	// If key is not equal to any key currently in the map, and if the
	// key/value pair can be added to the map, then do so and return true.
	// Otherwise, make no change to the map and return false (indicating
	// that either the key is already in the map, or the map has a fixed
	// capacity and is full).
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_data[i].m_key == key)
		{
			m_data[i].m_value = value;
			return true;
		}
	}
	return false;
	// If key is equal to a key currently in the map, then make that key no
	// longer map to the value it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// Otherwise, make no change to the map and return false.
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_data[i].m_key == key)
		{
			m_data[i].m_value = value;
			return true;
		}
	}

	if (m_size >= m_maxSize)
		return false;
	else
	{
		m_data[m_size].m_key = key;
		m_data[m_size].m_value = value;
		m_size++;
		return true;
	}

	// If key is equal to a key currently in the map, then make that key no
	// longer map to the value that it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// If key is not equal to any key currently in the map and if the
	// key/value pair can be added to the map, then do so and return true.
	// Otherwise, make no change to the map and return false (indicating
	// that the key is not already in the map and the map has a fixed
	// capacity and is full).
}

bool Map::erase(const KeyType& key)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_data[i].m_key == key)
		{
			m_size--;
			for (int j = i; j < m_size; j++)
			{
				m_data[j].m_key = m_data[j + 1].m_key;
				m_data[j].m_value = m_data[j + 1].m_value;
			}
			return true;
		}
	}
	return false;
	// If key is equal to a key currently in the map, remove the key/value
	// pair with that key from the map and return true.  Otherwise, make
	// no change to the map and return false.
}

bool Map::contains(const KeyType& key) const
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_data[i].m_key == key)
			return true;
	}
	return false;
	// Return true if key is equal to a key currently in the map, otherwise
	// false.
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_data[i].m_key == key)
		{
			value = m_data[i].m_value;
			return true;
		}
	}
	return false;
	// If key is equal to a key currently in the map, set value to the
	// value  in the map which that key maps to, and return true.  Otherwise,
	// make no change to the value parameter of this function and return
	// false.
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < size())
	{
		key = m_data[i].m_key;
		value = m_data[i].m_value;
		return true;
	}
	else
		return false;
	// If 0 <= i < size(), copy into the key and value parameters the
	// key and value of one of the key/value pairs in the map and return
	// true.  Otherwise, leave the key and value parameters unchanged and
	// return false.  (See below for details about this function.)
}

void Map::swap(Map& other)
{
	Data* store;
	int store_maxSize = m_maxSize;
	store = new Data [store_maxSize];
	int store_size = m_size;
	for (int i = 0; i < store_size; i++)
	{
		store[i].m_key = m_data[i].m_key;
		store[i].m_value = m_data[i].m_value;
	}

	delete[] m_data;
	m_maxSize = other.m_maxSize;
	m_data = new Data[m_maxSize];
	m_size = other.m_size;
	for (int i = 0; i < m_size; i++)
	{
		m_data[i].m_key = other.m_data[i].m_key;
		m_data[i].m_value = other.m_data[i].m_value;
	}

	delete[] other.m_data;
	other.m_maxSize = store_maxSize;
	other.m_data = new Data[other.m_maxSize];
	other.m_size = store_size;
	for (int i = 0; i < other.m_size; i++)
	{
		other.m_data[i].m_key = store[i].m_key;
		other.m_data[i].m_value = store[i].m_value;
	}

	delete[] store;
	// Exchange the contents of this map with the other one.
}

