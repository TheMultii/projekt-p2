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

player::player(const std::string& username, const int& level, const int& xp, const double& money, const double& weapon_bD, const double& weapon_cC, const double& weapon_p, const double& armor_bP, const double& armor_p) {
	this->username = username;
	this->level = level;
	this->xp = xp;
	this->money = money;
	this->weapon = new weaponBase(weapon_bD, weapon_cC, weapon_p);
	this->armor = new armorBase(armor_bP, armor_p);
	this->xpToNextLevel = 50 * this->level;
	this->maxHealth = this->health = 50 * this->level;
	system("cls");
}

void player::checkForNewLevel() {
	if (xp >= xpToNextLevel) {
		xp = 0;
		level++;
		xpToNextLevel = level * 50;
		maxHealth = health = level * 50;
	}
}

int player::getXP()
{
	return xp;
}

int player::getXPNext()
{
	return xpToNextLevel;
}

int player::getLevel()
{
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

void player::setWeapon(weaponBase* wB) {
	weapon = wB;
}

void player::setArmor(armorBase* aB) {
	armor = aB;
}

void player::addXP(const int& val) {
	//dopuszczam na tej fazie ujemny punkty XP. Pewnie potem zapomnê to usun¹æ.
	xp += val;
	if (xp < 0)
		xp = 0;
	checkForNewLevel();
}

std::ostream& operator<<(std::ostream& o, const player& p) {
	o << setfill('.') << setw(15) << std::right << p.username << "|";
	o << setfill('.') << setw(10) << std::right << p.xp << "/" << p.xpToNextLevel << "XP|";
	o << "WEP:" << setfill('.') << setw(15) << std::right << p.weapon->getBaseDamage() << "/" << p.weapon->getCriticalChance() << "/" << p.weapon->getPrice() << "$|";
	o << "ARM:" << setfill('.') << setw(15) << std::right << p.armor->getBaseProtection() << "/" << p.armor->getPrice() << "$|";
	o << "MON:" << setfill('.') << setw(15) << std::right << p.money << "$";

	return o;
}
