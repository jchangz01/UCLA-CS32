#include "newMap.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

int main()
{
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = {"dog", "cat", "rat", "mouse", "monkey", "dragon"};
    KeyType kc[240];
    ValueType v = 20;

    // Check if all Map objects are empty
    assert(a.empty());
    assert(b.empty());
    assert(c.empty());

    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));

    //No failures inserting pairs with 240 distinct keys into c
    for (int i = 0; i < 240; i++)
        assert(c.insert(to_string(i), v));

    // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));

    // When two Maps' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(k[5], v) && b.insert(k[5], v));
    assert(a.size() == 5 && b.size() == 1);
    assert(a.update("dog", 10));
    assert(a.erase("dog") && b.erase("dragon"));
    assert(a.size() == 4 && b.size() == 0);

    //swap objects b and c (size, capacities, and keys)
    c.swap(b);
    assert(!b.insert("241.000000", v) && c.insertOrUpdate("2.000000", v));
    assert(b.size() == 240 && c.size() == 1);
    cout << "Passed all tests" << endl;
} 