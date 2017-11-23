#include "StudentWorld.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

 int StudentWorld::init()
{
	 Field f;
	 string fieldFile = getFieldFilename();
	 string error;
	 if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	 {
		 setError(fieldFile + " " + error);
		 return GWSTATUS_LEVEL_ERROR; // something bad happened!
	 }
	 
	 //set ticks to 0
	 m_ticks = 0;
	 for (int i = 0; i < 4; i++)
		 m_totalAntsMade[i] = 0;
	 m_winningAnt = -1;
	 m_highestCount = 0;

	 //compiler
	 Compiler* compilerForEntrant[4] = { nullptr, nullptr, nullptr, nullptr };
	 std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
	 for (int i = 0; i < fileNames.size(); i++) {
		 compilerForEntrant[i] = new Compiler;
		 if (!compilerForEntrant[i]->compile(fileNames[i], error))
		 {
			 // entrant 0’s source code had a syntax error!
			 // send this error to our framework to warn the user.
			 // do it JUST like this!
			 setError(fileNames[i] + " " + error);
			 // return an error to tell our simulation framework
			 // that something went wrong, and it’ll inform the user
			 return GWSTATUS_LEVEL_ERROR;
		 }
		 compilers[i] = compilerForEntrant[i];
		 ++m_numCompilers;
	 }

	 //rest
	 for(int x = 0; x<VIEW_WIDTH; x++)
		 for (int y = 0; y < VIEW_HEIGHT; y++) {
			 Actor* ap = nullptr;

			 switch (f.getContentsOf(x, y)) { //prob may be here
			 case f.empty:
				 break;
			 case f.rock:
				 ap = new Pebble(x, y, this);
				 break;
			 case f.grasshopper:
				 ap = new BabyGrassHopper(x, y, this);
				 break;
			 case f.food:
				 ap = new Food(x, y, this);
				 break;
			 case f.water:
				 ap = new WaterPool(x, y, this);
				 break;
			 case f.poison:
				 ap = new Poison(x, y, this);
				 break;

			 case f.anthill0: 
			 {
				 Compiler* c = compilerForEntrant[0];
				 if (c != nullptr)
					 ap = new AntHill(x, y, this, 0, c);
				 break;
			 }
			 case f.anthill1:
			 {
				 Compiler* c = compilerForEntrant[1];
				 if (c != nullptr)
					 ap = new AntHill(x, y, this, 1, c);
				 break;
			 }
			 case f.anthill2:
			 {
				 Compiler* c = compilerForEntrant[2];
				 if (c != nullptr)
					 ap = new AntHill(x, y, this, 2, c);
				 break;
			 }
			 case f.anthill3:
			 {
				 Compiler* c = compilerForEntrant[3];
				 if (c != nullptr)
					 ap = new AntHill(x, y, this, 3, c);
				 break;
			 }

			 }
			 if (ap != nullptr) {
			 m_actors.insert(pair<Coord, Actor*>(Coord(x, y), ap) );
			 }
			 

			 
		 }

	 //display all
//	 for (auto it : m_actors) it.second->setVisible(true);

	return GWSTATUS_CONTINUE_GAME;
}

  int StudentWorld::move()
 {
	  
	  ++m_ticks;

	  for(auto it : m_actors)
		  it.second->doSomething();


	  for (auto it = m_actors.begin(); it != m_actors.end(); ) {
		 
		  //check if dead
		  if (it->second->alive()) {

			  //check if moved
			  int x = it->second->getX(); //true x
			  int y = it->second->getY(); //true y
			  if (x != it->first.x || y != it->first.y) {
				  Actor* ap = it->second;
				  m_actors.insert(pair<Coord, Actor*>(Coord(x, y), ap));
				  it = m_actors.erase(it);
			  }
			  else ++it;
		  }
		  else //it's dead
		  {
			  delete it->second;
			  it = m_actors.erase(it);
		  }
	  }

	  //text update
	  ostringstream oss;
	  oss << "Ticks:" << std::setw(5) << (2000 - getCurrentTicks());
	  oss << " - ";
	  for (int i = 0; i < m_numCompilers; i++) {
		  oss << compilers[i]->getColonyName();
		  if (i == m_winningAnt) oss << "*";
  		  oss << ": ";
		  oss.fill('0');
		  oss << setw(2) << m_totalAntsMade[i];
		  oss << "  ";
	  }
	  //truncate last two spaces
	  string displayed = oss.str();
	  displayed = displayed.substr(0, displayed.length() - 2);
	  this->setGameStatText(displayed);


	 // This code is here merely to allow the game to build, run, and terminate after you hit enter.
	 // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
	
	  if (this->getCurrentTicks() == 2000)
		  if (m_winningAnt == -1)
			  return GWSTATUS_NO_WINNER;
		  else {
			  this->setWinner(compilers[m_winningAnt]->getColonyName()  );
			  return GWSTATUS_PLAYER_WON;
		  }
	  return GWSTATUS_CONTINUE_GAME;
 }






  template <typename T>
	void  StudentWorld::thingAt(int x, int y, T& t) { //never called ... here to look cool :)
	 auto ret = this->allThingsAt(x, y);
	 for (auto it = ret.first; it != ret.second; ++it) {
		  Actor* a = it->second;
		  T thing = dynamic_cast<T>(a);
		  if (thing != nullptr) {
			  t = thing;
			  break;
		  }
	  }
	 
  };
	template <>
	void StudentWorld::thingAt(int x, int y, Food*& food) {
		auto ret = this->allThingsAt(x, y);
		for (auto it = ret.first; it != ret.second; ++it) {
			Actor* a = it->second;
			if (a->isFood()) {
				food = static_cast<Food*>(a);
				return;
			}	
		}
	}
	template <>
	void StudentWorld::thingAt(int x, int y, AntHill*& ahp) {
		auto ret = this->allThingsAt(x, y);
		for (auto it = ret.first; it != ret.second; ++it) {
			Actor* a = it->second;
			if (a->isAntHill()) {
				ahp = static_cast<AntHill*>(a);
				return;
			}
		}
	}

	void StudentWorld::pheromoneAt(int x, int y, int colony, Pheromone*& p) {
		p = pheromoneAt(x, y, colony);
	}

	Pheromone* StudentWorld::pheromoneAt(int x, int y, int colony) {
		auto ret = this->allThingsAt(x, y);
		for (auto it = ret.first; it != ret.second; ++it) {
			Actor* a = it->second;
			if(a->isPheromone(colony)) return static_cast<Pheromone*>(a);
			/*Pheromone* p = static_cast<Pheromone*>(a);
			if (p != nullptr) {
				if (p->colony() == colony) {
					return p;
				}
			} */
		}
		return nullptr;
	}
	
	Food* StudentWorld::foodAt(int x, int y) {
		Food* food = nullptr;
		thingAt(x, y, food); //put the food there into 'food'
		return food;
	}
	AntHill* StudentWorld::anthillAt(int x, int y) {
		AntHill* ahp = nullptr;
		thingAt(x, y, ahp);
		return ahp;
	}

	void StudentWorld::addFood(int x, int y, int amount) {
		Food* food = foodAt(x, y);
		if ((food != nullptr)) { //hasFood
			food->gainEnergy(amount);
			return;
		}
		//else
		Food* newFood = new Food(x, y, this, amount);
		m_actors.insert(pair<Coord, Actor*>(Coord(x, y), newFood));
	}


	void StudentWorld::addPheromone(int x, int y, int colony) { 
		Pheromone* pheromone = nullptr;
		pheromoneAt(x, y, colony, pheromone); //put the pheromone of same colony there into 'pheromone'

		if ((pheromone != nullptr)) { //hasPheromone of same colony
			pheromone->gainEnergy();
			return;
		}
		//else
		Pheromone* newPheromone = new Pheromone(colony+IID_PHEROMONE_TYPE0, x, y, this);
		m_actors.insert(pair<Coord, Actor*>(Coord(x, y), newPheromone));
	}

	void StudentWorld::addAdultGrassHopper(int x, int y) {
		AdultGrassHopper* aghp = new AdultGrassHopper(x, y, this);
		m_actors.insert(pair<Coord, Actor*>(Coord(x, y), aghp));
	}

	void  StudentWorld::addAnt(int x, int y, int colony) {
		Ant* ant = new Ant(colony, x, y, this);
		m_actors.insert(pair<Coord, Actor*>(Coord(x, y), ant));
		++m_totalAntsMade[colony];
		if (m_totalAntsMade[colony] > m_highestCount) {
			m_highestCount = m_totalAntsMade[colony];
			if(m_highestCount > 5)
				m_winningAnt = colony;
		}
	}

	void StudentWorld::giveFoodTo(Ant* ant) {
		Food* food = nullptr;
		thingAt(ant->getX(), ant->getY(), food); //put the food there into 'food'
		if ((food != nullptr)) { //hasFood
			if (food->energy() >= 400)
			{
				food->loseEnergy(ant->pickupFood(400));
			}
			else food->loseEnergy(ant->pickupFood(food->energy()));
		}
	}
	

	Compiler* StudentWorld::getCompiler(int colony) const {
		return compilers[colony];
	}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
