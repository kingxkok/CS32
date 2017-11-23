// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef MYMAP
#define MYMAP

#include <iostream>

using namespace std;

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap(): root(nullptr), m_size(0) {};
	~MyMap() { delete root;  }; //root will delete all its children too  ... gory :c
	void clear() { delete root; root = nullptr; };
	int size() const {
		return m_size;
	}

	void associate(const KeyType& key, const ValueType& value) {
		if (root == nullptr) { //empty Map
			root = new MyMap::Node(key, value);
			++m_size;
		}
		else if (root->add(key, value)) 
			++m_size;

	};

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const {
		if(root != nullptr)
			return root->find(key);
		else return nullptr;
	}

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node{
		Node(KeyType key, ValueType value): first(key), second(value), left(nullptr), right(nullptr){}
		~Node() { delete left; delete right; } //kills its branches/leaves

		bool add(KeyType key, ValueType value) {
			if (key == first) { //found matching key; replace value
				second = value;
				return false;
			}
			Node*& added = (key > first) ? right : left; //which branch?
			if (added == nullptr) { //key not in map yet, so add new
				added = new Node(key, value);
				return true;
			}
			else return added->add(key, value);
		};
		const ValueType* find(const KeyType& key) const{
			if (key == first) return &second;
			else if (key > first) {
				if (right == nullptr) return nullptr;
				else return right->find(key);
			}
			else
				if (left == nullptr) return nullptr;
				else return left->find(key);
		}


		KeyType first;
		ValueType second;
		Node* left;
		Node* right;
	};
	
	Node* root;
	size_t m_size;
};




#endif // !MYMAP
