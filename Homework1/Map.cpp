#include "Map.h"

Map::Map()
{
	m_size = 0;
	// Create an empty map (i.e., one with no key/value pairs)
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

	if (m_size >= DEFAULT_MAX_ITEMS)
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

	if (m_size >= DEFAULT_MAX_ITEMS)
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
				m_data[j].m_key = m_data[j+1].m_key;
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

bool Map:: contains(const KeyType& key) const
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
	if (other.size() > size())
	{
		for (int i = size(); i < other.size(); i++)
		{
			m_data[i].m_key = other.m_data[i].m_key;
			m_data[i].m_value = other.m_data[i].m_value;
		}

		for (int i = 0; i < size(); i++)
		{
			KeyType storeKey = m_data[i].m_key;
			ValueType storeValue = m_data[i].m_value;

			m_data[i].m_key = other.m_data[i].m_key;
			m_data[i].m_value = other.m_data[i].m_value;
			other.m_data[i].m_key = storeKey;
			other.m_data[i].m_value = storeValue;
		}
	}
	else
	{
		for (int i = other.size(); i < size(); i++)
		{
			other.m_data[i].m_key = m_data[i].m_key;
			other.m_data[i].m_value = m_data[i].m_value;
		}

		for (int i = 0; i < other.size(); i++)
		{
			KeyType storeKey = m_data[i].m_key;
			ValueType storeValue = m_data[i].m_value;

			m_data[i].m_key = other.m_data[i].m_key;
			m_data[i].m_value = other.m_data[i].m_value;
			other.m_data[i].m_key = storeKey;
			other.m_data[i].m_value = storeValue;
		}
	}
	int storeSize = m_size;
	m_size = other.m_size;
	other.m_size = storeSize;
	// Exchange the contents of this map with the other one.
}

