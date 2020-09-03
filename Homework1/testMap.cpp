/***************************
    Test strings to doubles
***************************/
#include "Map.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{
    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType v = -1234.5;
    assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5); //adds a data point to map m
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v) && k == "xyz" && v == 9876.5);
    assert(!m.insert("xyz", 1349013.0)); //return false bc "xyz" already exist in map

    //adds key 1-240 until map m is filled with data
    for (double i = 0; i < 240; i++)
    {
        string pkey;
        double pvalue;
        m.insert(to_string(i), i);
        assert(m.get(i, pkey, pvalue));
    }

    Map m2; //create second map
    assert(m.size() == 240);
    assert(m2.empty());
    m2.swap(m); //swap values and keys of m and m2
  
    //test for functionality of update and insertOrUpdate ("" is treated as a normal string)
    m.insertOrUpdate("", 3189.00); 
    m.update("", 31.32);
    double value;
    m.get("", value);
    assert(value == 31.32);

    //checks sizes of m and m2
    assert(m.size() == 1);
    assert(m2.size() == 240);

    //test for functionality of contains and erase
    assert(m2.contains("5.000000"));
    assert(m2.erase("5.000000"));
    assert(!m2.contains("5.000000"));

    cout << "Passed all tests" << endl;
} 


/***************************
    Test ints to strings
***************************/
/*#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map m;  // maps ints to strings
    assert(m.empty());
    ValueType v = "Ouch";
    assert(!m.get(42, v) && v == "Ouch"); // v unchanged by get failure
    m.insert(123456789, "Wow!");
    assert(m.size() == 1);
    KeyType k = 9876543;
    assert(m.get(0, k, v) && k == 123456789 && v == "Wow!");
    assert(!m.insert(123456789, "xyz")); //return false bc 123456789 already exist in map
    
    //adds key 1-240 until map m is filled with data
    for (int i = 0; i < 240; i++)
    {
        string pkey;
        int pvalue;
        m.insert(i, "i");
        assert(m.get(i, pvalue, pkey));
    }

    Map m2; //create second map
    assert(m.size() == 240);
    assert(m2.empty());
    m2.swap(m); //swap values and keys of m and m2

    //test for functionality of update and insertOrUpdate ("" is treated as a normal string)
    m.insertOrUpdate(3189, "");
    m.update(32, "");
    string value;
    m.get(32, value);
    assert(value == "");

    //checks sizes of m and m2
    assert(m.size() == 1);
    assert(m2.size() == 240);

    //test for functionality of contains and erase
    assert(m2.contains(5));
    assert(m2.erase(5));
    assert(!m2.contains(5));

    cout << "Passed all tests" << endl;
} */
