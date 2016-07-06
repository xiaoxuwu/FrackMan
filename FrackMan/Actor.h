#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//////////////////////////////////////////
// Actor Class and Direct Derived Classes
//////////////////////////////////////////

class Actor : public GraphObject
{
public:
	//Constructor/Destructor
	Actor(StudentWorld* worldPtr, int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, bool visible);
	~Actor() {};
	//Getters
	StudentWorld* getWorld() { return m_world; }
	bool isAlive() const { return m_isAlive; }
	virtual bool canMoveThrough() { return true; }
	virtual bool isPerson() {return false; }
	//Mutators
	virtual void doSomething() = 0;
	virtual void getAnnoyed(int health, int points) = 0;
	void kill();
private:
	StudentWorld* m_world;
	bool m_isAlive;
};

class Dirt : public Actor
{	
public:
	//Constructor/Destructor
	Dirt(StudentWorld* worldPtr, int startX, int startY);
	~Dirt() {}
	//Mutators
	virtual void doSomething();
	virtual void getAnnoyed(int health, int points) { return; }
};

class Boulder : public Actor
{
public:
	Boulder(StudentWorld* worldPtr, int x, int y);
	//Mutators
	void doSomething();
	virtual void getAnnoyed(int health, int points) { return; }
	//getters
	bool falling() const { return m_isFalling; }
	int waitTime() const { return m_ticksWaited; }
	virtual bool canMoveThrough() { return false; }
private:
	bool m_isFalling;
	int m_ticksWaited;
};

class Squirt : public Actor
{
public:
	Squirt(StudentWorld* worldPtr, int x, int y, Direction startDirection);
	virtual void doSomething();
	virtual void getAnnoyed(int health, int points) {}
	//Getters//
	int getMovesLeft() const { return m_nMovesLeft; }
private:
	int m_nMovesLeft;
};

/////////////////////////////////////
// People Class and Derived Classes
/////////////////////////////////////

class People : public Actor
{
public:
	People(int spriteID, StudentWorld* worldPtr, int x, int y, int health, Direction dir);
	//Mutators
	virtual void getAnnoyed(int health, int points) = 0;
	virtual void doSomething() = 0;
	//Getters
	int getHealth();
	void getHit(int health);
	virtual bool isPerson() { return true; }
private:
	int m_health;
};

class FrackMan : public People
{
public:
	//Constructor/Destructor
	FrackMan(StudentWorld* worldPtr);
	//Getters
	int numSquirts() const { return m_nSquirts; }
	int numSonar() const { return m_nSonar; }
	int numNuggets() const { return m_nNuggets; }
	//Mutators
	virtual void doSomething();
	virtual void getAnnoyed(int health, int points);
	void getSquirt() { m_nSquirts+=5; }
	void getSonar() { m_nSonar++; }
	void getNugget() { m_nNuggets++; }
	bool useSquirt() 
	{ 
		if (m_nSquirts != 0)
		{
			m_nSquirts--;
			return true;
		}
		return false;
	}
	bool useSonar()
	{
		if (m_nSonar != 0)
		{
			m_nSonar--;
			return true;
		}
		return false;
	}
	bool useNugget() 
	{
		if (m_nNuggets != 0)
		{
			m_nNuggets--;
			return true;
		}
		return false;
	}
private:
	int m_nSquirts;
	int m_nSonar;
	int m_nNuggets;
};

class Protesters : public People
{
public:
	Protesters(StudentWorld* worldPtr, int startX, int startY, int imageID, int health);
	//Virtual Functions//
	virtual void getAnnoyed(int health, int type) = 0;
	virtual void doSomething() = 0;
	//Mutators//
	//virtual void addGold() = 0;
	//virtual bool huntsFrackMan() const;
	// Set state to having given up protest
	void setMustLeaveOilField() { m_leavingField = true; }
	// Set number of ticks until next move
	void setTicksToNextMove();
	//Getters//
	bool isLeavingField() const { return m_leavingField; }
	int ticksWaited() const { return m_nTicksToRest; }
	int stepsLeft() const { return m_nStepsLeft; }
	int turnTicksLeft() const { return m_nTurnTicks; }
	bool isStunned() const { return m_isStunned; }
	int shoutTicks() const { return m_nShoutTicks;}
	//Mutators//
	void setTicksWaited(int n) { m_nTicksToRest = n; }
	void setStepsLeft(int n) { m_nStepsLeft = n; }
	void setTurnTicks(int n) { m_nTurnTicks = n; }
	void setStunned() { m_isStunned = true; }
	void setShoutTicks(int n) 
	{ 
		if (n > 0)
			m_nShoutTicks = n;
		else
			m_nShoutTicks = 0;
	}
private:
	bool m_leavingField;
	bool m_isStunned;
	int m_nTicksToRest;
	int m_nStepsLeft;
	int m_nTurnTicks;
	int m_nShoutTicks;
};

class RegularProtester : public Protesters
{
public:
	RegularProtester(StudentWorld* worldPtr, int startX, int startY);
	virtual void getAnnoyed(int health, int type);
	virtual void doSomething();
	virtual void addGold() {};
};

////////////////////////////
// Goodie Class
////////////////////////////

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* worldPtr, int imageID, int startX, int startY, float size, unsigned int depth, bool visible = true);
	//Getters
	bool isPermanent() const { return m_isPermanent; }
	bool isVisible() const { return m_isVisible; }
	//Mutators
	void setVisibility(bool visibility) { m_isVisible = visibility; }
	void setTemporary() { m_isPermanent = false; }
	//pure virtual funcs
	virtual void doSomething() = 0;
	virtual void getAnnoyed(int health, int points) { return; }
private:
	bool m_isPermanent;
	bool m_isVisible;
};

////////////////////////////
// Barrel Class
////////////////////////////

class OilBarrel : public Goodie
{
public:
	OilBarrel(StudentWorld* worldPtr, int startX, int startY);
	//Mutators
	virtual void doSomething();
};

////////////////////////////
// Gold Nugget Class
////////////////////////////

class GoldNugget : public Goodie
{
public:
	GoldNugget(StudentWorld* worldPtr, int startX, int startY, bool canFrackPickUp);
	//Getters//
	bool canFrackPickUp() { return m_canFrackPickUP; }
	int getTicks() { return m_nTicks; }
	//Mutators//
	virtual void doSomething();
	void decTicks() { m_nTicks--; }
private:
	bool m_canFrackPickUP;
	int m_nTicks;
};

////////////////////////////
// Spawnning Goodie Class
////////////////////////////

class SpawnningGoodie : public Goodie
{
public:
	SpawnningGoodie(StudentWorld* worldPtr, int imageID, int startX, int startY);
	virtual void doSomething() = 0;
	//Getters
	int getTicks() const { return m_nTicksLeft; }
	void decTicks() { m_nTicksLeft--; }
private:
	int m_nTicksLeft;
};

////////////////////////////
// Sonar Kit Class
////////////////////////////

class SonarKit : public SpawnningGoodie
{
public:
	SonarKit(StudentWorld* worldPtr);
	virtual void doSomething();
};

////////////////////////////
// Water Pool Class
////////////////////////////

class WaterPool : public SpawnningGoodie
{
public:
	WaterPool(StudentWorld* worldPtr, int x, int y);
	virtual void doSomething();
};

#endif // ACTOR_H_
