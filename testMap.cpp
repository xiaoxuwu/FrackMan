/* PLACE IN NODE STRUCT, AND MAKE SURE # OF CONSTRUCT == DESTRUCTOR
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
    b = b;
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
    cout << "Combine test 1" << endl;
    Map comb3;
    assert(combine(comb1, comb2, comb3) == true);
    for (int i = 0; i < comb3.size(); i++)
    {
        comb3.get(i, key, value);
        cout << i << ": "<< " KEY: " << key << " VALUE: " << value << endl;
    }
    cout << endl;
    assert(comb3.size() == 4);

    Map comb4;
    comb4.insert("Fred", 123);
    comb4.insert("Ethel", 456);
    comb4.insert("Lucy", 789);

    Map comb5;
    comb5.insert("Lucy", 654);
    comb5.insert("Ricky", 321);
    cout << "Combine test 2" << endl;
    Map comb6;
    assert(combine(comb4, comb5, comb6) == false);
    for (int i = 0; i < comb6.size(); i++)
    {
        comb6.get(i, key, value);
        cout << i << ": "<< " KEY: " << key << " VALUE: " << value << endl;
    }
    cout << endl;
    assert(comb6.size() == 3);
    cout << "Combine test 3" << endl;
    assert(combine(comb1, comb2, comb1) == true);
    for (int i = 0; i < comb1.size(); i++)
    {
        comb1.get(i, key, value);
        cout << i << ": "<< " KEY: " << key << " VALUE: " << value << endl;
    }

    cout << "\nSubtract test 1" << endl;
    Map comb7, comb8;
    subtract(comb1, comb3, comb7);
    assert(comb7.size() == 0);
    subtract(comb2, comb2, comb2);
    assert(comb2.size() == 0);
    subtract(comb4, comb2, comb8);
    for (int i = 0; i < comb8.size(); i++)
    {
        comb8.get(i, key, value);
        cout << i << ": "<< " KEY: " << key << " VALUE: " << value << endl;
    }
    cout << "\nSubtract test 2" << endl;
    subtract(comb4, comb5, comb8);
    assert(comb8.size() == 2);
    for (int i = 0; i < comb8.size(); i++)
    {
        comb8.get(i, key, value);
        cout << i << ": "<< " KEY: " << key << " VALUE: " << value << endl;
    }
    cout << endl;
    cout << "All tests passed!" << endl;
}
