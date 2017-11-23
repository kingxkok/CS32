#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include <map>
#include <list>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


struct Coord {
	int x, y;
	Coord(int x, int y) : x(x), y(y) {}
	bool operator<(const Coord& coord) const {
		if (x < coord.x) return true;
		if (x > coord.x) return false;
		//x == coord.x
		if (y < coord.y) return true;
		if (y > coord.y) return false;
		//x == coord.x && y == coord.y
		return false;
	}
	bool operator==(const Coord& coord) const {
		if (x == coord.x && y == coord.y)
			return true;
		return false;
	}
};


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), m_numCompilers(0)
	{
	}

	~StudentWorld() {
		cleanUp();
	}

	virtual int init();
	virtual int move();
	virtual void cleanUp()
	{
		for (auto it = m_actors.begin(); it != m_actors.end(); ) {
			delete it->second; //maybe todo	
			it = m_actors.erase(it);
		}
		for (int i = 0; i < m_numCompilers; ++i) {
			Compiler*& compiler = compilers[i];
			--m_numCompilers;
				delete compiler;
				compiler = nullptr;
			
		}
	}
	int getCurrentTicks() {
		return m_ticks;
	}

	bool hasPebble(int x, int y) const {
		auto it = m_actors.find(Coord(x, y));
		if (it == m_actors.end()) return false;
		Actor* ap = it->second;
		return ap->blocksAnt();
	}

	void addFood(int x, int y, int amount);
	void addPheromone(int x, int y, int colony);
	void addAdultGrassHopper(int x, int y);
	void addAnt(int x, int y, int colony);
	void giveFoodTo(Ant* ant);

	Food* foodAt(int x, int y);
	AntHill* anthillAt(int x, int y);
	template<typename T>
	void thingAt(int x, int y, T & t);
	Pheromone* pheromoneAt(int x, int y, int colony);
	void pheromoneAt(int x, int y, int colony, Pheromone*& p);
	auto allThingsAt(int x, int y) { //returns a pair of iterators to (Coord,Actor*) pairs
		return m_actors.equal_range(Coord(x, y)); //pair indicating start n end of Actors with given x&y
	}
	Compiler* getCompiler(int colony) const;

private:

	multimap<Coord, Actor*> m_actors;
	int m_ticks;
	Compiler* compilers[4];
	int m_numCompilers;
	int m_totalAntsMade[4];
	int m_winningAnt;
	int m_highestCount;
};

#endif // STUDENTWORLD_H_
