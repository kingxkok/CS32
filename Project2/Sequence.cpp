#include "Sequence.h"


Sequence::Sequence() {
	this->head = new Node();
	head->m_next = head;
	head->m_prev = head;

}

int Sequence::size()  const {
	int count = 0; //track items visited
	for (Node* p = head; p->m_next != head; p = p->m_next, count++); //stops when next is head (end of list)
	return count;
}

bool Sequence::empty() const {
	return this->size() == 0;
}


bool Sequence::insert(int pos, const ItemType& value) {
	if (pos < 0) return false;
	int count = 0;
	Node* p;
	for (p = head->m_next; count!=pos && p != head; p = p->m_next, count++); //start at first (0th) meaningful item
		//stops when pos is reached OR end of list 
		//(check if at pos yet before checking end since if pos reached it doesn't matter if it's at the end)
	
	if (count == pos) { //if at correct pos
		Node* inserted = new Node(); //make new node and
		//put it in
		inserted->m_data = value;
		inserted->m_next = p; //p is moved forward so inserted's next is p
		inserted->m_prev = p->m_prev; //inserted's prev is p's original prev 
		//update p->m_prev and p
		inserted->m_prev->m_next = inserted;
		p->m_prev = inserted;
		return true;
	}

	return false;
}

int Sequence::insert(const ItemType& value) {
	int pos = 0;
	Node* p;
	for (p = head->m_next; value > p->m_data && p != head; p = p->m_next, pos++); //keep track of pos

	Node* inserted = new Node(); //make new node and
								 //put it in
	inserted->m_data = value;
	inserted->m_next = p; //p is moved forward so inserted's next is p
	inserted->m_prev = p->m_prev; //inserted's prev is p's original prev 
			//update p->m_prev and p
	inserted->m_prev->m_next = inserted;
	p->m_prev = inserted;

	return pos;

}

bool Sequence::erase(int pos) {
	if (pos < 0) return false;
	int count = 0;
	Node* p;
	for (p = head->m_next; p != head && count != pos ; p = p->m_next, count++); //start at first (0th) meaningful item
																	  //stops when pos is reached OR end of list 
																	//check end of list first since cannot erase item at pos == size()
	if (p != head) { //if not end of list
		p->m_next->m_prev = p->m_prev;
		p->m_prev->m_next = p->m_next;
		delete p;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const {
	int count = 0;
	Node* p;
	for (p = head->m_next; p != head && p->m_data != value; p = p->m_next, count++); //start at first (0th) meaningful item
																			   //stops when pos is reached OR end of list 
																			   //check end of list first since cannot return pos == size()
	if (p == head) return -1;
	return count;
}

int Sequence::remove(const ItemType& value) {
	//Find, then erase, keeping a counter of how many 
	int count = 0;
	while (erase(find(value)))   //auto false if find returns -1
		count++;
	return count;	
}

bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0) return false;
	int count = 0;
	Node* p;
	for (p = head->m_next; p != head && count != pos; p = p->m_next, count++); //start at first (0th) meaningful item
																			   //stops when pos is reached OR end of list 
																			   //check end of list first since cannot get item at pos == size()
	if (p == head) return false;
	value = p->m_data;
	return true;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0) return false;
	int count = 0;
	Node* p;
	for (p = head->m_next; p != head && count != pos; p = p->m_next, count++); //start at first (0th) meaningful item
																			   //stops when pos is reached OR end of list 
																			   //check end of list first since cannot set item at pos == size()
	if (p == head) return false;
	p->m_data = value;
	return true;
}

void Sequence::swap(Sequence& other) {
	//swap heads
	Node* temp = this->head;
	this->head = other.head;
	other.head = temp;
}

Sequence::~Sequence() {
	Node* current = head->m_next; //start at first interesting element
	Node* next = current->m_next;

	while (current != head) {
		delete current;
		current = next;
		next = current->m_next;
	}
	delete current; //delete head
}

Sequence::Sequence(const Sequence& other) {
	this->head = new Node();
	head->m_next = head;
	head->m_prev = head;

	Node* p;
	int pos = 0;
	for (p = other.head->m_next; p != other.head; p = p->m_next) { //start at first (0th) meaningful item
		this->insert(pos, p->m_data); //insert at end of list
		pos++;
	} //efficient? maybe not, but efficient for the code writer :D


}

Sequence& Sequence::operator=(Sequence rhs) {
	//pass by value and swap
	this->swap(rhs);
	return *this;
}



/////////////////////
//Non-member functions
//////////////////////

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	//record sizes
	const int seq1size = seq1.size();
	const int seq2size = seq2.size();
	//seq1 should be the bigger sequence, seq2 should have stuff in it
	if (seq1size < seq2size || seq2size == 0) return -1;

	ItemType item1;
	ItemType item2;
	for (int i = 0; i < seq1size; i++) {
		
		int j;
		for (j = 0; j < seq2size; j++) {
			if (i + j == seq1size) return -1; //seq1 is exhausted, so return -1
			seq1.get(i+j, item1); 
			seq2.get(j, item2);
			if (item1 != item2) break;
		}
		if (j == seq2size) return i; //i is index of outerloop (first matching entry)
		//^wil only execute if all item2s match the item1s
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	Sequence tempResult; //use temp and assign later on to prevent aliasing problems
	ItemType item1;
	ItemType item2;
	
	int insertAt = 0; //mark where to insert (the end)
	for (int pos = 0; seq1.get(pos, item1) || seq2.get(pos, item2); pos++) {
		if (seq1.get(pos, item1)) {
			tempResult.insert(insertAt, item1);
			insertAt++; //tempResults has grown
		}
		if (seq2.get(pos, item2)) { 
			tempResult.insert(insertAt, item2); 
			insertAt++;
		}
	}
	result = tempResult;
}