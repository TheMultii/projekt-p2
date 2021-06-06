#pragma once
#include <string>

class Potion {
public:
	virtual std::string getType() = 0;
	virtual int getHealth() = 0;
};

class bigPotion: public Potion {
private:
	int health = 50;
public:
	bigPotion() {};
	virtual std::string getType();
	virtual int getHealth();
};

class smallPotion: public Potion {
private:
	int health = 20;
public:
	smallPotion() {};
	virtual int getHealth();
	virtual std::string getType();
};