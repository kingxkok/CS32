#include "Sequence.h"

Sequence::Sequence(){
	m_size = 0;
}

bool Sequence::empty() const {
	return m_size == 0;
}

int Sequence::size() const {
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos > m_size || pos < 0) return false;
	if (m_size == DEFAULT_MAX_ITEMS) return false;
	
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
	//check how many to swap
	int max = this->m_size;
	if (other.m_size > max) max = other.m_size;
	//contents
	for (int i = 0; i < max; i++) {
		ItemType temp = this->m_items[i];
		this->m_items[i] = other.m_items[i];
		other.m_items[i] = temp;
	}
	//length
	int tempsize = this->m_size;
	this->m_size = other.m_size;
	other.m_size = tempsize;

}