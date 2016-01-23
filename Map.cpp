////////////////////////////////////////////////////////////////////////////////
// MAP SOURCE FILE
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Map.h"

using namespace std;

//Constructs Map by setting head to nullpointer
Map::Map() { head = nullptr; m_size = 0;}
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
bool Map::insert(const KeyType& key, const ValueType& value)
{
    Node* newNode = new Node;
    newNode->m_key = key;
    newNode->m_value = value;
    newNode->m_prev = nullptr;
    newNode->m_next = head;
    head = newNode;
    if (m_size == 0)
        newNode->m_next->m_prev = newNode;
    m_size++;
    return true;
}
