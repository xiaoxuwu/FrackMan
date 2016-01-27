////////////////////////////////////////////////////////////////////////////////
// MAP HEADER FILE
////////////////////////////////////////////////////////////////////////////////

#ifndef MAP
#define MAP
#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
public:
    Map();
    ~Map();
    Map(const Map& source);
    Map& operator=(const Map& source);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* m_next;
        Node* m_prev;
    };
    Node* head;
    int m_size;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif
