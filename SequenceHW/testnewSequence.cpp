#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() //change to main
{
	Sequence s;
	assert(s.empty());
	assert(s.find(42) == -1);
	assert(s.insert(42) == 0);
	assert(s.size() == 1 && s.find(42) == 0);
	assert(s.insert(24) == 0);
	assert(s.insert(52) == 2);
	assert(s.insert(52) == 2);

	assert(s.erase(0));
	assert(s.size() == 3);
	assert(s.remove(52));
	assert(s.size() == 1);

	Sequence s2(3);
	assert(s2.empty());
	assert(s2.find(42) == -1);
	assert(s2.insert(42) == 0);
	assert(s2.size() == 1 && s2.find(42) == 0);
	assert(s2.insert(24) == 0);
	assert(s2.insert(52) == 2);
	assert(s2.insert(52) == -1);

	assert(s2.erase(0));
	assert(s2.size() == 2);
	assert(s2.remove(52));
	assert(s2.size() == 1);

	Sequence a(1000);   // a can hold at most 1000 items
	Sequence b(5);      // b can hold at most 5 items
	Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
	ItemType v = 69;

	// No failures inserting 5 items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(k, v));

	// Failure if we try to insert a sixth item into b
	assert(!b.insert(5, v));

	// When two Sequences' contents are swapped, their capacities are
	// swapped as well:
	a.swap(b);
	assert(!a.insert(5, v));
	for (int k = 0; k < 1000; k++)
		assert(b.insert(k, v));


	cout << "Passed all tests" << endl;

	return 0;
}


/**
int mainForString()
{
	Sequence s;
	assert(s.empty());
	assert(s.find("laobing") == -1);
	s.insert("laobing");
	assert(s.size() == 1 && s.find("laobing") == 0);
	cout << "Passed all tests" << endl;
}
*/