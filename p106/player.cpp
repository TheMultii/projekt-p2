#include "player.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

void setColor(const int& color_l = 7) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color_l);
}

player::player() {
	system("cls");
	cout << "Zanim zaczniesz, wpisz swoje imiê/nick:\n\nNazywam siê ";
	string imie_temp;
	setColor(11);
	cin >> imie_temp;
	setColor();
	while(imie_temp.length()>25) {
		system("cls");
		cout << "\nTwoje imiê/nick nie spe³niaj¹ kryteriów (1-25 znaków). Dostosuj siê, bo nie zagrasz >:C\n";
		cin >> imie_temp;
	}
	system("cls");
	cout << "Od dzisiaj nazywasz siê ";
	setColor(11);
	cout << imie_temp;
	setColor();
	username = imie_temp;
	cout << "!\n\n";

	//broñ
	weapon = new weaponBase(3.5, 0.08, 12);

	//armor
	armor = new armorBase(2, 6);

	system("pause");
	system("cls");
}

player::player(const std::string& username, const int& level, const int& xp, const double& money, const double& weapon_bD, const double& weapon_cC, const double& weapon_p, const double& armor_bP, const double& armor_p, const int& smallPotions, const int& bigPotions) {
	this->username = username;
	this->level = level;
	this->xp = xp;
	this->money = money;
	this->weapon = new weaponBase(weapon_bD, weapon_cC, weapon_p);
	this->armor = new armorBase(armor_bP, armor_p);
	this->xpToNextLevel = 50 * this->level;
	this->maxHealth = this->health = 50 * this->level;

	if (smallPotions + bigPotions > 15) {
		cout << "\nIloœæ potek siê nie zgadza, ustawiam na 10 ma³ych, 5 du¿ych...\n";
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 15; j++) {
				if (potki[j] == NULL) {
					potki[j] = new smallPotion;
					break;
				}
			}
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 15; j++) {
				if (potki[j] == NULL) {
					potki[j] = new bigPotion;
					break;
				}
			}
		}
	} else {
		for (int i = 0; i < smallPotions; i++) {
			for (int j = 0; j < 15; j++) {
				if (potki[j] == NULL) {
					//cout << "tworze s" << i << "\n";
					potki[j] = new smallPotion;
					break;
				}
				
			}
		}

		for (int i = 0; i < bigPotions; i++) {
			for (int j = 0; j < 15; j++) {
				if (potki[j] == NULL) {
					//cout << "tworze b" << i << "\n";
					potki[j] = new bigPotion;
					break;
				}
			}
		}
	}
	system("cls");
}

player::~player() {
	delete armor;
	delete weapon;
	for (int i = 0; i < 15; i++) {
		delete potki[i];
	}
	delete potki;
}

void player::checkForNewLevel(const bool& drukuj) {
	if (xp >= xpToNextLevel) {
		xp = 0;
		level++;
		xpToNextLevel = level * 50;
		maxHealth = health = level * 50;
		if (drukuj) {
			setColor(12);
			cout << "\nNowy poziom!";
			setColor();
			cout << "(LV" << level << ")\n";
		}
	}
}

int player::getXP() {
	return xp;
}

int player::getXPNext() {
	return xpToNextLevel;
}

int player::getLevel() {
	return level;
}

double player::getMoney() {
	return money;
}

weaponBase* player::getWeapon() {
	return weapon;
}

armorBase* player::getArmor() {
	return armor;
}

double player::getHealth() {
	return health;
}

double player::getMaxHealth() {
	return maxHealth;
}


std::string player::getUsername() {
	return username;
}

Potion** player::getPotions() {
	return potki;
}

int player::getPotionsCount(std::string type) {
	int ret = 0;
	for (int i = 0; i < 15; i++) {
		if (potki[i] != NULL) {
			if (potki[i]->getType() == type) {
				ret++;
			}
		}
	}
	return ret;
}

void player::usePotion(std::string type) {
	bool isSearching = true;
	for (int i = 14; i >= 0 && isSearching; i--) {
		if (potki[i] != NULL) {
			if (potki[i]->getType() == type) {
				setHealth(getHealth() + potki[i]->getHealth());
				delete potki[i];
				isSearching = false;
			}
		}
	}
}

void player::setWeapon(weaponBase* wB) {
	delete weapon;
	weapon = wB;
}

void player::setArmor(armorBase* aB) {
	delete armor;
	armor = aB;
}

void player::setMoney(const double& money) {
	this->money = money;
}

void player::setHealth(const double& hp) {
	health = hp;
	if (health > getMaxHealth())
		health = getMaxHealth();
}

void player::addXP(const int& val, const bool& drukuj) {
	//dopuszczam na tej fazie ujemny punkty XP. Pewnie potem zapomnê tego komentarza usun¹æ.
	xp += val;
	if (xp < 0)
		xp = 0;
	checkForNewLevel(drukuj);
}

std::ostream& operator<<(std::ostream& o, const player& p) {
	setColor(11);
	o << setfill('.') << setw(15) << std::right << p.username << ", LV "<< p.level;
	setColor();
	o << "|";
	setColor(12);
	o << setfill('.') << setw(8) << std::right << p.health << "/" << p.maxHealth << "HP";
	setColor();
	o << "|";
	o << setfill('.') << setw(8) << std::right << p.xp << "/" << p.xpToNextLevel << "XP|";
	o << "WEP:" << setfill('.') << setw(8) << std::right << p.weapon->getBaseDamage() << "/" << (p.weapon->getCriticalChance() * 100) << "%/" << p.weapon->getPrice() << "$|";
	o << "ARM:" << setfill('.') << setw(8) << std::right << p.armor->getBaseProtection() << "/" << p.armor->getPrice() << "$|";
	setColor(2);
	o << "MON:" << setfill('.') << setw(6) << std::right << p.money << "$|";
	setColor();
	int sP = 0, bP = 0;
	for (int i = 0; i < 15; i++) {
		if (p.potki[i] != NULL) {
			if (p.potki[i]->getType() == "smallPotion") {
				sP++;
			} else if (p.potki[i]->getType() == "bigPotion") {
				bP++;
			}
		}
	}
	o << "M:" << setfill('.') << setw(2) << std::right << sP << ", D:" << setfill('.') << setw(2) << std::right << bP;

	return o;
}
