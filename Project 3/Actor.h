#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
#include <iostream>
using namespace std;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//Actor
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, int depth)
		: GraphObject(imageID, startX, startY, right, depth), m_alive(true), m_world(world) {};

	bool alive() {
		return m_alive;
	};
	virtual void doSomething() = 0;
	virtual void getPoisoned() {};

	virtual bool blocksAnt() { return false; };
	virtual bool isTrap() { return false; }
	virtual bool isFood() { return false; }
	virtual bool isBug() { return false; }
	virtual bool isPheromone(int colony) { return false; }
	virtual bool isAnt(int colony) { return false; }
	virtual bool isAntHill() { return false; } //can only have 1 anthill in one x,y

protected:
	virtual void die() {  m_alive = false; };
	StudentWorld* world() { return m_world; }
	
private:
	bool m_alive;
	StudentWorld* m_world;

};

///////////////
//inanimate///
//////////////

class Pebble : public Actor {
public:
	Pebble(int startX, int startY, StudentWorld* world)
		: Actor(IID_ROCK, startX, startY, world, 1) {} //depth == 1
	void doSomething() {};
	bool blocksAnt() { return true; }
};

//traps (water n poison)
class Trap : public Actor {
public:
	Trap(int imageID, int startX, int startY, StudentWorld* world)
		: Actor(imageID, startX, startY, world, 2) {};
	virtual void actOn(Actor* a) = 0;
	void doSomething(); //for all things in same (x,y) act on them
	bool isTrap() { return true; }
	
};
class WaterPool : public Trap {
public:
	WaterPool(int startX, int startY, StudentWorld* world)
		: Trap(IID_WATER_POOL, startX, startY, world) {};
	void actOn(Actor* a);
};
class Poison : public Trap {
public:
	Poison(int startX, int startY, StudentWorld* world)
		: Trap(IID_POISON, startX, startY, world) {};
	void actOn(Actor* a) { a->getPoisoned(); }
};




/////////////////////
//ENERGY HOLDERS!//
/////////////////

class EnergyHolder : public Actor {
public: 
	EnergyHolder(int imageID, int startX, int startY, StudentWorld* world, int depth, int startAmount)
	: Actor(imageID, startX, startY, world, depth), m_amount(startAmount)	{};
	virtual void gainEnergy(int amount) { m_amount += amount; };
	virtual int loseEnergy(int amount) {
		if (amount < m_amount) {
			m_amount -= amount;
			return amount;
		} //else amount lost >= m_amount
		this->die();
		int prevm_amount = m_amount;
		m_amount = 0; //eaten up; prevents being eaten again!
		return prevm_amount; //can only give m_amount before eaten
	}
	int energy() {
		return m_amount;
	}
	virtual void doSomething() { loseEnergy(1); };

private:
	int m_amount;

};

class Food : public EnergyHolder {
public:
	Food(int startX, int startY, StudentWorld* world, int startAmount = 6000)
		:EnergyHolder(IID_FOOD, startX, startY, world, 2, startAmount) {};
	virtual void doSomething() {}; //overrides -1 each tick
	bool isFood() { return true; }
};

class Pheromone : public EnergyHolder {
public:
	Pheromone(int imageID, int startX, int startY, StudentWorld* world)
		:EnergyHolder(imageID, startX, startY, world, 2, 256), m_colony(imageID-IID_PHEROMONE_TYPE0) {};
	 void gainEnergy() { 
		EnergyHolder::gainEnergy(256);
		if (energy() > 768) loseEnergy(energy() - 768);
	}
	int colony() { return m_colony; }
	bool isPheromone(int colony) { return colony == m_colony; }
private:
	int m_colony;


};

class AntHill : public EnergyHolder {
public:
	AntHill(int startX, int startY, StudentWorld* world, int colony, Compiler* compiler)
		: EnergyHolder(IID_ANT_HILL, startX, startY, world, 2, 8999), m_compiler(compiler), m_colony(colony) {}; 
	void doSomething();
	int colony() { return m_colony; }
	bool isAntHill() { return true; }
private:
	Compiler* m_compiler;
	int m_colony;

};


//////////
//BUGS!//
////////

class Bug : public EnergyHolder {
public:
	Bug(int imageID, int startX, int startY, int hp, StudentWorld* world)
		: EnergyHolder(imageID, startX, startY, world, 1, hp), m_stunTick(0), m_lastStunnedBy(-1,-1) //, m_hitPoints(hp) 
	{
		Direction dir = (Direction)randInt(1, 4);
		this->setDirection(dir);
	}

	virtual void doSomething();
	bool attemptMove(Direction dir);
	void die();
	virtual void getPoisoned() { loseEnergy(150); }

	//2 tick stun
	void stun() { ++m_stunTick; ++m_stunTick; }
	//void stun(int ticks) { m_stunTick += ticks; };
	virtual void waterStun() { stun(); }
	auto lastStunnedBy() { return m_lastStunnedBy; }
	void setLastStunnedBy(int x, int y) { m_lastStunnedBy.first = x; m_lastStunnedBy.second = y; }
	bool isBug() { return true; }

protected:
	virtual void blockedFromMoving() = 0;

private:
	bool canMove() { return m_stunTick == 0; }
	int m_stunTick;
	//int m_hitPoints;
	virtual void doExtraStuff() = 0;
	pair<int, int> m_lastStunnedBy;
};



class GrassHopper : public Bug {
public:
	GrassHopper(int imageID, int startX, int startY, int hp, StudentWorld* world);
	void newDirection();
protected:
	void blockedFromMoving();
	virtual bool doSpecificGHStuff() = 0;
private:
	virtual void doExtraStuff(); //gh's specific dosomethings (eat and walk)
	int m_distanceLeft;
};

class BabyGrassHopper : public GrassHopper {
public:
	BabyGrassHopper(int startX, int startY, StudentWorld* world)
		: GrassHopper(IID_BABY_GRASSHOPPER, startX, startY, 500, world) {}
protected:
	virtual bool doSpecificGHStuff(); //babyg's specific dosomethings
};
class AdultGrassHopper : public GrassHopper {
public:
	AdultGrassHopper(int startX, int startY, StudentWorld* world)
		: GrassHopper(IID_ADULT_GRASSHOPPER, startX, startY, 1600, world) {}
	void getPoisoned() {}
	void waterStun() {}
protected:
	int loseEnergy(int amount) {
		EnergyHolder::loseEnergy(amount);
		if (amount > 1 && amount < 100) {
			if(randInt(0,1)) //50% chance
				this->bite();
		}
		return 0; //can't be eaten directly
	}
	virtual bool doSpecificGHStuff();
private:
	bool bite();
};


/////////////////////////////////////////
///////		ANT			/////////////////
/////////////////////////////////////////


class Ant : public Bug {
public:
	Ant(int colony, int startX, int startY, StudentWorld* world);
	
	 //returns how much picked up
	 int pickupFood(int amount) { 
		 m_food += amount;
		 if (m_food > 1800) {
			 int excess = m_food - 1800;
			 m_food = 1800;
			 return amount - excess;
		 }
		 return amount;
	 }
	 bool isAnt(int colony) { return m_colony == colony; }
protected:
	void blockedFromMoving();
	 int loseEnergy(int amount) {
		 EnergyHolder::loseEnergy(amount);
		 if (amount > 1 && amount <100) m_wasBit = true;
		 return 0; //can't be eaten directly
	 }

private:
	 void doExtraStuff();
	 bool followCommand(Compiler::Command com);
	 Bug* findEnemy();
	 bool hasDangerInFront();

	int m_food;
	Compiler* m_compiler;
	int m_colony;
	int m_iCounter;
	int m_lastRandomNumber;
	bool m_wasBlocked;
	bool m_wasBit;
};





#endif // ACTOR_H_
