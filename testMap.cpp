/* PLACE IN NODE STRUCT
Node()
{
    std::cerr << "CONSTRUCTOR" << std::endl;
}
~Node()
{
    std::cerr << "DESTRUCTOR" << std::endl;
}
*/
#include <iostream>
#include "Map.h"
#include <cassert>

using namespace std;

bool combine(const Map& m1, const Map& m2, Map& result)
{
    bool toReturn = true;
    KeyType key;
    ValueType value;
    int i;
    //delete everything from result (if it is not empty)
    for (i = 0; i < result.size(); i++)
    {
        result.get(i, key, value);
        result.erase(key);
    }
    //Insert all values from m1
    for (i = 0; i < m1.size(); i++)
    {
        m1.get(i, key ,value);
        result.insert(key, value);
    }
    //Insert all values from m2, and delete those found to be the same key, diff value
    for (i = 0; i < m2.size(); i++)
    {
        m2.get(i, key ,value);
        if(result.insert(key, value) == false)
        {
            ValueType valueM1;
            m1.get(key, valueM1);
            if (value != valueM1)
            {
                result.erase(key);
                toReturn = false;
            }
        }
    }
    return toReturn;
}

int main()
{
    Map m;
    KeyType key;
    ValueType value;
    assert(m.empty() == true);
    m.insert("Hello", 1);
    assert(m.size() == 1);
    m.insert("Hi", 2);
    assert(m.size() == 2);
    assert(m.insert("Hi", 3) == false);
    assert(m.update("Hi", 2) == true);
    assert(m.contains("Hello") == true);
    assert(m.contains("hello") == false);
    assert(m.insertOrUpdate("hello", 1) == true);
    assert(m.get(0, key, value) == true && key == "hello" && value == 1);
    assert(m.get(1, key, value) == true && key == "Hi" && value == 2);
    assert(m.get(2, key, value) == true && key == "Hello" && value == 1);
    assert(m.get(3, key, value) == false);
    assert(m.get(-1, key, value) == false);
    assert(m.size() == 3);
    //Testing Copy Constructor
    Map a = m;
    assert(a.get(0, key, value) == true && key == "hello" && value == 1);
    assert(a.get(1, key, value) == true && key == "Hi" && value == 2);
    assert(a.get(2, key, value) == true && key == "Hello" && value == 1);
    assert(a.get(3, key, value) == false);
    assert(a.get(-1, key, value) == false);
    assert(a.size() == 3);
    //Testing Assignment
    Map b;
    b = m;
    b.swap(m);
    b.swap(m);
    assert(b.get(0, key, value) == true && key == "hello" && value == 1);
    assert(b.get(1, key, value) == true && key == "Hi" && value == 2);
    assert(b.get(2, key, value) == true && key == "Hello" && value == 1);
    assert(b.get(3, key, value) == false);
    assert(b.get(-1, key, value) == false);
    assert(b.size() == 3);

    //Testing Delete Functions
    assert(m.erase("Hello") == true);
    assert(m.contains("Hello") == false);
    assert(m.size() == 2);
    assert(m.erase("hello") == true);
    assert(m.contains("hello") == false);
    assert(m.size() == 1);
    assert(m.erase("Hi") == true);
    assert(m.contains("Hi") == false);
    assert(m.size() == 0);

    assert(a.erase("Hello") == true);
    assert(a.contains("Hello") == false);
    assert(a.size() == 2);
    assert(a.erase("hello") == true);
    assert(a.contains("hello") == false);
    assert(a.size() == 1);
    assert(a.erase("Hi") == true);
    assert(a.contains("Hi") == false);
    assert(a.size() == 0);

    Map c;
    assert(c.insert("lol", 1.01) == true);
    assert(c.insert("wat", 2.02) == true);
    assert(c.get(0, key, value) == true && key == "wat" && value == 2.02);
    assert(c.get(1, key, value) == true && key == "lol" && value == 1.01);

    c.swap(b);
    assert(b.get(0, key, value) == true && key == "wat" && value == 2.02);
    assert(b.get(1, key, value) == true && key == "lol" && value == 1.01);
    assert(c.get(0, key, value) == true && key == "hello" && value == 1);
    assert(c.get(1, key, value) == true && key == "Hi" && value == 2);
    assert(c.get(2, key, value) == true && key == "Hello" && value == 1);
    assert(c.get(3, key, value) == false);
    assert(c.get(-1, key, value) == false);
    assert(c.size() == 3);

    //Testing combine;
    Map comb1;
    comb1.insert("Fred", 123);
    comb1.insert("Ethel", 456);
    comb1.insert("Lucy", 789);

    Map comb2;
    comb2.insert("Lucy", 789);
    comb2.insert("Ricky", 321);

    Map comb3;
    assert(combine(comb1, comb2, comb3) == true);
    assert(comb3.get(0, key, value) == true && key == "Ricky" && value == 321);
    assert(comb3.get(1, key, value) == true && key == "Lucy" && value == 789);
    assert(comb3.get(2, key, value) == true && key == "Ethel" && value == 456);
    assert(comb3.get(3, key, value) == true && key == "Fred" && value == 123);
    assert(comb3.size() == 4);
    cout << "All tests passed!" << endl;
}
