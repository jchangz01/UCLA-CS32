#include <iostream>
#include "Map.h"

using namespace std;

bool combine(const Map& m1, const Map& m2, Map& result)
{
	bool noDuplicates = true;
	Map store1;
	Map store2;
	
	//Store all pairs of m1 into store1
	for (int i = 0; i < m1.size(); i++)
	{
		KeyType storeKey1;
		ValueType storeValue1;
		m1.get(i, storeKey1, storeValue1);
		store1.insert(storeKey1, storeValue1);
	}

	//store all pairs of m2 into store2
	for (int i = 0; i < m2.size(); i++)
	{
		KeyType storeKey2;
		ValueType storeValue2;
		m2.get(i, storeKey2, storeValue2);
		store2.insert(storeKey2, storeValue2);
	}

	//erases all previous values from Map result
	for (int i = 0; i < result.size(); i++)
	{
		KeyType eraseKey;
		ValueType eraseValue;
		result.get(i, eraseKey, eraseValue);
		result.erase(eraseKey);
		i--;
	}

	//add values of store1 and store2 into result
	for (int i = 0; i < store1.size(); i++)
	{
		KeyType keyM1;
		ValueType valueM1;
		store1.get(i, keyM1, valueM1);
		result.insert(keyM1, valueM1);
	}
	for (int i = 0; i < store2.size(); i++)
	{
		KeyType keyM2;
		ValueType valueM2;
		store2.get(i, keyM2, valueM2);
		if (!result.insert(keyM2, valueM2))
		{
			ValueType checkValue;
			if (result.get(keyM2, checkValue) && (checkValue == valueM2))
			{
				continue;	
			}
			else
			{
				result.erase(keyM2);
				noDuplicates = false;
			}
		}
	}
	return noDuplicates;
}

void reassign(const Map& m, Map& result)
{
	//store all values in Map m into Map store
	Map store;
	for (int i = 0; i < m.size(); i++)
	{
		KeyType storeKey;
		ValueType storeValue;
		m.get(i, storeKey, storeValue);
		store.insert(storeKey, storeValue);
	}

	//erases all previous values from Map result
	for (int i = 0; i < result.size(); i++)
	{
		KeyType eraseKey;
		ValueType eraseValue;
		result.get(i, eraseKey, eraseValue);
		result.erase(eraseKey);
		i--;
	}

	//rearranges Map store's pairs in Map result
	for (int i = 0; i < store.size(); i++)
	{
		KeyType reKey1;
		ValueType reValue1;
		KeyType reKey2;
		ValueType reValue2;

		if (i == store.size() - 1)
		{
			store.get(i, reKey1, reValue1);
			store.get(0, reKey2, reValue2);
			result.insert(reKey1, reValue2);
		}
		else
		{
			store.get(i, reKey1, reValue1);
			store.get(i + 1, reKey2, reValue2);
			result.insert(reKey1, reValue2);
		}
	}
}

///////////////////////////////////
/*Member Function Implementations*/
///////////////////////////////////
Map::Map()
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

Map::Map(const Map& src)
{
	m_size = 0;
	head = nullptr;
	tail = nullptr;

	Node* pCopy = src.tail;

	while (pCopy != nullptr)
	{
		insert(pCopy->m_key, pCopy->m_value);
		pCopy = pCopy->previous;
	}
}

Map& Map::operator=(const Map& src)
{
	if (&src == this) //return itself if we set an object to itself
		return *this;

	Node* p;
	p = head;
	while (p != nullptr)
	{
		Node* n = p->next;
		delete p;
		p = n;
	} //deletes all memory addresses in the current node

	
	m_size = 0;
	head = nullptr;
	tail = nullptr;

	Node* mCopy = src.tail;

	while (mCopy != nullptr)
	{
		insert(mCopy->m_key, mCopy->m_value);
		mCopy = mCopy->previous;
	}
	return *this;
}

Map::~Map()
{
	Node* p;
	p = head;
	while (p != nullptr)
	{
		Node* n = p->next;
		delete p;
		p = n;
	}
}

bool Map::empty() const
{
	if (head == nullptr)
		return true;
	else
		return false;
}

int Map::size() const
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	//Check if key is already in linked list
	Node* forward = head;
	Node* reverse = tail;
	while (forward != nullptr && reverse != nullptr)
	{
		if (forward->m_key == key)
			return false;
		if (reverse->m_key == key)
			return false;
		forward = forward->next;
		reverse = reverse->previous;
	}

	//Adds new node
	Node* p;
	p = new Node;
	p->m_key = key; //put key into m_key in node
	p->m_value = value; //put value into m_value in node
	p->next = head;
	p->previous = nullptr;
	if (head != nullptr) //if map is not empty, have previous point to p (the new node)
		head->previous = p;
	else
		tail = p; //if map is empty, tail points to first node
	head = p;
	m_size++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	Node* forward = head;
	Node* reverse = tail;
	while (forward != nullptr && reverse != nullptr) //search linked list inward using both the head and tail
	{
		if (forward->m_key == key) 
		{
			forward->m_value = value;
			return true;
		}
		if (reverse->m_key == key)
		{
			reverse->m_value = value;
			return true;
		}
		forward = forward->next;
		reverse = reverse->previous;
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	Node* forward = head;
	Node* reverse = tail;
	while (forward != nullptr && reverse != nullptr) //search linked list inward using both head and tail
	{
		if (forward->m_key == key)
		{
			forward->m_value = value;
			return true;
		}
		if (reverse->m_key == key)
		{
			reverse->m_value = value;
			return true;
		}
		forward = forward->next;
		reverse = reverse->previous;
	}

	Node* p;
	p = new Node;
	p->m_key = key; //put key into m_key in node
	p->m_value = value; //put value into m_value in node
	p->next = head;
	p->previous = nullptr;
	if (head != nullptr) //if map is not empty, have previous point to p (the new node)
		head->previous = p;
	else
		tail = p; //if map is empty, tail points to first node
	head = p;
	m_size++;
	return true;
}

bool Map::erase(const KeyType& key)
{
	if (head == nullptr)
	{
		return false;
	}

	if (head->m_key == key)
	{
		Node* erase = head;
		head = erase->next;
		if (head != nullptr)
			head->previous = nullptr;
		else //if there is only one node, change value tail points to
			tail = erase->previous;
		m_size--;
		delete erase;
		return true;
	}  //for cases when deleting first node

	if (tail->m_key == key)
	{
		Node* erase = tail;
		tail = erase->previous;
		if (tail != nullptr)
			tail->next = nullptr;
		else //if there is noly one node, change value head points to 
			head = erase->next;
		m_size--;
		delete erase;
		return true;
	} //for cases when deleting final node

	Node* p = head;
	while (p != nullptr)
	{
		if (p->next != nullptr && p->next->m_key == key)
			break;
		p = p->next;
	}
	if (p != nullptr)
	{
		Node* erase = p->next;
		erase->next->previous = p; //changes previous to new prev node
		p->next = erase->next;
		delete erase;
		m_size--;
		return true;
	}
	return false;
}

bool Map::contains(const KeyType& key) const
{
	Node* forward = head;
	Node* reverse = tail;
	while (forward != nullptr && reverse != nullptr) //search linked list inward using both head and tail
	{
		if (forward->m_key == key)
			return true;
		if (reverse->m_key == key)
			return true;
		forward = forward->next;
		reverse = reverse->previous;
	}
	return false;
}
bool Map::get(const KeyType& key, ValueType& value) const
{
	Node* forward = head;
	Node* reverse = tail;
	while (forward != nullptr && reverse != nullptr) //search linked list inward using both head and tail
	{
		if (forward->m_key == key)
		{
			value = forward->m_value;
			return true;
		}
		if (reverse->m_key == key)
		{
			value = reverse->m_value;
			return true;
		}
		forward = forward->next;
		reverse = reverse->previous;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i < 0 && i >= m_size)
		return false;
	else {
		Node* p = head;
		for (int j = 1; j < i + 1; j++)
			p = p->next;
		key = p->m_key;
		value = p->m_value;
		return true;
	}
}

void Map::swap(Map& other)
{
	Map store = other;
	other = *this;
	*this = store;
}
