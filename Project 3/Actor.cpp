#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <cmath>
#include <random>

//////////////
//helper////
//////////
double randDouble(double min, double max)
{
	if (max < min)
		std::swap(max, min);
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::uniform_real_distribution<> distro(min, max);
	return distro(generator);
}
double randomAngle() {
	return randDouble(0, 6.28318530718);
}
double randomRadius() {
	return randDouble(0, 10);
}



void getFacedCoord(int& desiredX, int& desiredY, GraphObject::Direction dir) {
	switch (dir) {
	case GraphObject::right:
		++desiredX;
		break;
	case GraphObject::left:
		--desiredX;
		break;
	case GraphObject::up:
		++desiredY;
		break;
	case GraphObject::down:
		--desiredY;
		break;
	}
}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/////////
//TRAP//
////////
void Trap::doSomething() {  //for all things in same x,y, act on them
	auto ret = world()->allThingsAt(getX(), getY());
	for (auto it = ret.first; it != ret.second; ++it)
		actOn(it->second);
}

void WaterPool::actOn(Actor* a) {
	if (a->isBug()) {
		Bug* b = static_cast<Bug*>(a);
		if (b != nullptr)  //it's a bug!
			if (b->lastStunnedBy() != pair<int, int>(getX(), getY())) {
				b->waterStun(); //2tick stun by default	
				b->setLastStunnedBy(getX(), getY());
			}
	}
}


////////
//BUG//
///////

bool Bug::attemptMove(Direction dir) {
	int desiredX = getX();
	int desiredY = getY();
	getFacedCoord(desiredX, desiredY, dir);
	if (world()->hasPebble(desiredX, desiredY)) {
		blockedFromMoving();
		return false;
	}
	else {
		moveTo(desiredX, desiredY);
		m_lastStunnedBy.first = -1;
		m_lastStunnedBy.second = -1;
		return true;
	}
}

void Bug::die() {
	Actor::die();
	world()->addFood(getX(), getY(), 100);
}

void Bug::doSomething() {
	{
		EnergyHolder::doSomething();
		//this->loseEnergy(1) included--die included there too
		if (!alive()) return;
		if (canMove()) //not stunned
			doExtraStuff(); //step 4
		else m_stunTick--;
	}
}


//GrassHopper

GrassHopper::GrassHopper(int imageID, int startX, int startY, int hp, StudentWorld* world)
	: Bug(imageID, startX, startY, hp, world) { //CHANGE
	newDirection();
}
void GrassHopper::newDirection() {
	this->setDirection((Direction)randInt(1, 4));
	m_distanceLeft = randInt(2, 10);
}
void GrassHopper::blockedFromMoving() {
	m_distanceLeft = 0;
}
void GrassHopper::doExtraStuff() {
	if(this->doSpecificGHStuff()) return; //baby wud maybe transform, adult cud jump etc
	Food* food = nullptr;
	world()->thingAt(getX(), getY(), food);
	if (food != nullptr) //there's food 
	{
		this->gainEnergy(food->loseEnergy(200)); //energy<200 logic built in loseEnergy
		if (randInt(0, 1)) { //if wantToRest(=randInt(0,1)==50% chance)... stun()
			stun();
			return;
		}
	}
	if (m_distanceLeft <= 0) {
		newDirection();
	}
	this->attemptMove(getDirection());
	--m_distanceLeft;
	stun();
}


bool BabyGrassHopper::doSpecificGHStuff() {
	if (energy() >= 1600) {   //step 5
	 	world()->addAdultGrassHopper(getX(), getY()); 
		this->die();
		return true;
	}
	return false;
}

bool AdultGrassHopper::bite() {
	auto ret = world()->allThingsAt(getX(), getY()); //includes itself lul
	for (auto it = ret.first; it != ret.second; ++it) {
		if (it->second != this) {
			if (it->second->isBug()) {
				Bug* b = static_cast<Bug*>(it->second);
				if (b != nullptr) {
					b->loseEnergy(50);
					return true;
				}
			}
		}
	}
	return false;
}

bool AdultGrassHopper::doSpecificGHStuff() {
	int willBite = randInt(0, 2);
	if (willBite == 0) //want to bite
		if(bite()) return true;
	
	//didn't bite so maybe jump?
	int willJump = randInt(0, 9);
	if (willJump == 0) //wanna jump
	{	
			double r = randomRadius();
			double theta = randomAngle();
			int x = round(r*cos(theta));
			int y = round(r*sin(theta));
			int desiredX = getX() + x;
			int desiredY = getY() + y;

			int timesRan = 0;
			const int increment = 1;
			while (!(desiredX < VIEW_WIDTH && desiredX >= 0 && desiredY >= 0 && desiredY < VIEW_HEIGHT))
			{
				++timesRan;
				 theta+=increment;	//increase angle to find open spot
				 x = round(r*cos(theta));
				 y = round(r*sin(theta));
				 desiredX = getX() + x;
				 desiredY = getY() + y;

				 if (timesRan > 6.28/increment) break; //went full circle
			}
			if (desiredX < VIEW_WIDTH && desiredX >= 0 && desiredY >= 0 && desiredY < VIEW_HEIGHT) {
				if (!world()->hasPebble(desiredX, desiredY)) {
					moveTo(desiredX, desiredY);
					attemptMove(none); //reset stun from water 
					return true;
				}
			}
			//fails to find open square
	}
	//doesn't jump
	//proceed to general GrassHoppers doSomething
	return false; //indicate no specific move executed

}




//AntHill
void AntHill::doSomething() {
	EnergyHolder::doSomething(); //	this->loseEnergy(1);
	if (!alive()) return;
	Food* food = nullptr;
	world()->thingAt(getX(), getY(), food);
	if (food != nullptr) //there's food, eat
	{
		this->gainEnergy(food->loseEnergy(10000));
		return;
	}
	if (energy() >= 2000) {
		this->world()->addAnt(getX(), getY(), m_colony); //includes total number tracking 
		this->loseEnergy(1500);
	}

}

//////////////////////////////////////
///////////////ANT///////////////////
/////////////////////////////////////

//colony == IID_ANTHILLk where k is colony number, so
Ant::Ant(int colony, int startX, int startY, StudentWorld* world)
	:Bug(colony, startX, startY, 1500, world), m_colony(colony), m_food(0), m_iCounter(0), m_lastRandomNumber(0), m_wasBlocked(false), m_wasBit(false) {
	m_compiler = this->world()->getCompiler(colony);
}
void Ant::doExtraStuff() {
	Compiler::Command com;
	int comsExecuted = 0;

	while (comsExecuted < 10) {
		if (m_compiler->getCommand(m_iCounter, com)) {
			++m_iCounter; //todo remove? //should be fine since if m_iCounter should be manually changed, followCommand will do it
			//^must be before followCommand()
				//do commands
			if (followCommand(com)) return; //if followCommand changed external state of simulation, return
			++comsExecuted;
		}
		else { //smth went wrong with command fetching
			this->die();
			return;
		}
	}
}

void Ant::blockedFromMoving() { m_wasBlocked = true; }
bool Ant::followCommand(Compiler::Command com) {
	switch (com.opcode) {
	case Compiler::rotateClockwise:
		this->setDirection((Direction)(((getDirection() - 1) + 1) % 4 + 1)); //set to start at 0 then add then %4 to correct for overflow THEN fix startat0 back to startat1
		return true;
		break;
	case Compiler::rotateCounterClockwise:
		this->setDirection((Direction)(((getDirection() - 1) + 3) % 4 + 1)); //set to start at 0 then -1 (use +3 to avoid negatives) then %4 to correct for overflow THEN fix startat0 back to startat1
		return true;
		break;
	case Compiler::moveForward:
		if (attemptMove(this->getDirection())) { //includes blockedFromMoving()
			m_wasBlocked = false;
			m_wasBit = false;
		}
		return true; //moved so changed external state
		break;
	case Compiler::eatFood:
		if (m_food >= 100) {
			this->gainEnergy(100);
			m_food -= 100;
		}
		else {
			this->gainEnergy(m_food);
			m_food = 0;
		}
		return true;
		break;
	case Compiler::dropFood:
		if (m_food > 0) {
			world()->addFood(getX(), getY(), m_food);
			m_food = 0;
		}
		return true;
		break;
	case Compiler::bite: {
		Bug* b = findEnemy();
		if (b != nullptr)
			b->loseEnergy(15);
		return true;
		break;
	}
		break;
	case Compiler::pickupFood:
		world()->giveFoodTo(this);
		return true;
		break;
	case Compiler::emitPheromone:
		world()->addPheromone(getX(), getY(), m_colony );
		return true;
		break; 
	case Compiler::faceRandomDirection:
		this->setDirection((Direction)randInt(1, 4));
		return true;
		break;

	//doesn't change external
	case Compiler::generateRandomNumber:
		m_lastRandomNumber = randInt(0, stoi(com.operand1));
		return false; //didn't change external
		break;
	case Compiler::goto_command:
		m_iCounter = stoi(com.operand1);
		return false;
		break;

	case Compiler::if_command: {
		Compiler::Condition condition = (Compiler::Condition)stoi(com.operand1);
		switch (condition) {
		case Compiler::last_random_number_was_zero:
			if(m_lastRandomNumber == 0) m_iCounter = stoi(com.operand2);
			return false;
			break;
		case Compiler::i_am_carrying_food:
			if (m_food != 0) m_iCounter = stoi(com.operand2);
			return false;
			break;
		case Compiler::i_am_hungry:
			if (this->energy() <= 25) m_iCounter = stoi(com.operand2);
			return false;
			break;
		case Compiler::i_am_standing_with_an_enemy:
			if(findEnemy() != nullptr)  m_iCounter = stoi(com.operand2);
			return false;
			break;
		case Compiler::i_am_standing_on_food: {
			Food* food = world()->foodAt(getX(), getY());
			if (food != nullptr) m_iCounter = stoi(com.operand2);
			return false;
		}
			break;
		case Compiler::i_am_standing_on_my_anthill: {
			AntHill* ahp = world()->anthillAt(getX(), getY());
			if (ahp != nullptr && ahp->colony() == this->m_colony) m_iCounter = stoi(com.operand2);
			return false;
		}
			break;	
		case Compiler::i_smell_pheromone_in_front_of_me:{
			int desiredX = getX();
			int desiredY = getY();
			getFacedCoord(desiredX, desiredY, this->getDirection());
			if (world()->pheromoneAt(desiredX, desiredY, m_colony) != nullptr) {
				m_iCounter = stoi(com.operand2); 
			}
			return false;
		}
			break;
		case Compiler::i_smell_danger_in_front_of_me:
			
			if (hasDangerInFront()) { m_iCounter = stoi(com.operand2); 
			}
			return false;
			break;
		case Compiler::i_was_bit:
			if (m_wasBit) { m_iCounter = stoi(com.operand2); }
			return false;
			break;
		case Compiler::i_was_blocked_from_moving:
			if(m_wasBlocked) m_iCounter = stoi(com.operand2);
			return false;
			break;

		} //end of switch case with if commands
	}//end of if_command
	default:
		return false;
	}//end of swtich (com.opcode)
} //end of Ant::followCommand()


Bug* Ant::findEnemy() {
		auto ret = world()->allThingsAt(getX(), getY()); //includes itself lul
		for (auto it = ret.first; it != ret.second; ++it) {
			if (it->second->isBug()) {
					if(it->second->isAnt(m_colony))
						continue; //ant and same colony so move to next Actor*
					else
						return  static_cast<Bug*>(it->second);	
			}
		}
		return nullptr;
}

bool Ant::hasDangerInFront() {
	int desiredX = getX();
	int desiredY = getY();
	getFacedCoord(desiredX, desiredY, this->getDirection());
	auto ret = world()->allThingsAt(desiredX, desiredY); //includes itself lul
	for (auto it = ret.first; it != ret.second; ++it) {
		if (it->second->isTrap()) {  return true; }
		if (it->second->isBug()) {
			Actor* a = it->second;
			if (a->isAnt(m_colony)) {
				continue; //ant and same colony so move to next Actor*
			}
			else
				return true;	
		}
	}
	return false;
}