#pragma once
#include <iostream>
#include <string>
#include "weaponBase.h"
#include "armorBase.h"

class player {
private:
	std::string username;
	int level = 1;
	int xp = 0;
	int xpToNextLevel = level * 50;
	double money = 10.0;
	weaponBase* weapon;
	armorBase* armor;
public:
	player();

	void checkForNewLevel();
	//drukowanie #TODO

	int getXP();
	int getXPNext();
	int getLevel();

	void setWeapon(weaponBase* wB);
	void setArmor(armorBase* aB);
	
	void addXP(const int& val);
};

