////////////////////////////////////////////////////////////////////////////////
// MAP SOURCE FILE
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Map.h"

using namespace std;

//Constructs Map by setting head to nullpointer
Map::Map() { head = nullptr; m_size = 0;}
Map::~Map()
{
    Node* iter = head;
    while(iter != nullptr)
    {
        Node* toDel = iter;
        iter = iter->m_next;
        delete toDel;
    }
}
//Copy constructor
Map::Map(const Map& source)
{
    head = nullptr;
    m_size = 0;
    KeyType key;
    ValueType value;
    if (source.size() == 0)
        return;
    for(int i = source.size() - 1; i >= 0; i--)
    {
        source.get(i, key, value);
        this->insert(key, value);
    }
}
//Assignment
Map& Map::operator=(const Map& source)
{
    if (&source == this)
        return *this;
    //delete current map
    Node* iter = head;
    while(iter != nullptr)
    {
        Node* toDel = iter;
        iter = iter->m_next;
        delete toDel;
    }
    head = nullptr;
    m_size = 0;
    //refill Map
    KeyType key;
    ValueType value;
    if (source.size() == 0)
        return *this;
    for(int i = source.size() - 1; i >= 0; i--)
    {
        source.get(i, key, value);
        this->insert(key, value);
    }
    return *this;
}
//Returns true if map is empty
bool Map::empty() const
{
    if (m_size == 0)
        return true;
    else
        return false;
}
//Returns size;
int Map::size() const
{
    return m_size;
}
//inserts in the beginning
//TODO: SHOULD ONLY INSERT IF KEY DOES NOT EXIST ETC. Done?
bool Map::insert(const KeyType& key, const ValueType& value)
{
    //Checks if key already exists in the linked list
    if (contains(key) == true)
        return false;
    //Creates pointer newNode, pointing to a new node initialized to key and value
    Node* newNode = new Node;
    newNode->m_key = key;
    newNode->m_value = value;
    m_size++;
    //Node at start has nullptr behind and head in front
    newNode->m_prev = nullptr;
    newNode->m_next = head;
    //Node in front points to newNode
    if (size() > 1)
        head->m_prev = newNode;
    //Change head to the new node and increment size
    head = newNode;
    return true;
}
//Updates the value mapped to by key
bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* iter = head;
    while (iter != nullptr)
    {
        if (iter->m_key == key)
        {
            iter->m_value = value;
            return true;
        }
        iter = iter->m_next;
    }
    return false;
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
    while(iter != nullptr)
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
        {
            iter->m_next->m_prev = iter->m_prev;
            iter->m_prev->m_next = iter->m_next;
            delete iter;
            m_size--;
            return true;
        }
        else if (iter->m_key == key && iter->m_next == nullptr)//if Node to be deleted is at the end
        {
            iter->m_prev->m_next = nullptr;
            delete iter;
            m_size--;
            return true;
        }
        iter = iter->m_next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* iter = head;
    while (iter != nullptr)
    {
        if (iter->m_key == key)
        {
            value = iter->m_value;
            return true;
        }
        iter = iter->m_next;
    }
    return false;
}
bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 || i >= size())
        return false;
    Node* iter = head;
    for (int j = 0; j < i; j++)
        iter = iter->m_next;
    key = iter->m_key;
    value = iter->m_value;
    return true;
}
//checks if the map contains the key
bool Map::contains(const KeyType& key) const
{
    Node* iter = head;
    while (iter != nullptr)
    {
        if (iter->m_key == key)
            return true;
        iter = iter->m_next;
    }
    return false;
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

/*
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

void subtract(const Map& m1, const Map& m2, Map& result)
{

}
*/
