// rats.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>


#include "globals.h"
#include "Rat.h"
#include "Arena.h"
#include "Game.h"
#include "Player.h"


using namespace std;



///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

//class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Rat declaration.





///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(20, 20, 50);

	// Play the game
	g.play();
}
