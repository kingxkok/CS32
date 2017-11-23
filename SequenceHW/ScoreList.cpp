#include "ScoreList.h"

ScoreList::ScoreList() {

}

bool ScoreList::add(unsigned long score){
	if(score >= 0 && score <= 100)
		return m_sequence.insert(m_sequence.size(), score);
	return false;
}

bool ScoreList::remove(unsigned long score) {
	return m_sequence.erase( m_sequence.find(score) );
}

int ScoreList::size() const {
	return m_sequence.size();
}

unsigned long ScoreList::minimum() const {
	if (size() == 0) return NO_SCORE;
	//returned val
	unsigned long min; 
	m_sequence.get(0, min);
	//to store each val
	unsigned long temp;
	for (int i = 1; i < size(); i++) {
		m_sequence.get(i, temp);
		if (temp < min) min = temp;
	}
	return min;
}

unsigned long ScoreList::maximum() const {
	if (size() == 0) return NO_SCORE;
	//returned val
	unsigned long max;
	m_sequence.get(0, max);
	//to store each val
	unsigned long temp;
	for (int i = 1; i < size(); i++) {
		m_sequence.get(i, temp);
		if (temp > max) max = temp;
	}
	return max;
}

