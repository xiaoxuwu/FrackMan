#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld()
{
	//remove Frackman
	delete m_frackMan;
	m_frackMan = nullptr;
	//remove all dirt
	for (int i = 0; i < VIEW_HEIGHT - 4; i++)
	{
		for (int j = 0; j < VIEW_WIDTH; j++)
		{
			delete m_dirt[i][j];
			m_dirt[i][j] = nullptr;
		}
	}
	//remove all actors
	vector<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end())
	{
		delete *iter;
		iter++;
	}
	m_actors.clear(); //clear function wipes the entire vector
}

Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const
{
	vector<Actor*>::const_iterator iter = m_actors.begin();
	while (iter != m_actors.end())
	{
		if ((*iter)->isPerson() && *iter != a && isDistAway(radius, (*iter)->getX(), (*iter)->getY(), a->getX(), a->getY()))
			return *iter;
		iter++;
	}
	return nullptr;
}

Actor* StudentWorld::findNearbyFrackMan(Actor* a, int radius) const
{
	if (isDistAway(radius, getFrackX(), getFrackY(), a->getX(), a->getY())) //if A is within radius of frackman, return frackman pointer
		return m_frackMan;
	return nullptr;
}

int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int type, int radius)
{
	vector<Actor*>::const_iterator iter = m_actors.begin();
	int count = 0;
	while (iter != m_actors.end())
	{
		if (isDistAway(radius, (*iter)->getX(), (*iter)->getY(), annoyer->getX(), annoyer->getY()))
		{
			if (!annoyer->canMoveThrough())
				(*iter)->getAnnoyed(100, type);
			else
				(*iter)->getAnnoyed(2, type);
		}
		count++, iter++;
	}
	return count;
}

GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y)
{
	//check directions
	GraphObject::Direction Dir = GraphObject::none;
	int arr[4];
	int lowest = 1000;
	//Check each direction
	arr[0] = m_findWayBack[x][y + 1]; //up
	arr[1] = m_findWayBack[x][y - 1]; //down
	arr[2] = m_findWayBack[x - 1][y]; //left
	arr[3] = m_findWayBack[x + 1][y]; //right
	for (int i = 0; i < 4; i++)
	{
		if (arr[i] >= 0 && arr[i] < lowest)
		{
			lowest = arr[i];
			switch(i)
			{
				case 0: Dir = GraphObject::up; break;
				case 1: Dir = GraphObject::down; break;
				case 2: Dir = GraphObject::left; break;
				case 3: Dir = GraphObject::right; break;
			}
		}
	}
	return Dir;
}

GraphObject::Direction StudentWorld::lineOfSightToFrackMan(Actor* a) const
{
	//int x = 0, y = 0;
	//check up;
}

void StudentWorld::useNugget(int x, int y)
{
	if (m_frackMan->useNugget())
		m_actors.push_back(new GoldNugget(this, x, y, false));
}

void StudentWorld::useSquirt()
{
	if (m_frackMan->useSquirt())
	{
		GraphObject::Direction Dir = m_frackMan->getDirection();
		int x, y;
		switch (Dir)
		{
		case GraphObject::up: x = 0, y = spriteSize; break;
		case GraphObject::down: x = 0, y = -spriteSize; break;
		case GraphObject::left: x = -spriteSize, y = 0; break;
		case GraphObject::right: x = spriteSize, y = 0; break;
		}
		m_actors.push_back(new Squirt(this, m_frackMan->getX() + x, m_frackMan->getY() + y, Dir));
		playSound(SOUND_PLAYER_SQUIRT);
	}

}

bool StudentWorld::removeDirt(int x, int y)
{
	if (y >= VIEW_HEIGHT - 4 || x >= VIEW_WIDTH || x < 0 || y < 0)
		return false;
	if (m_dirt[y][x] == nullptr)
		return false;

	//reinitialize the array
	for (int i = 0; i < VIEW_HEIGHT; i++)
	for (int j = 0; j < VIEW_WIDTH; j++)
	m_findWayBack[i][j] = -1;
	//initialize variables
	queue<Coord> oilField;
	Coord c; c.x = 60; c.y = 60;
	m_findWayBack[c.x][c.y] = 0;
	oilField.push(c);
	while (!oilField.empty())
	{
		Coord temp = oilField.front();
		oilField.pop();
		//check north
		if (canMoveXY(temp.x, temp.y, GraphObject::up) && m_findWayBack[temp.x][temp.y + 1] < 0)
		{
			m_findWayBack[temp.x][temp.y + 1] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x; c.y = temp.y + 1;
			oilField.push(c);
		}
		//check south
		if (canMoveXY(temp.x, temp.y, GraphObject::down) && m_findWayBack[temp.x][temp.y - 1] < 0)
		{
			m_findWayBack[temp.x][temp.y - 1] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x; c.y = temp.y - 1;
			oilField.push(c);
		}
		//check west
		if (canMoveXY(temp.x, temp.y, GraphObject::left) && m_findWayBack[temp.x - 1][temp.y] < 0)
		{
			m_findWayBack[temp.x - 1][temp.y] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x - 1; c.y = temp.y;
			oilField.push(c);
		}
		//check east
		if (canMoveXY(temp.x, temp.y, GraphObject::right) && m_findWayBack[temp.x + 1][temp.y] < 0)
		{
			m_findWayBack[temp.x + 1][temp.y] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x + 1; c.y = temp.y;
			oilField.push(c);
		}
	}

	//remove dirt object
	delete m_dirt[y][x];
	m_dirt[y][x] = nullptr;
		return true;
}

bool StudentWorld::canMoveXY(int x, int y, GraphObject::Direction dir) const //TODO: check for boulders
{
	int dirCheck = (dir == GraphObject::up || dir == GraphObject::right ? spriteSize : -1);
	//check for dirt objects
	switch (dir)
	{
		case GraphObject::up:
		case GraphObject::down:
		{
			if (dir == GraphObject::up && x + spriteSize > VIEW_HEIGHT) //if moving up, check to see there it is room at top
				return false;
			if (dir == GraphObject::down && y == 0) //if moving down, check to see there is room at bottom
				return false;
			for (int xTemp = x; xTemp < x + spriteSize; xTemp++) //check for dirt objects above (if moving up) or below (if moving down)
			{
				if (m_dirt[y + dirCheck][xTemp] != nullptr)
					return false;
			}
			break;
		}
		case GraphObject::left:
		case GraphObject::right:
		{
			if (dir == GraphObject::right && x + spriteSize > VIEW_WIDTH) //if moving right, check to see there it is room right
				return false;
			if (dir == GraphObject::left && x == 0) //if moving left, check to see there is room left
				return false;
			for (int yTemp = y; yTemp < y + spriteSize; yTemp++) //check for dirt objects left (if moving left) or right (if moving right)
			{
				if (m_dirt[yTemp][x + dirCheck] != nullptr)
					return false;
			}
			break;
		}
	}
	if (boulderInWayXY(x, y, dir)) //check for boulders
		return false;
	return true;
}

bool StudentWorld::boulderInWayXY(int x, int y, GraphObject::Direction dir) const
{
	//Check for boulder
	vector<Actor*>::const_iterator iter = m_actors.begin();
	int dirCheckX = 0;
	int dirCheckY = 0;
	switch (dir)
	{
	case GraphObject::up:
		dirCheckY = 1;
		break;
	case GraphObject::down:
		dirCheckY = -1;
		break;
	case GraphObject::left:
		dirCheckX = -1;
		break;
	case GraphObject::right:
		dirCheckX = 1;
		break;
	}
	while (iter != m_actors.end() && !(*iter)->canMoveThrough())
	{
		if (isDistAway(3, x + dirCheckX, y + dirCheckY, (*iter)->getX(), (*iter)->getY()))
			return true;
		iter++;
	}
	return false;
}

bool StudentWorld::canMove(const Actor* thisActor, GraphObject::Direction dir) const //TODO: check for boulders
{
	int dirCheck = (dir == GraphObject::up || dir == GraphObject::right ? spriteSize : -1);
	//check for dirt objects
	switch (dir)
	{
		case GraphObject::up:
		case GraphObject::down:
		{
			if (dir == GraphObject::up && thisActor->getY() + spriteSize > VIEW_HEIGHT) //if moving up, check to see there it is room at top
				return false;
			if (dir == GraphObject::down && thisActor->getY() < 0) //if moving down, check to see there is room at bottom
				return false;
			for (int x = thisActor->getX(); x < thisActor->getX() + spriteSize; x++) //check for dirt objects above (if moving up) or below (if moving down)
			{
				if (m_dirt[thisActor->getY() + dirCheck][x] != nullptr)
					return false;
			}
			break;
		}
		case GraphObject::left:
		case GraphObject::right:
		{
			if (dir == GraphObject::right && thisActor->getX() + spriteSize > VIEW_WIDTH) //if moving right, check to see there it is room right
				return false;
			if (dir == GraphObject::left && thisActor->getX() < 0) //if moving left, check to see there is room left
				return false;
			for (int y = thisActor->getY(); y < thisActor->getY() + spriteSize; y++) //check for dirt objects left (if moving left) or right (if moving right)
			{
				if (m_dirt[y][thisActor->getX() + dirCheck] != nullptr)
					return false;
			}
			break;
		}
	}
	if (boulderInWay(thisActor, dir)) //check for boulders
		return false;
	return true;
}

bool StudentWorld::boulderInWay(const Actor* thisActor, GraphObject::Direction dir) const
{
	//Check for boulder
	vector<Actor*>::const_iterator iter = m_actors.begin();
	int dirCheckX = 0;
	int dirCheckY = 0;
	switch (dir)
	{
	case GraphObject::up:
		dirCheckY = 1;
		break;
	case GraphObject::down:
		dirCheckY = -1;
		break;
	case GraphObject::left:
		dirCheckX = -1;
		break;
	case GraphObject::right:
		dirCheckX = 1;
		break;
	}
	while (iter != m_actors.end() && *iter != thisActor && !(*iter)->canMoveThrough())
	{
		if (isDistAway(3, thisActor->getX() + dirCheckX, thisActor->getY() + dirCheckY, (*iter)->getX(), (*iter)->getY()))
			return true;
		iter++;
	}
	return false;
}

bool StudentWorld::isDistAway(int radius, int x1, int y1, int x2, int y2) const
{
	double dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	return dist <= radius;
}

void StudentWorld::revealActors(int dist, int x, int y)
{
	vector<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end())
	{
		if (isDistAway(dist, x, y, (*iter)->getX(), (*iter)->getY()))
			(*iter)->setVisible(true);
		iter++;
	}
}

int StudentWorld::init()
{
	//Declare local variables
	vector<Actor*>::iterator iter;
	int numAdded = 0;
	bool canAdd = true;
	//set barrels equal to 0
	m_nBarrels = 0;
	//Initialize Dirt
	for (int y = 0; y < VIEW_HEIGHT; y++)
	{
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			if (!(x >= 30 && x <= 33 && y >= 4) && y < 60) //ignores mineshaft
				m_dirt[y][x] = new Dirt(this, x, y);
			else
				m_dirt[y][x] = nullptr;
		}
	}
	//Initialize Boulders//

	//Calculate number of Boulders needed
	int currBoulNum = getLevel() / 2 + 2;
	int B = min(currBoulNum, 6);

	//Distribute Boulders
	while (numAdded != B)
	{
		int x, y;
		while (true)
		{
			x = rand() % 61, y = rand() % 37 + 20; //generate random Xs and Ys
			for (iter = m_actors.begin(); iter != m_actors.end(); iter++)
			{
				if (isDistAway(6, x, y, (*iter)->getX(), (*iter)->getY())) //if it is within 6 units of another boulder, do not add
				{
					canAdd = false;
					break;
				}
			}
			if (!(x >= 26 && x <= 34 && y >= 4) && canAdd == true) //if in mineshaft, do not add
				break;
			canAdd = true;
		}
		m_actors.push_back(new Boulder(this, x, y)); //add boulder if above test pass
		numAdded++; //increment boulders added
	}
	//initialize Barrels//

	//Calculate number of Boulders needed
	int m = 2 + getLevel();
	int L = min(m, 20);

	//Distribute Barrels
	numAdded = 0;
	canAdd = true;
	while (numAdded != L)
	{
		int x, y;
		while (true)
		{
			x = rand() % 61, y = rand() % 57; //generate random Xs and Ys
			for (iter = m_actors.begin(); iter != m_actors.end(); iter++)
			{
				if (isDistAway(6, x, y, (*iter)->getX(), (*iter)->getY())) //if it is within 6 units of another distributed object, do not add
				{
					canAdd = false;
					break;
				}
			}
			if (!(x >= 26 && x <= 34 && y >= 4) && canAdd == true) //if in mineshaft, do not add
				break;
			canAdd = true;
		}
		m_actors.push_back(new OilBarrel(this, x, y)); //add barrel if above test pass
		numAdded++; //increment barrels added
	}

	//initialize Gold Nuggets//

	//Calculate number of Nuggets needed
	int gold = 5 - getLevel() / 2;
	int G = max(gold, 2);

	//Distribute Nuggets
	numAdded = 0;
	canAdd = true;
	while (numAdded != G)
	{
		int x, y;
		while (true)
		{
			x = rand() % 61, y = rand() % 57; //generate random Xs and Ys
			for (iter = m_actors.begin(); iter != m_actors.end(); iter++)
			{
				if (isDistAway(6, x, y, (*iter)->getX(), (*iter)->getY())) //if it is within 6 units of another distributed object, do not add
				{
					canAdd = false;
					break;
				}
			}
			if (!(x >= 26 && x <= 34 && y >= 4) && canAdd == true) //if in mineshaft, do not add
				break;
			canAdd = true;
		}
		m_actors.push_back(new GoldNugget(this, x, y, true)); //add barrel if above test pass
		numAdded++; //increment barrels added
	}

	//Create array to naviage back//
	//reinitialize the array
	for (int i = 0; i < VIEW_HEIGHT; i++)
		for (int j = 0; j < VIEW_WIDTH; j++)
			m_findWayBack[i][j] = -1;
	//initialize variables
	queue<Coord> oilField;
	Coord c; c.x = 60; c.y = 60;
	m_findWayBack[c.x][c.y] = 0;
	oilField.push(c);
	while (!oilField.empty())
	{
		Coord temp = oilField.front();
		oilField.pop();
		//check north
		if (canMoveXY(temp.x, temp.y, GraphObject::up) && m_findWayBack[temp.x][temp.y + 1] < 0)
		{
			m_findWayBack[temp.x][temp.y + 1] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x; c.y = temp.y + 1;
			oilField.push(c);
		}
		//check south
		if (canMoveXY(temp.x, temp.y, GraphObject::down) && m_findWayBack[temp.x][temp.y - 1] < 0)
		{
			m_findWayBack[temp.x][temp.y - 1] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x; c.y = temp.y - 1;
			oilField.push(c);
		}
		//check west
		if (canMoveXY(temp.x, temp.y, GraphObject::left) && m_findWayBack[temp.x - 1][temp.y] < 0)
		{
			m_findWayBack[temp.x - 1][temp.y] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x - 1; c.y = temp.y;
			oilField.push(c);
		}
		//check east
		if (canMoveXY(temp.x, temp.y, GraphObject::right) && m_findWayBack[temp.x + 1][temp.y] < 0)
		{
			m_findWayBack[temp.x + 1][temp.y] = m_findWayBack[temp.x][temp.y] + 1;
			Coord c; c.x = temp.x + 1; c.y = temp.y;
			oilField.push(c);
		}
	}

	//Initialize Frackman
	m_frackMan = new FrackMan(this);
	//testing
	m_actors.push_back(new RegularProtester(this, 60, 60));
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//declare iterator
	vector<Actor*>::iterator iter;
	//first check if frackMan is alive
	if (!m_frackMan->isAlive())
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	//randomly spawn sonar kits and pools
	int G = getLevel() * 25 + 300;
	int addGoodie = rand() % G;
	if (addGoodie == 0) //add sonar
	{
		int whatGoodie = rand() % 5;
		if (whatGoodie == 0)
			m_actors.push_back(new SonarKit(this));
		else // otherwise add waterpool
		{
			bool canAdd = true;
			while (true)
			{
				int x = rand() % 61, y = rand() % 57; // Place WaterPools Randomly
				for (int i = 0; i < spriteSize && canAdd == true; i++)
				{
					for (int j = 0; j < spriteSize && canAdd == true; j++)
					{
						if (m_dirt[y + j][x + i] != nullptr)
							canAdd = false;
					}
				}
				if (canAdd)
				{
					m_actors.push_back(new WaterPool(this, x, y));
					break;
				}
				canAdd = true;
			}
		}
	}

	//tell every actor to do something
	for (iter = m_actors.begin(); iter != m_actors.end(); iter++)
		(*iter)->doSomething();
	//tell frackman to do something
	m_frackMan->doSomething();
	//delete all dead actors
	iter = m_actors.begin();
	while (iter != m_actors.end())
	{
		if (!(*iter)->isAlive())
		{
			delete *iter;
			iter = m_actors.erase(iter);
		}
		else
			iter++;
	}
	//set Display Text
	setDisplayText();
	//Check to see if all barrels have been picked up
	if (getBarrels() == 0)
		return GWSTATUS_FINISHED_LEVEL;
	else
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//remove Frackman
	delete m_frackMan;
	m_frackMan = nullptr;
	//remove all dirt
	for (int i = 0; i < VIEW_HEIGHT - 4; i++)
	{
		for (int j = 0; j < VIEW_WIDTH; j++)
		{
			delete m_dirt[i][j];
			m_dirt[i][j] = nullptr;
		}
	}
	//remove all actors
	vector<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end())
	{
		delete *iter;
		iter++;
	}
	m_actors.clear(); //clear function wipes the entire vector
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = getFrackHealth();
	int squirts = m_frackMan->numSquirts();
	int gold = m_frackMan->numNuggets();
	int sonar = m_frackMan->numSonar();
	int barrelsLeft = getBarrels();
	string s = formatText(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft)
{
	//format the score
	string formScore = "000000";
	if (score <= 999999) //if score is greater than 999999, return that number (to prevent indexing errors)
	{
		int currentIndex = 5;
		while ((score < 10 && currentIndex >= 0) || score != 0)
		{
			int toAdd = score % 10;
			score /= 10;
			formScore[currentIndex] = toAdd + '0';
			currentIndex--;
		}
	}
	else
		formScore = "999999";
	//format the level
	string formLevel = formatTwoDigit(level);
	//format Lives
	string formLives = formatTwoDigit(lives);
	//format health
	string formHealth = formatTwoDigit(health);
	if (formHealth[0] == '0')
		formHealth[0] = ' ';
	//format Squirts
	string formSquirts = formatTwoDigit(squirts);
	//format Gold
	string formGold = formatTwoDigit(gold);
	//format Sonar
	string formSonar = formatTwoDigit(sonar);
	//format barrelsLeft
	string formBarrels = formatTwoDigit(barrelsLeft);

	string finalFormatted =
		"Scr: " + formScore +
		"  Lvl: " + formLevel +
		"  Lives: " + formLives +
		"  Hlth: " + formHealth + "0%"
		"  Wtr: " + formSquirts +
		"  Gld: " + formGold +
		"  Sonar: " + formSonar +
		"  Oil Left: " + formBarrels;
	return finalFormatted;
}

string StudentWorld::formatTwoDigit(int num)
{
	if (num > 99) //A check to prevent indexing errors
		return "99";
	//format Lives
	string formattedNum = "00"; //start off with two digit
	int currentIndex = 1;
	while ((num < 10 && currentIndex >= 0) || num != 0)
	{
		int toAdd = num % 10; //get the farthest right digit
		num /= 10;
		formattedNum[currentIndex] = toAdd + '0'; //convert to char, add to formatted string
		currentIndex--;
	}
	return formattedNum;
}
