#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>
#include <list>

const int spriteSize = 4;

class Actor;
class Dirt;
class FrackMan;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_nBarrels(0) {}
	virtual ~StudentWorld();
	///////////
	//Getters//
	///////////
	bool isDistAway(int radius, int x1, int y1, int x2, int y2) const;
	bool canMove(const Actor* thisActor, GraphObject::Direction dir) const;
	bool canMoveXY(int x, int y, GraphObject::Direction dir) const;
	bool boulderInWay(const Actor* thisActor, GraphObject::Direction dir) const;
	bool boulderInWayXY(int x, int y, GraphObject::Direction dir) const;
	int getFrackX() const { return m_frackMan->getX(); }
	int getFrackY() const { return m_frackMan->getY(); }
	int getFrackHealth() const { return m_frackMan->getHealth(); }
	int getBarrels() const { return m_nBarrels; }
	Actor* findNearbyPickerUpper(Actor* a, int radius) const;
	Actor* findNearbyFrackMan(Actor* a, int radius) const;
	int annoyAllNearbyActors(Actor* annoyer, int type, int radius);
	GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;
	////////////
	//Mutators//
	////////////
	GraphObject::Direction determineFirstMoveToExit(int x, int y);
	void revealActors(int dist, int x, int y);
	bool removeDirt(int x, int y);
	void attackFrack(int health) { m_frackMan->getAnnoyed(health, 0); }
	//gives Frackman goodie
	void giveSonar() { m_frackMan->getSonar(); }
	void giveSquirt() { m_frackMan->getSquirt(); }
	void giveNugget() { m_frackMan->getNugget(); }
	//tells Frackman to use goodie
	bool useSonar() { playSound(SOUND_SONAR); return m_frackMan->useSonar(); }
	void useSquirt();
	void useNugget(int x, int y);
	//increment and decrement number of barrels
	void incBarrels() { m_nBarrels++; }
	void decBarrels() { m_nBarrels--; }
	/////////////////////////
	//Game Engine Functions//
	/////////////////////////
	virtual int init();
	virtual int move();
	virtual void cleanUp();
private:
	std::vector<Actor*> m_actors;
	Dirt* m_dirt[VIEW_HEIGHT][VIEW_WIDTH];
	FrackMan* m_frackMan;
	int m_nBarrels;
	//////////////////////////
	//Display text functions//
	//////////////////////////
	void setDisplayText();
	std::string formatText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft);
	std::string formatTwoDigit(int num);
	//////////////////////////////
	// Array for finding way back
	//////////////////////////////
	int m_findWayBack[VIEW_HEIGHT][VIEW_WIDTH];
	struct Coord { int x, y; };
};

#endif // STUDENTWORLD_H_
