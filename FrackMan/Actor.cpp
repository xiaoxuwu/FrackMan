#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <algorithm>

using namespace std;

//////////////
//Actor Class
//////////////

Actor::Actor(StudentWorld* worldPtr, int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, bool visible = true)
	: m_world(worldPtr), GraphObject(imageID, startX, startY, startDirection, size, depth), m_isAlive(true)
{
	setVisible(visible);
}

void Actor::kill() { m_isAlive = false; }

/////////////////
// Dirt Class
/////////////////

Dirt::Dirt(StudentWorld* worldPtr, int startX, int startY)
	: Actor(worldPtr, IID_DIRT, startX, startY, right, 0.25, 3) //Dirt is initialized to StartX, StartY, facing right, with size 0.25 and depth 3
{} //Dirt is set to visible once created


void Dirt::doSomething() { return; } //Empty do something function for dirt because it doesn't do anything :P

/////////////////
// Boulder Class
/////////////////

Boulder::Boulder(StudentWorld* worldPtr, int x, int y)
	: Actor(worldPtr, IID_BOULDER, x, y, down, 1.0, 1), m_isFalling(false), m_ticksWaited(0)
{
	for (int delY = y; delY < y + 4; delY++)
		for (int delX = x; delX < x + 4; delX++)
			getWorld()->removeDirt(delX, delY);
}

void Boulder::doSomething()
{
	if (!isAlive()) //check to see if alive
		return;
	//increment ticks and set state to waiting state for 30 ticks, if it can move
	if (getWorld()->canMove(this, down))
	{
		m_ticksWaited++;
		//If below tests pass, then boulder is allowed to fall
		if (m_isFalling == false && m_ticksWaited == 31)
		{
			getWorld()->playSound(SOUND_FALLING_ROCK);
			m_isFalling = true;
		}
		if (m_isFalling == true) //if in falling state, fall down
		{
			moveTo(getX(), getY() - 1);
			if (getWorld()->isDistAway(3, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY()))
				getWorld()->attackFrack(10); //attack frackman if within 3 units away
			getWorld()->annoyAllNearbyActors(this, 2, 3);
		}
		return;
	}
	if (waitTime() > 0)
		kill();
}

/////////////////
// Squirt Class
/////////////////

Squirt::Squirt(StudentWorld* worldPtr, int x, int y, Direction startDirection)
	: Actor(worldPtr, IID_WATER_SPURT, x, y, startDirection, 1.0, 1), m_nMovesLeft(4) {}

void Squirt::doSomething() //TODO: kill protestors
{
	if (getMovesLeft() == 0)
		kill();
	if (!isAlive())
		return;
	switch (getDirection())
	{
	case up:
	{
		if (getWorld()->canMove(this, up))
		{
			moveTo(getX(), getY() + 1);
			m_nMovesLeft--;
		}
		else
			kill();
		break;
	}
	case down:
	{
		if (getWorld()->canMove(this, down))
		{
			moveTo(getX(), getY() - 1);
			m_nMovesLeft--;
		}
		else
			kill();
		break;
	}
	case left:
	{
		if (getWorld()->canMove(this, left))
		{
			moveTo(getX() - 1, getY());
			m_nMovesLeft--;
		}
		else
			kill();
		break;
	}
	case right:
	{
		if (getWorld()->canMove(this, right))
		{
			moveTo(getX() + 1, getY());
			m_nMovesLeft--;
		}
		else
			kill();
		break;
	}
	}
	//annoy all protesters around it
	if (getWorld()->findNearbyPickerUpper(this, 3) != nullptr)
	{
		getWorld()->annoyAllNearbyActors(this, 1, 3);
		kill();
	}
}

/////////////////
// People Class
/////////////////

People::People(int spriteID, StudentWorld* worldPtr, int x, int y, int health, Direction dir = right)
	: Actor(worldPtr, spriteID, x, y, dir, 1.0, 0), m_health(health) {}

int People::getHealth() { return m_health; }

void People::getHit(int health)
{
	if (m_health - health < 0)
	{
		m_health = 0;
		return;
	}
	m_health -= health;
}

///////////////////
// Protestors class
///////////////////

Protesters::Protesters(StudentWorld* worldPtr, int startX, int startY, int imageID, int health)
	: People(imageID, worldPtr, startX, startY, health, left), m_leavingField(false), m_nTurnTicks(200), m_nTicksToRest(2)
{
	m_nStepsLeft = rand() % 53 + 8;
}

void Protesters::setTicksToNextMove()
{
	int formula = 100 - getWorld()->getLevel() * 10;
	int N = max(50, formula);
	m_nTicksToRest = N;
}

/////////////////////
// Regular Protester
/////////////////////

RegularProtester::RegularProtester(StudentWorld* worldPtr, int startX, int startY)
	: Protesters(worldPtr, startX, startY, IID_PROTESTER, 5) {}

void RegularProtester::doSomething()
{
	if (!isAlive())
		return;
	if (isLeavingField() && getX() == 60 && getY() == 60)
		kill();
	//Below executes if protester is trying to leave the field
	if (isLeavingField()) //TODO: insert code here for when protester leaves
	{
		//move on every 3rd tick
		if (ticksWaited() == 0)
			setTicksWaited(2);
		else
		{
			setTicksWaited(ticksWaited() - 1);
			return;
		}
		Direction Dir = getWorld()->determineFirstMoveToExit(getX(), getY());
		setDirection(Dir);
		switch(Dir)
		{
			if (ticksWaited() == 0)
			{
			case up: moveTo(getX(), getY() + 1); break;
			case down: moveTo(getX(), getY() - 1); break;
			case left: moveTo(getX() - 1, getY()); break;
			case right: moveTo(getX() + 1, getY()); break;
			case none: break;
			}
		}
		return;
	}

	if (ticksWaited() == 0)
	{
		setTicksWaited(2);
		if (shoutTicks() != 0)
		{
			setShoutTicks(shoutTicks() - 1);
			return;
		}
		if (turnTicksLeft() > 1)
			setTurnTicks(turnTicksLeft() - 1);
	}
	else
	{
		setTicksWaited(ticksWaited() - 1);
		return;
	}
	//Attack Frackman if within radius 4 and is not in leaving field state
	if (!isLeavingField() && shoutTicks() == 0 && getWorld()->isDistAway(4, getX(), getY(), getWorld()->getFrackX(), getWorld()->getFrackY())) //check if within radius 4 of FrackMan
	{
		if (getWorld()->canMove(this, getDirection()))
		{
			switch (getDirection())
			{
			case up:
				if (getWorld()->isDistAway(3, getX(), getY() + 1, getWorld()->getFrackX(), getWorld()->getFrackY()))
				{
					getWorld()->attackFrack(2);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					setShoutTicks(15);
					return;
				}
				break;
			case down:
				getWorld()->isDistAway(3, getX(), getY() - 1, getWorld()->getFrackX(), getWorld()->getFrackY());
				{
					getWorld()->attackFrack(2);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					setShoutTicks(15);
					return;
				}
				break;
			case left:
				getWorld()->isDistAway(3, getX() - 1, getY(), getWorld()->getFrackX(), getWorld()->getFrackY());
				{
					getWorld()->attackFrack(2);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					setShoutTicks(15);
					return;
				}
				break;
			case right:
				getWorld()->isDistAway(3, getX() + 1, getY(), getWorld()->getFrackX(), getWorld()->getFrackY());
				{
					getWorld()->attackFrack(2);
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					setShoutTicks(15);
					return;
				}
				break;
			}
		}
	}
	// Turning perpendicularily
	if (!isLeavingField() && turnTicksLeft() == 1) //if protester can not turn
	{
		Direction dir = getDirection();
		//checks two options (left or right and up or down)
		bool option1 = false;
		bool option2 = false;
		switch (dir)
		{
		case up:
		case down:
		{
			option1 = getWorld()->canMove(this, left);
			option2 = getWorld()->canMove(this, right);
			if (option1 == false && option2 == false) //if there is no intersection, do nothing here
				break;
			if (option1 == true && option2 == true) //if there is an intersection, and protester must choose between two directions
			{
				int randomDir = rand() % 2;
				switch (randomDir)
				{
				case 0:
				{
					setDirection(left);
					moveTo(getX() - 1, getY());
					break;
				}
				case 1:
				{
					setDirection(right);
					moveTo(getX() + 1, getY());
					break;
				}
				}
				setStepsLeft(rand() % 53 + 8); //choose random number of steps
				setStepsLeft(stepsLeft() - 1); //take step
				setTurnTicks(200);             //reset turn ticks
				return;
			}
			//if only one option is true
			if (option1 == true)
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			if (option2 == true)
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
			setStepsLeft(rand() % 53 + 8);
			setStepsLeft(stepsLeft() - 1);
			setTurnTicks(200);
			return;
		}
		case left:
		case right:
		{
			option1 = getWorld()->canMove(this, up);
			option2 = getWorld()->canMove(this, down);
			if (option1 == false && option2 == false)
				break;
			if (option1 == true && option2 == true)
			{
				int randomDir = rand() % 2;
				switch (randomDir)
				{
				case 0:
				{
					setDirection(up);
					moveTo(getX(), getY() + 1);
					break;
				}
				case 1:
				{
					setDirection(down);
					moveTo(getX(), getY() - 1);
					break;
				}
				}
				setStepsLeft(rand() % 53 + 8);
				setStepsLeft(stepsLeft() - 1);
				setTurnTicks(200);
				return;
			}
			if (option1 == true)
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
			if (option2 == true)
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
			setStepsLeft(rand() % 53 + 8);
			setStepsLeft(stepsLeft() - 1);
			setTurnTicks(200);
			return;
		}
		}
	}
	//If there is nothing in the way, and there are still steps to take, move in current direction
	Direction dir = getDirection();
	if (!isLeavingField() && getWorld()->canMove(this, dir) && stepsLeft() > 0)
	{
		setStepsLeft(stepsLeft() - 1);
		switch (dir)
		{
		case up:
			moveTo(getX(), getY() + 1); return;
		case down:
			moveTo(getX(), getY() - 1); return;
		case left:
			moveTo(getX() - 1, getY()); return;
		case right:
			moveTo(getX() + 1, getY()); return;
		}
	}
	else
	{
		Direction newDirection = none;
		int newDir;
		while (true)
		{
			newDir = rand() % 4;
			switch (newDir)
			{
			case 0: newDirection = up; break;
			case 1: newDirection = down; break;
			case 2: newDirection = left; break;
			case 3: newDirection = right; break;
			}
			if (getWorld()->canMove(this, newDirection))
				break;
		}
		setDirection(newDirection);
		switch (newDirection)
		{
		case up:
			moveTo(getX(), getY() + 1); break;
		case down:
			moveTo(getX(), getY() - 1); break;
		case left:
			moveTo(getX() - 1, getY()); break;
		case right:
			moveTo(getX() + 1, getY()); break;
		}
		setStepsLeft(rand() % 53 + 8);
		setStepsLeft(stepsLeft() - 1);
	}
}

void RegularProtester::getAnnoyed(int health, int type)
{
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	getHit(health);
	if (getHealth() == 0)
	{
		setMustLeaveOilField();
		if (type == 1)
			getWorld()->increaseScore(100);
		if (type == 2)
			getWorld()->increaseScore(500);
	}
	//set resting ticks to N ticks
	int MaxRestTicks = 100 - getWorld()->getLevel() * 10;
	int N = max(50, MaxRestTicks);
	setTicksWaited(N);
}

//////////////////
// Frackman Class
//////////////////

FrackMan::FrackMan(StudentWorld* worldPtr)
	: People(IID_PLAYER, worldPtr, 30, 60, 10)
	, m_nSquirts(5), m_nSonar(1), m_nNuggets(0)
{}

void FrackMan::doSomething()
{
	if (getHealth() == 0)
		kill();
	//Check if Frackman is dead
	if (isAlive() == false)
		return;
	//Delete dirt around Frackman
	bool isDeleted = false;
	for (int x = getX(); x <= getX() + 3; x++)
	{
		for (int y = getY(); y <= getY() + 3; y++)
		{
			if (getWorld()->removeDirt(x, y))
				isDeleted = true;
		}
	}
	if (isDeleted)
		getWorld()->playSound(SOUND_DIG);
	//get input and move
	int input;
	getWorld()->getKey(input);
	switch (input)
	{
	case KEY_PRESS_UP:
	{
		if (getWorld()->boulderInWay(this, up)) //check for boulders
		{
			setDirection(up);
			break;
		}
		if (getY() + 1 <= VIEW_HEIGHT - spriteSize) //check if at boundary
		{
			if (getDirection() == up)
				moveTo(getX(), getY() + 1);
		}
		else
			moveTo(getX(), getY());
		setDirection(up);
		break;
	}
	case KEY_PRESS_DOWN:
	{
		if (getWorld()->boulderInWay(this, down)) //check for boulders
		{
			setDirection(down);
			break;
		}
		if (getY() - 1 >= 0) //check if at boundary
		{
			if (getDirection() == down)
				moveTo(getX(), getY() - 1);
		}
		else
			moveTo(getX(), getY());
		setDirection(down);
		break;
	}
	case KEY_PRESS_LEFT:
	{
		if (getWorld()->boulderInWay(this, left)) //check for boulders
		{
			setDirection(left);
			break;
		}
		if (getX() - 1 >= 0) //check if at boundary
		{
			if (getDirection() == left)
				moveTo(getX() - 1, getY());
		}
		else
			moveTo(getX(), getY());
		setDirection(left);
		break;
	}
	case KEY_PRESS_RIGHT:
	{
		if (getWorld()->boulderInWay(this, right)) //check for boulders
		{
			setDirection(right);
			break;
		}
		if (getX() + 1 <= VIEW_WIDTH - spriteSize) //check if at boundary
		{
			if (getDirection() == right)
				moveTo(getX() + 1, getY());
		}
		else
			moveTo(getX(), getY());
		setDirection(right);
		break;
	}
	case 'Z':
	case 'z':
	{
		if (getWorld()->useSonar())
			getWorld()->revealActors(12, getX(), getY());
		break;
	}
	case KEY_PRESS_TAB: getWorld()->useNugget(getX(), getY()); break;
	case KEY_PRESS_SPACE: getWorld()->useSquirt(); break;
	}
}

void FrackMan::getAnnoyed(int health, int points)
{
	getHit(health);
}

/////////////////
// Goodie Class
/////////////////

Goodie::Goodie(StudentWorld* worldPtr, int imageID, int startX, int startY, float size, unsigned int depth, bool visible)
	: Actor(worldPtr, imageID, startX, startY, right, 1.0, depth, visible) {}

////////////////
// Barrel Class
////////////////

OilBarrel::OilBarrel(StudentWorld* worldPtr, int startX, int startY)
	: Goodie(worldPtr, IID_BARREL, startX, startY, 1.0, 2, false)
{
	setVisibility(false);
	getWorld()->incBarrels();
}

void OilBarrel::doSomething()
{
	if (!isAlive()) //check to see if alive
		return;
	if (!isVisible() && getWorld()->isDistAway(4, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY())) //if Frackman is within 4, reveal itself
	{
		setVisible(true);
		setVisibility(true);
		return;
	}
	if (getWorld()->isDistAway(3, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY())) //if Frackman is within 3, activate itself
	{
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decBarrels();
		kill();
	}
}

////////////////////
// Gold Nugget Class
////////////////////

GoldNugget::GoldNugget(StudentWorld* worldPtr, int startX, int startY, bool canFrackPickUp)
	: Goodie(worldPtr, IID_GOLD, startX, startY, 1.0, 2, !canFrackPickUp), m_canFrackPickUP(canFrackPickUp), m_nTicks(100)
{
	setVisibility(!canFrackPickUp);
}

void GoldNugget::doSomething()
{
	if (!isAlive()) //check to see if alive
		return;
	if (getTicks() == 0) //if lifetime runs out, kill it
	{
		kill();
		return;
	}
	if (canFrackPickUp()) //only in invisible state
	{
		if (!isVisible() && getWorld()->isDistAway(4, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY())) //if Frackman is within 4, reveal itself
		{
			setVisible(true);
			setVisibility(true);
			return;
		}
		if (getWorld()->isDistAway(3, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY())) //if Frackman is within 3, activate itself
		{
			getWorld()->playSound(SOUND_GOT_GOODIE); //play sound
			getWorld()->increaseScore(10); //give points
			getWorld()->giveNugget(); //give nugget
			kill();
		}
	}
	else
	{
		decTicks(); //decrement ticks (if nugget is not in frackman pick up state)
		Actor* findProtestor = getWorld()->findNearbyPickerUpper(this, 3);
		if (findProtestor != nullptr && findProtestor->isPerson())
		{
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD); //play sound
			getWorld()->increaseScore(25); //give points
			//TO DO:  Kill Protestor
			kill();
		}
	}
}

//////////////////////////
// Spawnning Goodie Class
//////////////////////////

SpawnningGoodie::SpawnningGoodie(StudentWorld* worldPtr, int imageID, int startX, int startY)
	: Goodie(worldPtr, imageID, startX, startY, 1.0, 2)
{
	int T = 300 - 10 * getWorld()->getLevel();
	m_nTicksLeft = max(100, T);
}

////////////////////////
//	Sonar Kit Class
////////////////////////

SonarKit::SonarKit(StudentWorld* worldPtr)
	: SpawnningGoodie(worldPtr, IID_SONAR, 0, 60) {}

void SonarKit::doSomething()
{
	if (!isAlive()) //check to see if alive
		return;
	if (getTicks() == 0) //if lifetime expires, kill it
	{
		kill();
		return;
	}
	else
		decTicks(); //decrement ticks
	if (getWorld()->isDistAway(3, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY()))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE); //play sound
		getWorld()->giveSonar(); //give frackman sonar kit
		getWorld()->increaseScore(75); //increase score
		kill();
	}
}

////////////////////
// Water Pool
////////////////////

WaterPool::WaterPool(StudentWorld* worldPtr, int x, int y)
	: SpawnningGoodie(worldPtr, IID_WATER_POOL, x, y) {}

void WaterPool::doSomething()
{
	if (!isAlive()) //check to see if alive
		return;
	if (getTicks() == 0) //if lifetime expires, kill it
	{
		kill();
		return;
	}
	else
		decTicks(); //decrement ticks
	if (getWorld()->isDistAway(3, getWorld()->getFrackX(), getWorld()->getFrackY(), getX(), getY()))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE); //play sound
		getWorld()->giveSquirt(); //give frackman squirt
		getWorld()->increaseScore(75); //increase score
		kill();
	}
}
