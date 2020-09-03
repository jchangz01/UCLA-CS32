#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

Socrates* StudentWorld::getSoc() const
{
    return m_soc;
}

void StudentWorld::randCoord120(double& x, double& y)
{
    int angle = randInt(0, 359);
    double r = 120 * sqrt(static_cast<double>(randInt(0, 1000)) / 1000);
    x = VIEW_WIDTH / 2 + r * cos(angle * DEGTORAD);
    y = VIEW_HEIGHT / 2 + r * sin(angle * DEGTORAD);
}

void StudentWorld::randCoordCirc(double& x, double& y)
{
    double angle = randInt(0, 359);
    x = VIEW_WIDTH / 2 + VIEW_RADIUS * cos(angle * DEGTORAD);
    y = VIEW_HEIGHT / 2 + VIEW_RADIUS * sin(angle * DEGTORAD);
}

void StudentWorld::insertProjectile(double x, double y, Direction dir, int flameOrSpray)
{
    if (flameOrSpray == 0)
        m_actors.push_back(new Flame(this, x, y, dir));
    else if (flameOrSpray == 1)
        m_actors.push_back(new Spray(this, x, y, dir));
}

void StudentWorld::insertBacteria(double x, double y, int type)
{
    if (type == 0) //adds regSal
        m_actors.push_back(new RegularSalmonella(this, x, y));
    else if (type == 1) //adds aggSal
        m_actors.push_back(new AggressiveSalmonella(this, x, y));
    else if (type == 2) //add eColi
        m_actors.push_back(new eColi(this, x, y));
}

bool StudentWorld::projectileDamage(double x, double y, int dmgAmt)
{
    for (int i = 0; i < m_actors.size(); i++) //Have each actor do something
    {
        if (sqrt(pow(x - m_actors[i]->getX(), 2) + pow(y - m_actors[i]->getY(), 2)) <= SPRITE_WIDTH) 
        { 
                if (m_actors[i]->checkDamageable() && m_actors[i]->getAlive()) //Check if actor can be damaged
                {
                    m_actors[i]->updateHP(dmgAmt);
                    if (m_actors[i]->checkBacteriaType() == SALMONELLA)
                    {
                        if (m_actors[i]->getAlive())
                            playSound(SOUND_SALMONELLA_HURT);
                        else
                        {
                            playSound(SOUND_SALMONELLA_DIE);
                            increaseScore(100);
                            int foodSpawn = randInt(0, 1);
                            if (foodSpawn)
                                m_actors.push_back(new Food(this, m_actors[i]->getX(), m_actors[i]->getY()));
                        }
                    }
                    else if (m_actors[i]->checkBacteriaType() == ECOLI)
                    {
                        if (m_actors[i]->getAlive())
                            playSound(SOUND_ECOLI_HURT);
                        else
                        {
                            playSound(SOUND_ECOLI_DIE);
                            increaseScore(100);
                            int foodSpawn = randInt(0, 1);
                            if (foodSpawn)
                                m_actors.push_back(new Food(this, m_actors[i]->getX(), m_actors[i]->getY()));
                        }
                    }
                    return true;
                }
        }
    }
    return false;
}

bool StudentWorld::overlapsSoc(double x, double y)
{
    if (sqrt(pow(m_soc->getX() - x, 2) + pow(m_soc->getY() - y, 2)) <= SPRITE_WIDTH)
    {
        return true;
    }
    return false;
}

bool StudentWorld::foodConsumed(double x, double y)
{
    for (int i = 0; i < m_actors.size(); i++) //Have each actor do something
    {
        if (m_actors[i]->checkBacteriaConsumable()) //Check to see if actor can be consumed by bacteria
        {
            if (sqrt(pow(x - m_actors[i]->getX(), 2) + pow(y - m_actors[i]->getY(), 2)) <= SPRITE_WIDTH)
            {
                m_actors[i]->setDead();
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::validBacteriaPath(double x, double y, double dist, Direction dir)
{
    for (int i = 0; i < m_actors.size(); i++) //Have each actor do something
    {
        if (m_actors[i]->checkBlockableActor()) //Check to see if actor blocks bacteria
        {
            if (sqrt(pow(x + dist * cos(dir * DEGTORAD) - m_actors[i]->getX(), 2) + pow(y + dist * sin(dir * DEGTORAD) - m_actors[i]->getY(), 2)) <= SPRITE_WIDTH / 2)
                return false;
        }
        if (sqrt(pow(x + dist * cos(dir * DEGTORAD) - VIEW_WIDTH / 2, 2) + pow(y + dist * sin(dir * DEGTORAD) - VIEW_HEIGHT / 2, 2)) >= VIEW_RADIUS)
            return false;
    }
    return true;
}

double StudentWorld::stuckOnDirt(double x, double y, double dist, Direction dir)
{
    for (int i = 0; i < m_actors.size(); i++) //Have each actor do something
    {
        if (m_actors[i]->checkBlockableActor()) //Check to see if actor blocks bacteria
        {
            if (sqrt(pow(x - (m_actors[i]->getX() + cos((dir) * DEGTORAD)), 2) + pow(y - (m_actors[i]->getY() + sin((dir) * DEGTORAD)), 2)) <= dist)
            {
                double d = sqrt(pow(x - (m_actors[i]->getX() + cos((dir + 180) * DEGTORAD)), 2) 
                    + pow(y - (m_actors[i]->getY() + sin((dir + 180) * DEGTORAD)), 2));
                if (d > 0.2)
                    return 0;
                else
                    return d;
            }
        }
    }
    return dist;
}

double StudentWorld::findClosestFood(double x, double y, Direction& dir)
{
    double closestFood = 129;
    double xCoord = 0;
    double yCoord = 0;
    for (int i = 0; i < m_actors.size(); i++) //Have each actor do something
    {
        if (m_actors[i]->checkBacteriaConsumable()) //Check to see if actor can be consumed by bacteria
        {
            if (sqrt(pow(x - m_actors[i]->getX(), 2) + pow(y - m_actors[i]->getY(), 2)) < closestFood)
            {
                closestFood = sqrt(pow(x - m_actors[i]->getX(), 2) + pow(y - m_actors[i]->getY(), 2));
                xCoord = m_actors[i]->getX() - x;
                yCoord = m_actors[i]->getY() - y;
            }
        }
    }
    if (closestFood < 129)
    {

        dir = atan2(yCoord, xCoord) * RADTODEG;
        return closestFood;
    }
    else
        return 0;
}

double StudentWorld::findSocratesDist(double x, double y, int& dir)
{
    double dist = (sqrt(pow(m_soc->getX() - x, 2) + pow(m_soc->getY() - y, 2)));
    double xCoord, yCoord;
    xCoord = m_soc->getX() - x;
    yCoord = m_soc->getY() - y;
    dir = atan2(yCoord,xCoord) * RADTODEG;
    return dist;
}

bool StudentWorld::getBacteriaReleased() const
{
    return allBacteriaReleased;
}

void StudentWorld::setBacteriaReleased()
{
    pitsReleased++;
    if (pitsReleased == getLevel())
        allBacteriaReleased = true;
}

int StudentWorld::getBacteriaRemaining() const
{
    int bacteriaCount = 0;
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->checkBacteriaType())
            bacteriaCount++;
    }
    return bacteriaCount;
}

string StudentWorld::updateGameStatsVisual()
{
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss << setw(6) << getScore();
    oss.fill(' ');
    oss << setw(9) << "Level: " << getLevel();
    oss << setw(9) << "Lives: " << getLives();
    oss << setw(10) << "Health: " << m_soc->getHP();
    oss << setw(10) << "Sprays: " << m_soc->getSprayCharge();
    oss << setw(10) << "Flames: " << m_soc->getFlameCharge();
    string s = oss.str();
    return s;
}

int StudentWorld::init()
{
    m_soc = new Socrates (this, 0, VIEW_HEIGHT/2); //creating socrates
    double tempX, tempY;
    for (int i = 0; i < getLevel(); i++) //creating pits
    {
        randCoord120(tempX, tempY);
        for (int k = 0; k < m_actors.size(); k++)
        {
            if (m_actors[k]->checkSpawnSpacing(tempX, tempY))
                continue;
            else
                k = -1;
        }
        m_actors.push_back(new Pits(this, tempX, tempY));
    }
    allBacteriaReleased = false; //bacteria have yet to be released into the game
    pitsReleased = 0;

    for (int i = 0; i < min(5 * getLevel(), 25); i++) //creating food
    {
        randCoord120(tempX, tempY);
        for (int h = 0; h < m_actors.size(); h++)
        {
            if (m_actors[h]->checkSpawnSpacing(tempX, tempY))
                continue;
            else
                h = -1;
        }
        m_actors.push_back(new Food(this, tempX, tempY));
    }
    for (int i = 0; i < max(180 - 20 * getLevel(), 20); i++) //creating dirt piles
    {
        randCoord120(tempX, tempY);
        for (int j = 0; j < (getLevel() + min(5 * getLevel(), 25)); j++)
        {
            if (m_actors[j]->checkSpawnSpacing(tempX, tempY))
                continue;
            else
                j = -1;
        }
        m_actors.push_back(new Dirt(this, tempX, tempY));
    }

    ChanceGoodie = max(510 - getLevel() * 10, 250); //goodie spawn chance
    ChanceFungus = max(510 - getLevel() * 10, 200); //fungus spawn chance
    setGameStatText(updateGameStatsVisual()); //update game stat's text
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    ////////////////////////////////////////////
    /*Give Each Actor a Chance to Do Something*/
    ////////////////////////////////////////////
    m_soc->doSomething();
    for (int i = 0; i < m_actors.size(); i++) //Have each actor do something
    {
        if (m_actors[i]->getAlive())
            m_actors[i]->doSomething();

        if (!m_soc->getAlive()) //end game if Socrates dies
        {
            playSound(SOUND_PLAYER_DIE); 
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }

        if (getBacteriaReleased() && getBacteriaRemaining() == 0)
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }

    //////////////////////////////////////
    /*Remove Dead Actors After Each Tick*/
    //////////////////////////////////////
    it = m_actors.begin();
    while (it != m_actors.end())
    {
        if (!(*it)->getAlive())
        {
            delete* it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }

    ///////////////////////////////////////////////
    /*Add New Goodies As Required After Each Tick*/
    ////////////////////////////////////////////////
    int chance;
    double xCoord, yCoord;
    
    chance = randInt(0, ChanceFungus);
    if (chance == 0) //spawn fungi
    {
        randCoordCirc(xCoord, yCoord);
        m_actors.push_back(new Fungus(this, xCoord, yCoord));
    }

    chance = randInt(0, ChanceGoodie);
    if (chance == 0) //spawn goodies
    {
        randCoordCirc(xCoord, yCoord);
        int goodieType = randInt(0, 9);
        switch (goodieType)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            m_actors.push_back(new RestoreHealthGoodie(this, xCoord, yCoord));
            break;
        case 6:
        case 7:
        case 8:
            m_actors.push_back(new FlamethrowerGoodie(this, xCoord, yCoord));
            break;
        case 9:
            m_actors.push_back(new ExtraLifeGoodie(this, xCoord, yCoord));
            break;
        }
    }

    //Update game stat's text
    setGameStatText(updateGameStatsVisual()); 
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (m_soc != nullptr)
    {
        delete m_soc;
        m_soc = nullptr;
    }
    it = m_actors.begin();
    while (it != m_actors.end())
    {
        delete* it;
        it = m_actors.erase(it);
    }
}

