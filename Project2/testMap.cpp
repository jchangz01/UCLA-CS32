#include <iostream>
#include <cassert>
#include "Map.h"

using namespace std;

int main()
{
	//strings to doubles
	//default constructor
	Map m1;
	//check insert, erase, and empty
	assert(m1.empty()); //default map m1 is empty
	assert(m1.size() == 0); //size of map m1 is 0
	assert(m1.insert("dog", 3)); //map now contains a node
	assert(!m1.insert("dog", 20)); //returns false because "dog" is already in m 
	assert(m1.erase("dog")); //erases dog pair and returns true
	assert(m1.empty()); //m1 should be empty again

	//add pairs to map and checks functionality of Map::contain and Map::get
	m1.insert("cat", 2);
	m1.insert("rat", 21);
	m1.insert("snake", 23);
	m1.insert("bat", 33);
	m1.erase("rat");
	assert(m1.contains("snake")); //returns true because m possesses the key snake
	assert(!m1.contains("rat"));  //returns false because m no longer possesses the key rat
	string s;
	double v = 40;
	double u;
	assert(m1.get("snake", u) && (u == 23)); //returns true because u gets snake pair's value 23
	assert(m1.get(0, s, v) && s == "bat" && v == 33); //returns true because s and v gets the node of latest pair inserted 
	//m1 now contains bat - 33 (head), snake - 23, and cat - 2(tail)

	//check functionality of Map::update and Map::insertOrUpdate
	assert(m1.update("bat", 3)); // returns true and bat's value pair is now 3
	assert(!m1.update("dog", 10)); //returns false because dog is not in m1
	assert(m1.update("snake", 24)); //returns true and snake's value is now 24
	assert(m1.insertOrUpdate("anaconda", 100)); //returns true and ananconda with value pair 100 is added to m
	assert(m1.get("snake", u) && (u == 24)); //returns true because u get snake pair's new value 24
	assert(m1.get(0, s, v) && !(s == "bat") && !(v == 3)); //returns false because the head of the Map now points to anaconda and 100
	////m1 now contains anaconda - 100 (head) bat - 3, snake - 24, and cat - 2(tail)
	assert(m1.size() == 4); //m1 now has 4 pairs
	
	//Define m2 with copy constructor on m1
	Map m2 = m1;
	assert(m2.size() == 4);
	assert(!m2.erase("rat")); //returns false because rat is not in m2
	assert(m2.erase("bat")); //returns true and bat is deleted along with its value pair
	assert(m2.erase("snake")); //returns true and snake is deleted along with its value pair
	//m2 contains anaconda - 100 (head) and cat - 2(tail)

	//Define m3 
	Map m3;
	m3 = m2; //test assignment operator
	assert(m2.contains("anaconda")); //returns true because anaconda is in m2
	m3.erase("anaconda"); //returns true and anaconda is deleted along with its value pair
	//m3 contains cat - 2 (head and tail)

	//Test Map::swap
	m3.swap(m1); 
	assert(m1.size() == 1); //m1 new size is now 1
	assert(m3.size() == 4); //m3 new size is now 4

	//test combine();
	Map m4; //Define m4 with default constructor
	m4.insert("turtle", 131);
	m4.insert("fish", 3211);
	m4.insert("dino", 123);
	m4.insert("swordfish", 32112);
	assert(combine(m4, m3, m1)); //returns true because no duplicate pairs and m1 now has pairs from both m4 and m3
	//m1 now contains anaconda - 100 (head), bat - 3, snake - 24, cat - 2, swordfish - 32112, dino - 123, fish - 3211, turtle - 131 (tail)
	
	//test reassign()
	Map m5; //Define m5 with default constructor
	m5.insert("bunny", 05); 
	reassign(m1, m5); //m5 loses its pairs and now contains all of m4 pairs but with scrambled keys and values
	//m5 now contains anaconda - 131 (head), bat - 100, snake - 3, cat - 24, swordfish - 2, dino - 32112, fish - 123, turtle - 3211(tail)

	cout << "Passed All Tests!" << endl; 

	/*//ints to strings
	//default constructor
	Map m1;
	//check insert, erase, and empty
	assert(m1.empty()); //default map m1 is empty
	assert(m1.size() == 0); //size of map m1 is 0
	assert(m1.insert(3, "dog")); //map now contains a node
	assert(!m1.insert(3, "do")); //returns false because 3 is already in m 
	assert(m1.erase(3)); //erases 3 pair and returns true
	assert(m1.empty()); //m1 should be empty again

	//add pairs to map and checks functionality of Map::contain and Map::get
	m1.insert(2, "cat");
	m1.insert(21 ,"rat");
	m1.insert(23, "snake");
	m1.insert(33, "bat");
	m1.erase(21);
	assert(m1.contains(23)); //returns true because m possesses the key 23
	assert(!m1.contains(21));  //returns false because m no longer possesses the key 21
	int s;
	string v = "xxx";
	string u;
	assert(m1.get(23, u) && (u == "snake")); //returns true because u gets key 23's value pair
	assert(m1.get(0, s, v) && s == 33 && v == "bat"); //returns true because s and v gets the node of latest pair inserted 
	//m1 now contains 33 - bat (head), 23 - snake, and 2 - cat (tail)

	//check functionality of Map::update and Map::insertOrUpdate
	assert(m1.update(33, "bats")); // returns true and 3's value pair is now bat
	assert(!m1.update(10, "dog")); //returns false because 10 is not in m1
	assert(m1.update(23, "snakes")); //returns true and 23's value is now snakes
	assert(m1.insertOrUpdate(100, "anaconda")); //returns true and 100 with value pair ananconda is added to m
	assert(m1.get(23, u) && (u == "snakes")); //returns true because u gets 23's pair snakes
	assert(m1.get(0, s, v) && !(s == 33) && !(v == "bats")); //returns false because the head of the Map now points to 100 and anaconda
	////m1 now contains 100 - anaconda(head), 33 - bats, 23 - snakes, and 2 - cat(tail)
	assert(m1.size() == 4); //m1 now has 4 pairs

	//Define m2 with copy constructor on m1
	Map m2 = m1;
	assert(m2.size() == 4);
	assert(!m2.erase(21)); //returns false because 21 is not in m2
	assert(m2.erase(33)); //returns true and 33 is deleted along with its value pair
	assert(m2.erase(23)); //returns true and 23 is deleted along with its value pair
	//m2 contains 100 - anaconda (head) and 2 - cat(tail)

	//Define m3 
	Map m3;
	m3 = m2; //test assignment operator
	assert(m2.contains(100)); //returns true because 100 is in m2
	m3.erase(100); //returns true and 100 is deleted along with its value pair
	//m3 contains 2 - cat (head and tail)

	//Test Map::swap
	m3.swap(m1);
	assert(m1.size() == 1); //m1 new size is now 1
	assert(m3.size() == 4); //m3 new size is now 4

	//test combine();
	Map m4; //Define m4 with default constructor
	m4.insert(131, "turtle");
	m4.insert(3211, "fish");
	m4.insert(123, "dino");
	m4.insert(32112, "swordfish");
	assert(combine(m4, m3, m1)); //returns true because no duplicate pairs and m1 now has pairs from both m4 and m3
	//m1 now contains 100 - anaconda (head), 3 - bats, 24 - snakes, 2 - cat, 32112 - swordfish, 123 - dino, 3211 - fish, 131 - turtle(tail)

	//test reassign()
	Map m5; //Define m5 with default constructor
	m5.insert(05, "bunny");
	reassign(m1, m5); //m5 loses its pairs and now contains all of m4 pairs but with scrambled keys and values
	//m5 now contains 100 - turtle (head), 3 - anaconda, 24 - bats, 2 - snakes, 32112 - cat, 123 - swordfish, 3211 - dino, 131 - fish(tail)

	cout << "Passed All Tests!" << endl; */
}

//void dumpTail();
//void dumpHead();
/*void Map::dumpTail()
{
	Node* p;
	p = tail; // p points to 1st node

	while (p != nullptr) // or while ( p ) bc C++ evaluates true for > 1 and false for <= 0
	{
		cout << p->m_key << endl;
		cout << p->m_value << endl;
		p = p->previous;
	}
}

void Map::dumpHead()
{
	Node* p;
	p = head; // p points to 1st node

	while (p != nullptr) // or while ( p ) bc C++ evaluates true for > 1 and false for <= 0
	{
		cout << p->m_key << endl;
		cout << p->m_value << endl;
		p = p->next;
	}
} */