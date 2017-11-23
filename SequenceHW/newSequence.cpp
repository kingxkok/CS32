#include "newSequence.h"
#include <stdlib.h>



Sequence::Sequence(int maxSize) {
	if (maxSize < 0) exit(1);
	m_size = 0;
	m_MAX_ITEMS = maxSize;
	m_items = new ItemType[m_MAX_ITEMS];
}

Sequence::~Sequence() {
	delete[] this->m_items;
}

Sequence::Sequence(const Sequence& other) {
	
	this->m_MAX_ITEMS = other.m_MAX_ITEMS;
	this->m_size = other.m_size;
	m_items = new ItemType[m_MAX_ITEMS];

	for (int i = 0; i < m_size; i++) {
		this->m_items[i] = other.m_items[i];
	}
}

Sequence& Sequence::operator=(Sequence rhs) {
	this->swap(rhs);
	return *this;
}

bool Sequence::empty() const {
	return m_size == 0;
}

int Sequence::size() const {
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos > m_size || pos < 0) return false;
	if (m_size == m_MAX_ITEMS) return false;
	
	//valid
	for (int n = m_size; n > pos; n--)
		m_items[n] = m_items[n - 1];
	m_items[pos] = value;
	m_size++;

	return true;
}

int Sequence::insert(const ItemType& value) {
	int p = 0;
	while (p<m_size && value > m_items[p]) p++;
	if (insert(p, value)) return p;
	return -1;
}

bool Sequence::erase(int pos) {
	if (0 <= pos && pos < m_size) {
		for (int n = pos; pos < m_size - 1; pos++)
			m_items[pos] = m_items[pos + 1];
		m_size--;
		return true;
	}
	return false;
}

int Sequence::remove(const ItemType& value) {
	//Find, then erase, keeping a counter of how many 
	int count = 0; 
	while (erase(find(value)))   //auto false if find returns -1
		count++;
	return count;
}

bool Sequence::get(int pos, ItemType& value) const {
	if (0 <= pos && pos < m_size) {
		value = m_items[pos];
		return true;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (0 <= pos && pos < m_size) {
		m_items[pos] = value;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const {
	int p;
	for (p = 0; p < m_size; p++) {
		if (value == m_items[p]) return p;
	}
	p = -1;
	return p;
}

void Sequence::swap(Sequence& other) {
	
	//length
	int tempsize = this->m_size;
	this->m_size = other.m_size;
	other.m_size = tempsize;
	//max
	int tempMax = this->m_MAX_ITEMS;
	this->m_MAX_ITEMS = other.m_MAX_ITEMS;
	other.m_MAX_ITEMS = tempMax;
	//pointer
	ItemType* tempP = this->m_items;
	this->m_items = other.m_items;
	other.m_items = tempP;

}