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