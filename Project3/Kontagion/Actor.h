#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(StudentWorld* world, int imgID, double x, double y, Direction dir = 0, int depth = 0, int hp = -1);
	StudentWorld* getWorld() const; //returns pointer the world
	bool getAlive() const; //returns Actor status dead/alive
	int getHP() const; //returns Actor's heatlh if valid
	void setDead(); //sets Actor'status to dead
	void updateHP(int dmgAmt); 
	virtual bool checkSpawnSpacing(double& x, double& y);
	virtual bool checkDamageable() const = 0;
	virtual bool checkBacteriaConsumable() const;
	virtual bool checkBlockableActor() const;
	virtual int checkBacteriaType() const;
	virtual void doSomething() = 0;
protected:
	const double PI = 4 * atan(1);
	const double DEGTORAD = PI / 180;
	const double RADTODEG = 180 / PI;
private:
	bool m_alive;
	int m_health;
	StudentWorld* m_world;
};

class Socrates : public Actor {
public:
	Socrates(StudentWorld* world, double x, double y);
	int getSprayCharge() const;
	int getFlameCharge() const;
	void addFlameCharge();
	virtual bool checkDamageable() const;
	virtual void doSomething();
private:
	int m_positionalAngle;
	int m_sprayCharges;
	int m_flameCharges;
	const int FLAMEPROJECTILE = 0;
	const int SPRAYPROJECTILE = 1;
	const int FLAMESPREAD = 22;
};

//////////////////////////////
/*Projectile Base Class Objects*/
//////////////////////////////
class Projectile : public Actor {
public:
	Projectile(StudentWorld* world, int imgID, double x, double y, Direction dir, int dmg, int tDist);
	virtual bool checkDamageable() const;
	int getDamageAmount() const;
	int gettDist() const;
	void dectDist(); //decrease m_travelDistance by SPRITE_WIDTH
	virtual void doSomething() = 0;
private:
	int m_damage;
	int m_travelDistance;
};

class Flame : public Projectile {
public:
	Flame(StudentWorld* world, double x, double y, Direction dir);
	virtual void doSomething();
};

class Spray : public Projectile {
public:
	Spray(StudentWorld* world, double x, double y, Direction dir);
	virtual void doSomething();
};

/////////////////////////////////
/*Consumable Base Class Objects*/
/////////////////////////////////
class Consumable : public Actor {
public:
	Consumable(StudentWorld* world, int imgID, double x, double y);
	virtual bool checkDamageable() const;
	int getLifetime() const;
	void decLifetime();
	virtual void doSomething() = 0;
private:
	int m_lifetime;
};

class RestoreHealthGoodie : public Consumable {
public:
	RestoreHealthGoodie(StudentWorld* world, double x, double y);
	virtual void doSomething();
};

class FlamethrowerGoodie : public Consumable {
public:
	FlamethrowerGoodie(StudentWorld* world, double x, double y);
	virtual void doSomething();
};

class ExtraLifeGoodie : public Consumable {
public: 
	ExtraLifeGoodie(StudentWorld* world, double x, double y);
	virtual void doSomething();
};

class Fungus : public Consumable {
public:
	Fungus(StudentWorld* world, double x, double y);
	virtual void doSomething();
};

///////////////////////////////
/*Bacteria Base Class Objects*/
///////////////////////////////
class Bacteria : public Actor {
public:
	Bacteria(StudentWorld* world, int imgID, double x, double y, int hp);
	virtual bool checkDamageable() const;
	virtual int checkBacteriaType() const;
	int getFoodEaten() const;
	int getMovementPlan() const;
	void incFoodEaten();
	void decMovementPlan();
	void resetFoodEaten();
	void setMovementPlan(int amt);
	void generateDividePoint(double& x, double& y);
	void procedure(int dmgAmt, int type);
	virtual void doSomething() = 0;
private:
	int m_foodEaten;
	int m_movementPlan;
};

class RegularSalmonella : public Bacteria {
public:
	RegularSalmonella(StudentWorld* world, double x, double y);
	virtual void doSomething();
};

class AggressiveSalmonella : public Bacteria {
public:
	AggressiveSalmonella(StudentWorld* world, double x, double y); 
	virtual void doSomething();
};

class eColi : public Bacteria {
public:
	eColi(StudentWorld* world, double x, double y);
	virtual int checkBacteriaType() const;
	virtual void doSomething();
};

///////////////////////////////////////
/*Initially Spawned Objects aka Misc.*/
///////////////////////////////////////
class initialSpawnedActor : public Actor {
public:
	initialSpawnedActor(StudentWorld* world, int imgID, double x, double y, Direction dir, int depth = 1);
	virtual bool checkDamageable() const;
	virtual bool checkBacteriaConsumable() const;
	virtual bool checkBlockableActor() const;
	virtual void doSomething();
private:
};

class Dirt : public initialSpawnedActor {
public:
	Dirt(StudentWorld* World, double x, double y);
	virtual bool checkDamageable() const;
	virtual bool checkBlockableActor() const;
private:
};

class Food  : public initialSpawnedActor {
public:
	Food(StudentWorld* World, double x, double y);
	virtual bool checkBacteriaConsumable() const;
private:
};


class Pits : public initialSpawnedActor {
public: 
	Pits(StudentWorld* World, double x, double y);
	virtual void doSomething();
private:
	int regSal;
	int aggSal;
	int eColi;
};
#endif // ACTOR_H_
