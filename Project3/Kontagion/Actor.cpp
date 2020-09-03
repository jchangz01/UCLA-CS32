#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* world, int imgID, double x, double y, Direction dir, int depth, int hp)
	:GraphObject(imgID, x, y, dir, depth)
{
	m_world = world;
	m_health = hp;
	m_alive = true;
}

StudentWorld* Actor::getWorld() const {
	return m_world;
}
bool Actor::getAlive() const {
	return m_alive;
}
int Actor::getHP() const {
	return m_health;
}
void Actor::setDead() {
	m_alive = false;
}
void Actor::updateHP(int amtDmg) {
	m_health -= amtDmg;
	if (m_health <= 0)
		setDead();
}
bool Actor::checkSpawnSpacing(double &x, double&y) {
	bool noErrors = true;
	while (sqrt(pow(getX() - x, 2) + pow(getY() - y, 2)) <= SPRITE_WIDTH)
	{
		int angle = randInt(0, 359);
		double r = 120 * sqrt(static_cast<double>(randInt(0, 1000)) / 1000);
		x = VIEW_WIDTH / 2 + r * cos(angle * DEGTORAD);
		y = VIEW_HEIGHT / 2 + r * sin(angle * DEGTORAD);
		noErrors = false;
	}
	return noErrors;
}
bool Actor::checkBacteriaConsumable() const {
	return false;
}
bool Actor::checkBlockableActor() const {
	return false;
}
int Actor::checkBacteriaType() const {
	return 0; //not bacteria
}

Socrates::Socrates(StudentWorld* world, double x, double y)
	:Actor(world, IID_PLAYER, x, y, 0, 0, 100) //a newly spawned Socrates always starts at (0, 128)
{
	m_positionalAngle = 0;
	m_sprayCharges = 20;
	m_flameCharges = 5;
}
int Socrates::getSprayCharge() const {
	return m_sprayCharges;
}
int Socrates::getFlameCharge() const {
	return m_flameCharges;
}
void Socrates::addFlameCharge() {
	m_flameCharges+=5;
}
bool Socrates::checkDamageable() const {
	return true;
}
void Socrates::doSomething() {
	if (!getAlive())
		return;

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_RIGHT: //move soc in clockwise dir
			m_positionalAngle += 5;
			setDirection(-m_positionalAngle);
			moveTo(VIEW_HEIGHT / 2 + VIEW_RADIUS * -cos(m_positionalAngle * DEGTORAD), VIEW_WIDTH / 2 + VIEW_RADIUS * sin(m_positionalAngle * DEGTORAD));
			break;
		case KEY_PRESS_LEFT: //move soc in counterclockwise dir
			m_positionalAngle -= 5;
			setDirection(-m_positionalAngle);
			moveTo(VIEW_HEIGHT / 2 + VIEW_RADIUS * -cos(m_positionalAngle * DEGTORAD), VIEW_WIDTH / 2 + VIEW_RADIUS * sin(m_positionalAngle * DEGTORAD));
			break;
		case KEY_PRESS_ENTER: //shoot flame
			if (m_flameCharges > 0)
			{
				for (int i = 0; i < 16; i++)
				{
					getWorld()->insertProjectile(getX() + SPRITE_WIDTH * cos((getDirection() * DEGTORAD) + i * (FLAMESPREAD * DEGTORAD)),
						getY() + SPRITE_WIDTH * sin((getDirection() * DEGTORAD) + i * (FLAMESPREAD * DEGTORAD)), getDirection() + i * FLAMESPREAD, FLAMEPROJECTILE);
				}
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				m_flameCharges--;
			}
			break;
		case KEY_PRESS_SPACE: //shoot spray
			if (m_sprayCharges > 0)
			{
				getWorld()->insertProjectile(getX() + SPRITE_WIDTH * cos(getDirection() * DEGTORAD), getY() + SPRITE_WIDTH * sin(getDirection() * DEGTORAD), getDirection(), SPRAYPROJECTILE);
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				m_sprayCharges--;                 
			}
			break;
		}
	}
	else
	{
		if (m_sprayCharges < 20)
			m_sprayCharges++;
	}

}

Projectile::Projectile(StudentWorld* world, int imgID, double x, double y, Direction dir, int dmg, int tDist)
	:Actor(world, imgID, x, y, dir, 1)
{
	m_damage = dmg;
	m_travelDistance = tDist;
}
bool Projectile::checkDamageable() const {
	return false;
}
int Projectile::getDamageAmount() const {
	return m_damage;
}
int Projectile::gettDist() const {
	return m_travelDistance;
}
void Projectile::dectDist() {
	m_travelDistance -= SPRITE_WIDTH;
}

Flame::Flame(StudentWorld* world, double x, double y, Direction dir)
	:Projectile (world, IID_FLAME, x, y, dir, 5, 32)
{
}
void Flame::doSomething() {
	if (!getAlive())
		return;

	if (getWorld()->projectileDamage(getX(), getY(), getDamageAmount())) //if object comes in contact with flame, inflict 5 damage
	{
		setDead();
		return;
	}

	moveAngle(getDirection(), SPRITE_WIDTH);
	dectDist();
	if (gettDist() < 0)
		setDead();
}

Spray::Spray(StudentWorld* world, double x, double y, Direction dir) 
	:Projectile(world, IID_SPRAY, x, y, dir, 2, 112)
{
}
void Spray::doSomething() {
	if (!getAlive())
		return;

	if (getWorld()->projectileDamage(getX(), getY(), getDamageAmount())) //if object comes in contact with spray, inflict 2 damage
	{
		setDead();
		return;
	}

	moveAngle(getDirection(), SPRITE_WIDTH);
	dectDist();
	if (gettDist() < 0)
		setDead();
}

Consumable::Consumable(StudentWorld* world, int imgID, double x, double y)
	:Actor(world, imgID, x, y, 0, 1)
{
		m_lifetime = std::max(randInt(0, 300 - 10 * getWorld()->getLevel() - 1), 50);
}
bool Consumable::checkDamageable() const {
	return true;
}
int Consumable::getLifetime() const {
	return m_lifetime;
}
void Consumable::decLifetime() {
	m_lifetime--;
}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, double x, double y)
	:Consumable(world, IID_RESTORE_HEALTH_GOODIE, x, y)
{
}
void RestoreHealthGoodie::doSomething() {
	if (!getAlive())
		return;

	if (getWorld()->overlapsSoc(getX(), getY())) //if Soc consumes goodie, restore health and increase points by 250
	{
		getWorld()->increaseScore(250);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getSoc()->updateHP(getWorld()->getSoc()->getHP() - 100);
		return;
	}
	decLifetime();
	if (getLifetime() <= 0)
		setDead();
}

FlamethrowerGoodie::FlamethrowerGoodie(StudentWorld* world, double x, double y) 
	:Consumable(world, IID_FLAME_THROWER_GOODIE, x, y)
{
}
void FlamethrowerGoodie::doSomething() {
	if (!getAlive())
		return;

	if (getWorld()->overlapsSoc(getX(), getY())) //if Soc consumes goodie, add 5 flame charges and increase points by 300
	{
		getWorld()->increaseScore(250);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getSoc()->addFlameCharge();
		return;
	}
	decLifetime();
	if (getLifetime() <= 0)
		setDead();
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, double x, double y)
	:Consumable(world, IID_EXTRA_LIFE_GOODIE, x, y)
{
}
void ExtraLifeGoodie::doSomething(){
	if (!getAlive())
		return;

	if (getWorld()->overlapsSoc(getX(), getY())) //if Soc consumes goodie, add an extra life and increase points by 500
	{
		getWorld()->increaseScore(500);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->incLives();
		return;
	}
	decLifetime();
	if (getLifetime() <= 0)
		setDead();
}

Fungus::Fungus(StudentWorld* world, double x, double y)
	:Consumable(world, IID_FUNGUS, x, y)
{
}
void Fungus::doSomething(){
	if (!getAlive())
		return;

	if (getWorld()->overlapsSoc(getX(), getY())) //if Soc consumes fungus, decrease health by 20 and decrease points by 50
	{
		getWorld()->increaseScore(-50);
		setDead();
		getWorld()->getSoc()->updateHP(20);
		return;
	}
	decLifetime();
	if (getLifetime() <= 0)
		setDead();
}

Bacteria::Bacteria(StudentWorld* world, int imgID, double x, double y, int hp)
	:Actor(world, imgID, x, y, 90, 0, hp)
{
	m_foodEaten = 0;
	m_movementPlan = 0;
}
bool Bacteria::checkDamageable() const {
	return true;
}
int Bacteria::checkBacteriaType() const {
	return 1; //Salmonella
}
int Bacteria::getFoodEaten() const {
	return m_foodEaten;
}
int Bacteria::getMovementPlan() const {
	return m_movementPlan;
}
void Bacteria::incFoodEaten() {
	m_foodEaten++;
}
void Bacteria::decMovementPlan() {
	m_movementPlan--;
}
void Bacteria::resetFoodEaten() {
	m_foodEaten = 0;
}
void Bacteria::setMovementPlan(int amt) {
	m_movementPlan = amt;
}
void Bacteria::generateDividePoint(double&x, double&y) {
	if (getX() < VIEW_WIDTH / 2)
		x = getX() + SPRITE_WIDTH;
	else if (getX() > VIEW_WIDTH / 2)
		x = getX() - SPRITE_WIDTH;
	else
		x = getX();

	if (getY() < VIEW_HEIGHT / 2)
		y = getY() + SPRITE_WIDTH;
	else if (getY() > VIEW_HEIGHT / 2)
		y = getY() - SPRITE_WIDTH;
	else
		y = getY();
}

void Bacteria::procedure(int dmgAmt, int type)
{
	if (getWorld()->overlapsSoc(getX(), getY())) //deal damage per tick when salmonella or ecoli is on Socrates
	{
		getWorld()->getSoc()->updateHP(dmgAmt);
		getWorld()->playSound(SOUND_PLAYER_HURT);
	}
	else if (getFoodEaten() == 3) //if 3 objects are consumed bacterias will clone
	{
		double xCoord, yCoord;
		generateDividePoint(xCoord, yCoord);
		getWorld()->insertBacteria(xCoord, yCoord, type);
		resetFoodEaten();
	}
	else if (getWorld()->foodConsumed(getX(), getY())) //if bacteria is on a food object, consume the food
		incFoodEaten();
}

RegularSalmonella::RegularSalmonella(StudentWorld* world, double x, double y)
	:Bacteria(world, IID_SALMONELLA, x, y, 4)
{
}
void RegularSalmonella::doSomething()
{
	if (!getAlive())
		return;

	procedure(1, 0); //calls procedure of every bacteria

	if (getMovementPlan() > 0)
	{
		decMovementPlan();
		if (getWorld()->validBacteriaPath(getX(), getY(), 3, getDirection()))
			moveAngle(getDirection(), 3);
		else
		{
			setDirection(randInt(0, 359));
			setMovementPlan(10);
		}
		return;
	}
	else
	{
		Direction towardsFood = 0;
		if (!getWorld()->findClosestFood(getX(), getY(), towardsFood))
		{
			setDirection(randInt(0, 359));
			setMovementPlan(10);
			return;
		}
		else
		{
			if (getWorld()->validBacteriaPath(getX(), getY(), 3, towardsFood))
			{
				setDirection(towardsFood);
				moveAngle(getDirection(), 3);
				setMovementPlan(10);
			}
			else
			{
				setDirection(randInt(0, 359));
				setMovementPlan(10);
				return;
			}
		}
	}
}

AggressiveSalmonella::AggressiveSalmonella(StudentWorld* world, double x, double y)
	:Bacteria(world, IID_SALMONELLA, x, y, 10)
{
}
void AggressiveSalmonella::doSomething()
{
	if (!getAlive())
		return;

	Direction towardsSoc = 0;
	double distance = getWorld()->findSocratesDist(getX(), getY(), towardsSoc);
	if (distance <= 72)
	{
		setDirection(towardsSoc);
		if (getWorld()->validBacteriaPath(getX(), getY(), 3, getDirection()))
		{
			if (distance > 1)
				moveAngle(getDirection(), 3); 
		}
		else
		{
			distance = getWorld()->stuckOnDirt(getX(), getY(), 3, getDirection());
			if (distance < 1)
				moveAngle(getDirection(), distance);
		}
		procedure(2, 1);
		return;
	}

	procedure(2, 1); //calls procedure of every bacteria

	if (getMovementPlan() > 0)
	{
		decMovementPlan();
		if (getWorld()->validBacteriaPath(getX(), getY(), 3, getDirection()))
			moveAngle(getDirection(), 3);
		else
		{
			setDirection(randInt(0, 359));
			setMovementPlan(10);
		}
		return;
	}
	else
	{
		Direction towardsFood = 0;
		if (!getWorld()->findClosestFood(getX(), getY(), towardsFood))
		{
			setDirection(randInt(0, 359));
			setMovementPlan(10);
			return;
		}
		else
		{
			if (getWorld()->validBacteriaPath(getX(), getY(), 3, towardsFood))
			{
				setDirection(towardsFood);
				moveAngle(getDirection(), 3);
			}
			else
			{
				setDirection(randInt(0, 359));
				setMovementPlan(10);
				return;
			}
		}
	}
}

eColi::eColi(StudentWorld* world, double x, double y)
	:Bacteria(world, IID_ECOLI, x, y, 5)
{
}
int eColi::checkBacteriaType() const
{
	return 2; //eColi 
}
void eColi::doSomething()
{
	if (!getAlive())
		return;

	procedure(4, 2); //calls procedure of every bacteria

	Direction towardsSoc;
	double distance = getWorld()->findSocratesDist(getX(), getY(), towardsSoc);
	setDirection(towardsSoc);
	for (int i = 0; i < 10; i++)
	{
		if (getWorld()->validBacteriaPath(getX(), getY(), 2, getDirection()))
		{
			if (distance > 1)
				moveAngle(getDirection(), 2);
			return;
		}
		else
		{
			distance = getWorld()->stuckOnDirt(getX(), getY(), 2, getDirection());
			if (distance < 0)
			{
				moveAngle(getDirection(), distance);
				return;
			}
			else 
			{
				if (getDirection() == 360)
					setDirection(0);
				setDirection(getDirection() + 10);
			}
		}
	}
}

initialSpawnedActor::initialSpawnedActor(StudentWorld* world, int imgID, double x, double y, Direction dir, int depth)
	:Actor(world, imgID, x, y, dir, depth)
{}
bool initialSpawnedActor::checkDamageable() const {
	return false;
}
bool initialSpawnedActor::checkBacteriaConsumable() const {
	return false;
}
bool initialSpawnedActor::checkBlockableActor() const {
	return false;
}
void initialSpawnedActor::doSomething() {
	return;
}

Dirt::Dirt(StudentWorld* world, double x, double y)
	:initialSpawnedActor(world, IID_DIRT, x, y, 0)
{
}
bool Dirt::checkDamageable() const {
	return true;
}
bool Dirt::checkBlockableActor() const {
	return true;
}

Food::Food(StudentWorld* world, double x, double y) 
	: initialSpawnedActor(world, IID_FOOD, x, y, 90)
{
}
bool Food::checkBacteriaConsumable() const {
	return true;
}

Pits::Pits(StudentWorld* world, double x, double y)
	: initialSpawnedActor(world, IID_PIT, x, y, 0)
{
	regSal = 5;
	aggSal = 3;
	eColi = 2;
}
void Pits::doSomething(){
	if (regSal == 0 && aggSal == 0 && eColi == 0)
	{
		setDead();
	}

	int chance = randInt(0, 49);
	if (chance == 24)
	{
		int typeBacteria = randInt(0, 2);
		if (typeBacteria == 0 && regSal > 0)
		{
			getWorld()->insertBacteria(getX(), getY(), typeBacteria);
			regSal--;
			getWorld()->playSound(SOUND_BACTERIUM_BORN);
		}
		if (typeBacteria == 1 && aggSal > 0)
		{
			getWorld()->insertBacteria(getX(), getY(), typeBacteria);
			aggSal--;
			getWorld()->playSound(SOUND_BACTERIUM_BORN);
		}
		if (typeBacteria == 2 && eColi > 0)
		{
			getWorld()->insertBacteria(getX(), getY(), typeBacteria);
			eColi--;
			getWorld()->playSound(SOUND_BACTERIUM_BORN);
		}
	}
	if (regSal == 0 && aggSal == 0 && eColi == 0)
		getWorld()->setBacteriaReleased();
}