﻿#pragma once
#include <iostream>
#include <string>
#include "weaponBase.h"
#include "armor.h"
#include "Potions.h"
#include <vector>

class player {
private:
	std::string username;
	int level = 1;
	int xp = 0;
	int xpToNextLevel = 50 * level;
	double health = 50 * level;
	double maxHealth = 50 * level;
	double money = 10.0;
	weaponBase* weapon;
	armor* armorVALUE;
	Potion** potki = new Potion*[15]{ NULL };

	friend std::ostream& operator<<(std::ostream& o, const player& p);
public:
	player();
	player(const std::string& username, const int& level, const int& xp, const double& money, const double& weapon_bD, const double& weapon_cC, const double& weapon_p, const double& armor_bP, const double& armor_p, const int& smallPotions, const int& bigPotions, const string& armor_name); //konstruktor dla wczytywania gry
	~player();

	void checkForNewLevel(const bool& drukuj);

	int getXP();
	int getXPNext();
	int getLevel();
	double getMoney();
	weaponBase* getWeapon();
	armor* getArmor();
	double getHealth();
	double getMaxHealth();
	std::string getUsername();

	Potion** getPotions();
	int getPotionsCount(std::string type);
	void usePotion(std::string type);

	void setWeapon(weaponBase* wB);
	void setArmor(armor* aB);
	void setMoney(const double& money);
	void setHealth(const double& hp);
	
	void addXP(const int& val, const bool& drukuj = false);
};