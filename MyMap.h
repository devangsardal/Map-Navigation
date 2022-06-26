#include <iostream>

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const { return m_size; }
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    
    struct Node//make the structure of each node to have a left and right node
    {
        Node* m_left = nullptr;
        Node* m_right = nullptr;
        KeyType m_key;
        ValueType m_value;
        ~Node()
        {
            if (m_left != nullptr)
                delete m_left;
            if (m_right != nullptr)
                delete m_right;
        }
    };
    int m_size;
    Node* m_head;
    Node* findNode(const KeyType &key) const//move through the tree to find the key being looked for
    {
        if (m_size > 0)
        {
            Node* temp = m_head;
            while (temp != nullptr)
            {
                if (temp->m_key == key)
                    return temp;
                if (temp->m_key < key)
                {
                    temp = temp->m_right;
                    continue;
                }
                if (temp->m_key > key)
                {
                    temp = temp->m_left;
                    continue;
                }
            }
        }
        return nullptr;
    }
};



template<typename KeyType, typename ValueType> MyMap<KeyType, ValueType>::MyMap()
{
    m_size = 0;
    m_head = nullptr;
}

template<typename KeyType, typename ValueType> MyMap<KeyType, ValueType>::~MyMap()
{
    delete m_head;
}

template<typename KeyType, typename ValueType> void MyMap<KeyType, ValueType>::clear()
{
    delete m_head;
    m_size = 0;
    m_head = nullptr;
}

template<typename KeyType, typename ValueType> const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
    Node* temp = findNode(key);//move through tree to find the node being looked for
    if (temp != nullptr)
        return &(temp->m_value);
    else
        return nullptr;
}

template<typename KeyType, typename ValueType> void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    if (m_size <= 0)//if empty tree at at root
    {
        m_head = new Node;
        m_head->m_key = key;
        m_head->m_value = value;
        m_size++;
        return;
    }
    
    Node* temp = findNode(key);//find the node with the key passed in
    if (temp != nullptr)
    {
        temp->m_value = value;
        return;
    }
    
    Node* a = m_head;
    for (int i = 1; i > 0;)//loop until finding the key and if not found then add a new node at the bottom of tree
    {
        if (a->m_key < key)
        {
            if (a->m_right == nullptr)
            {
                a->m_right = new Node;
                a->m_right->m_key = key;
                a->m_right->m_value = value;
                m_size++;
                return;
            }
            else
            {
                a = a->m_right;
                continue;
            }
        }
        if (a->m_key > key)
        {
            if (a->m_left == nullptr)
            {
                a->m_left = new Node;
                a->m_left->m_key = key;
                a->m_left->m_value = value;
                m_size++;
                return;
            }
            else
            {
                a = a->m_left;
                continue;
            }
        }
    }
    
}







