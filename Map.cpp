////////////////////////////////////////////////////////////////////////////////
// MAP SOURCE FILE
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Map.h"

using namespace std;

//Constructs Map by assigning private members
Map::Map() { head = nullptr; m_size = 0;}
//Destructor
Map::~Map()
{
    Node* iter = head;
    while(iter != nullptr) //loops through Map
    {
        Node* toDel = iter;
        iter = iter->m_next;
        delete toDel; //uses temp Node pointer to iterate and delete all the Nodes
    }
}
//Copy constructor
Map::Map(const Map& source)
{
    head = nullptr; //set the private members of map
    m_size = 0;
    //declare local variables
    KeyType key;
    ValueType value;
    if (source.size() == 0) //if source is empty, return
        return;
    for(int i = source.size() - 1; i >= 0; i--)
    {
        source.get(i, key, value);
        insert(key, value);
    }
}
//Assignment Function
Map& Map::operator=(const Map& source)
{
    if (&source == this) //check for aliasing
        return *this;
    //wipe current map
    Node* iter = head;
    while(iter != nullptr) //iterate through the map
    {
        Node* toDel = iter;
        iter = iter->m_next;
        delete toDel; //delete every node
    }
    head = nullptr; //reset private members of map
    m_size = 0;
    //refill Map with nodes from source, declare local variables
    KeyType key;
    ValueType value;
    if (source.size() == 0) //if source is empty, return current map
        return *this;
    for(int i = source.size() - 1; i >= 0; i--)
    {
        source.get(i, key, value);
        insert(key, value);
    }
    return *this;
}
//Empty Function
bool Map::empty() const
{
    if (m_size == 0) //Returns true if map is empty
        return true;
    else
        return false; //False otherwise
}
//Size Function
int Map::size() const
{
    return m_size; //Returns size;
}
//Insert Function (Implementation inserts at the beginning of Map)
bool Map::insert(const KeyType& key, const ValueType& value)
{
    //Checks if key already exists in the linked list
    if (contains(key) == true)
        return false;
    //Creates pointer newNode, pointing to a new node initialized to key and value, and increment size
    Node* newNode = new Node;
    newNode->m_key = key;
    newNode->m_value = value;
    m_size++;
    //Node at start has nullptr behind and head in front
    newNode->m_prev = nullptr;
    newNode->m_next = head;
    //Node in front points to newNode (only if there is more than one node)
    if (size() > 1)
        head->m_prev = newNode;
    //Change head to the new node
    head = newNode;
    return true;
}
//Updates the value mapped to by key
bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* iter = head;
    while (iter != nullptr) //Loops through Map
    {
        if (iter->m_key == key) //If key is found in a Node, set value to Node's value
        {
            iter->m_value = value;
            return true;
        }
        iter = iter->m_next;
    }
    return false;   //return false if the key is not of any Node
}
//Either inserts or updates
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (insert(key, value) == false) //If entry cannot be inserted, update it
        update(key, value);
    return true;
}
//Erases key
bool Map::erase(const KeyType& key)
{
    Node* iter = head;
    while(iter != nullptr) //Loops through the entire Map
    {
        if(iter->m_key == key && size() == 1) //if Node to be deleted is the only node
        {
            delete iter;
            head = nullptr;
            m_size--;
            return true;
        }
        else if(iter->m_key == key && iter == head) //if Node to be deleted is the first node
        {
            //set the next Node's previous pointer to nullpointer
            iter = iter->m_next;
            iter->m_prev = nullptr;
            //delete the current head, decrement size
            delete head;
            head = iter;
            m_size--;
            return true;
        }
        else if (iter->m_key == key && iter->m_next != nullptr)//if Node to be deleted is in the middle
        {   //exchange pointers of node ahead and before the node to be deleted
            iter->m_next->m_prev = iter->m_prev;
            iter->m_prev->m_next = iter->m_next;
            delete iter; //delete node, decrement size
            m_size--;
            return true;
        }
        else if (iter->m_key == key && iter->m_next == nullptr)//if Node to be deleted is at the end
        {
            iter->m_prev->m_next = nullptr; //sets the m_next of the node before last node to nullptr
            delete iter; //delete node. decrement size
            m_size--;
            return true;
        }
        iter = iter->m_next;
    }
    return false;   //return false if node cannot be found in the Map
}
//2 parameter get function
bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* iter = head;
    while (iter != nullptr)
    {   //loops until the node with key is found
        if (iter->m_key == key)
        {   //set value to value of node with same key
            value = iter->m_value;
            return true;
        }
        iter = iter->m_next;
    }
    return false;   //return false if node with key is not found
}
//3 parameter get function
bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 || i >= size()) //check for invalid i parameter
        return false;
    Node* iter = head;
    for (int j = 0; j < i; j++) //finds the i-th node
        iter = iter->m_next;
    //sets key and value to those of i-th node
    key = iter->m_key;
    value = iter->m_value;
    return true;
}
//checks if the map contains the key
bool Map::contains(const KeyType& key) const
{
    Node* iter = head;
    while (iter != nullptr) //loops through Map
    {
        if (iter->m_key == key) //If it finds the key, return true
            return true;
        iter = iter->m_next;
    }
    return false; //returns key if looped through map but did not find key
}
//Swap function
void Map::swap(Map& other)
{
    //Swap sizes
    int tempSize = size();
    m_size = other.size();
    other.m_size = tempSize;
    //Swap head pointers
    Node* tempHead = head;
    head = other.head;
    other.head = tempHead;
}


bool combine(const Map& m1, const Map& m2, Map& result)
{
    //Declare local variables
    bool toReturn = true;
    KeyType key;
    ValueType value;
    int i;
    Map resultCopy = result; //duplicate result to prevent errors from aliasing
    //delete everything from resultCopy (if it is not empty)
    for (i = 0; i < result.size(); i++)
    {
        result.get(i, key, value);
        resultCopy.erase(key);
    }
    //Insert all values from m1
    for (i = 0; i < m1.size(); i++)
    {
        m1.get(i, key ,value);
        resultCopy.insert(key, value);
    }
    //Insert all values from m2, and delete those found to be the same key, diff value
    for (i = 0; i < m2.size(); i++)
    {
        m2.get(i, key ,value);
        if(resultCopy.insert(key, value) == false)
        {
            ValueType valueM1;
            m1.get(key, valueM1); // get value from m1
            if (value != valueM1) // compare value from m1 and m2, and if diff, delete from resultCopy
            {
                resultCopy.erase(key);
                toReturn = false;
            }
        }
    }
    result.swap(resultCopy);
    return toReturn;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    KeyType key;
    ValueType value;
    int i;
    Map resultCopy = result; //duplicate result to prevent errors from aliasing
    //delete everything from resultCopy (if it is not empty)
    for (i = 0; i < result.size(); i++)
    {
        result.get(i, key, value);
        resultCopy.erase(key);
    }
    //Insert all values from m1
    for (i = 0; i < m1.size(); i++)
    {
        m1.get(i, key ,value);
        resultCopy.insert(key, value);
    }
    //Remove all values from resultCopy that are also found in m2
    for (i = 0; i < m2.size(); i++)
    {
        m2.get(i, key, value);
        if (m1.contains(key))
            resultCopy.erase(key);
    }
    result.swap(resultCopy);
}
