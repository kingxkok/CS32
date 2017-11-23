#include "ScoreList.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() { //change to main
	ScoreList list;

	assert(list.add(100) == true);
	assert(list.add(0) == true);
	assert(list.add(17) == true);
	assert(list.add(42) == true);
	assert(list.add(73) == true);


	assert(list.add(101) == false);
	assert(list.add(-1) == false);

	assert(list.size() == 5);
	assert(list.maximum() == 100);
	assert(list.minimum() == 0);
	
	assert(list.add(0) == true);
	assert(list.add(0) == true);
	assert(list.remove(100) == true);
	assert(list.size() == 5 + 2 - 1);
	assert(list.remove(0) == true);
	assert(list.size() == 5);
	

	cout << "All tests passed." << endl;

	return 0;
}