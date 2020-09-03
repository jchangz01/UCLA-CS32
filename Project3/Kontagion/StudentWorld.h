#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    Socrates* getSoc() const;
    void insertProjectile(double x, double y, Direction dir, int flameOrSpray);
    void insertBacteria(double x, double y, int type);
    bool projectileDamage(double x, double y, int dmgAmt); //deals damage using projectiles
    bool overlapsSoc(double x, double y); 
    bool foodConsumed(double x, double y);
    bool validBacteriaPath(double x, double y, double dist, Direction dir);
    double stuckOnDirt(double x, double y, double dist, Direction dir);
    double findClosestFood(double x, double y, Direction& dir);
    double findSocratesDist(double x, double y, int& dir);
    void setBacteriaReleased();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
private:
    Socrates* m_soc;
    std::vector <Actor*> m_actors;
    std::vector <Actor*>::iterator it;
    int ChanceGoodie;
    int ChanceFungus;
    int pitsReleased;
    bool allBacteriaReleased;
    void randCoord120(double& x, double& y); //generates random coordinate between the center and radius 120
    void randCoordCirc(double& x, double& y); //gererate random coordinate on the circumference
    int getBacteriaRemaining() const;
    bool getBacteriaReleased() const;
    std::string updateGameStatsVisual();
    const double PI = 4 * atan(1);
    const double DEGTORAD = PI / 180;
    const double RADTODEG = 180 / PI;
    const int X_AXIS = 0;
    const int Y_AXIS = 1;
    const int SALMONELLA = 1;
    const int ECOLI = 2;
};

#endif // STUDENTWORLD_H_
