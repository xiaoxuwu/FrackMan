#include "Map.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	// default constructor
	Map m;
	Map m2;

	// For an empty map:
	assert(m.size() == 0);      // test size
	assert(m.empty());          // test empty
	assert(!m.erase("Ricky"));  // nothing to erase
	assert(!m.update("Ricky", 1));     // can’t update nonexistent key
	assert(!m.contains("Ricky"));   // test doesn’t contain key
	ValueType value;
	assert(!m.get("Ricky", value)); // test get if key is not in map
	KeyType key;
	//assert(!m.get(0, key, value));  // test get if map is empty

	// test insert, update, and insertOrUpdate
	assert(m.insert("Ricky", 344));    // test insert
	assert(m.size() == 1 && !m.empty());    // test if size updated
	assert(!m.update("Lucy", 123)); // can’t update nonexistent key
	assert(m.contains("Ricky"));    // m should contain key “Ricky” now
	assert(!m.insert("Ricky", 122));    // should not be allowed to insert existing key
	assert(m.update("Ricky", 122)); // update Ricky’s value to 122
	assert(m.insertOrUpdate("Ricky", 123)); // should update Ricky’s value to 123
	assert(m.insertOrUpdate("Lucy", 1));    // should insert Lucy
	assert(m.size() == 2);   // make sure map’s size is updating

    /*
	// test both get methods
	assert(m.get("Ricky", value) && value == 123);  // test if get function works properly
	assert(m.get("Lucy", value) && value == 1); // same as above
	assert(!m.get("Bob", value) && value == 1);       // key not in map, value shouldn’t change
	// key and value must equal the first keyvalue pair in map
	assert(m.get(0, key, value) && key == "Ricky" && value == 123);
	// key and value should match second keyvalue pair in map
	assert(m.get(1, key, value) && key == "Lucy" && value == 1);
	assert(!m.get(2, key, value));  // 2 is greater than mySize
    */
	//test erase
	assert(!m.erase("Bob")); // can’t erase nonexistent key
	assert(m.erase("Ricky"));	// Ricky should erase properly
	//assert(m.get(0, key, value) && key == "Lucy" && value == 1);  // test if Lucy is now first
	assert(!m.contains("Ricky"));   // test that Ricky is no longer in map
	assert(m.size() == 1);  // make sure size updated
	assert(!m.get("Ricky", value));    // can’t get Ricky
	assert(!m.erase("Ricky"));	// can't erase Ricky again

	//test swap
	assert(m2.insert("Bob", 24));
	assert(m2.insert("Kate", 52.5));
	m.swap(m2);
	assert(m.size() == 2 && m2.size() == 1); // check if sizes swapped properly
	assert(m.get("Bob", value) && value == 24); // check if m1 contains m2’s data
	assert(m.get("Kate", value) && value == 52.5);
	assert(m2.get("Lucy", value) && value == 1);    // check if m2 contains m1’s data
	assert(!m.get("Lucy", value) && value == 1);    // check that m1 doesn’t have old data
	assert(!m2.get("Bob", value) && value == 1); // check that m2 doesn’t have old data
	Map a;
	Map b;
	b.insert("Test", 0);
	a.swap(b);
	// test swap for nonempty and empty map
	assert(a.contains("Test") && !b.contains("Test"));

	//test assignment and copy constructor
	Map n = m;
	assert(n.size() == m.size());   // test if sizes are same
	//assert(n.get(0, key, value) && key == "Bob" && value == 24);    // test if contains m’s data
	Map q;
	n = q;
	assert(n.size() == 0);  // check that n is empty
	// check n doesn’t have any of its old values
	//assert(!n.get(0, key, value) && key == "Bob" && value == 24);

	// test combine
	Map result;
	assert(combine(n, q, result)); // test combine for empty maps
	assert(combine(m, m2, result)); // test combine for maps with no same keys
	assert(result.get("Bob", value) && result.get("Lucy", value)); // check if contains data from both maps
	m.insert("Lucy", 1);
	assert(combine(m, m2, result)); // test combine for maps with same key same value
	assert(result.get("Lucy", value)); // result should contain key Lucy
	m.update("Lucy", 2);
	assert(!combine(m, m2, result));    // return false because same key different values
	assert(!result.get("Lucy", value));    // result should not contain Lucy

	// test subtract
	subtract(n, q, result);
	assert(result.size() == 0); // test subtract for empty maps
	m2.insert("Faith", 22.2);
	subtract(m, m2, result);
	assert(!result.contains("Lucy"));   // Lucy in both maps, result should not include Lucy
	assert(!result.contains("Faith"));  // result should not include any keys from m2
	assert(result.contains("Bob")); // should contain keys from m1
	subtract(m, m, result);		// test subtract with same maps
	assert(result.empty());	// result should be empty

	// test aliasing with combine
	m2.update("Lucy", 2);
	assert(combine(m, m2, m));  // make sure function still works if third parameter = m
	assert(m.contains("Bob") && m.contains("Faith"));   // m must have data from both maps

	// test aliasing with subtract
	subtract(m, m2, m2);
	assert(!m2.contains("Faith"));  // m2 should not contain anything from original m2
	assert(!m2.contains("Lucy"));
}
