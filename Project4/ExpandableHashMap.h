#ifndef EXPANDABLEHASHMAP_INCLUDED
#define EXPANDABLEHASHMAP_INCLUDED

#include <vector>

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);
	
	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	struct Bucket
	{
		KeyType key;
		ValueType value;
		Bucket* next;
	};
	unsigned int getBucketNumber(const KeyType& key) const//private member function to retrieve bucket number data
	{
		unsigned int hasher(const KeyType & key);
		unsigned int h = hasher(key) % m_numBuckets;
		return h;
	}
	void freeMemory(std::vector<Bucket*> buckets, int numBuckets);
	void reHash();
	std::vector <Bucket*> m_BUCKETS;
	int m_numBuckets;
	int m_numAssociations;
	double m_maxLoadFactor;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	m_numBuckets = 8;
	m_numAssociations = 0;
	m_BUCKETS.resize(8);
	for (int i = 0; i < m_numBuckets; i++)
		m_BUCKETS[i] = nullptr;
	m_maxLoadFactor = maximumLoadFactor;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	freeMemory(m_BUCKETS, m_numBuckets);
	m_BUCKETS.clear();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	freeMemory(m_BUCKETS, m_numBuckets);
	m_BUCKETS.clear();
	m_numBuckets = 8;
	m_numAssociations = 0;
	m_BUCKETS.resize(8);
	for (int i = 0; i < m_numBuckets; i++)
		m_BUCKETS[i] = nullptr;
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_numAssociations;  // Delete this line and implement this function correctly
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	int bucketN1 = getBucketNumber(key);
	Bucket* itBucket = m_BUCKETS[bucketN1];

	if (itBucket == nullptr)
	{
		Bucket* b = new Bucket;
		b->key = key;
		b->value = value;
		b->next = nullptr;
		m_BUCKETS[bucketN1] = b;
		m_numAssociations++;
	}
	else
	{
		while (itBucket != nullptr)
		{
			if (itBucket->key == key)
			{
				itBucket->value = value; //update value if item key is already valid within the map
				return;
			}
			else if (itBucket->next == nullptr)
			{
				Bucket* b = new Bucket;
				b->key = key;
				b->value = value;
				b->next = nullptr;
				itBucket->next = b;
				m_numAssociations++;
				break;
			}
			itBucket = itBucket->next;
		}
	}

	if ((double)m_numAssociations / m_numBuckets > m_maxLoadFactor)
		reHash();
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	int bucket = getBucketNumber(key);
	Bucket* itBucket = m_BUCKETS[bucket];

	while (itBucket != nullptr)
	{
		if (itBucket->key == key)
			return &itBucket->value;
		itBucket = itBucket->next;
	}
	return nullptr;  // Delete this line and implement this function correctly
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reHash()
{
	int oldBucketCap = m_numBuckets;
	m_numBuckets *= 2;
	m_numAssociations = 0;
	std::vector<Bucket*> temp;
	temp = m_BUCKETS;
	m_BUCKETS.clear();
	m_BUCKETS.resize(m_numBuckets);
	for (int i = 0; i < m_numBuckets; i++)
		m_BUCKETS[i] = nullptr;

	for (int i = 0; i < temp.size(); i++)
	{
		Bucket* itBucket1 = temp[i];
		while (itBucket1 != nullptr)
		{
			associate(itBucket1->key, itBucket1->value);
			itBucket1 = itBucket1->next;
		}
	}

	freeMemory(temp, oldBucketCap);
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::freeMemory(std::vector<Bucket*> bucket, int numBuckets)
{
	for (int i = 0; i < numBuckets; i++)
	{
		Bucket* removeBuckets = bucket[i];
		while (removeBuckets != nullptr)
		{
			Bucket* tempBucket = removeBuckets->next;
			delete removeBuckets;
			removeBuckets = tempBucket;
		}
	}
}

#endif // !EXPANDABLEHASHMAP_INCLUDED