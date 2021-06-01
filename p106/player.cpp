#include "player.h"
#include <Windows.h>
#include <iostream>

using namespace std;

void setColor(const int& color_l = 7) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color_l);
}

player::player() {
	system("cls");
	cout << "Zanim zaczniesz, wpisz swoje imi�/nick:\n\nNazywam si�...";
	string imie_temp;

	cin >> imie_temp;
	while(imie_temp.length()>25) {
		system("cls");
		cout << "\nTwoje imi�/nick nie spe�niaj� kryteri�w (1-25 znak�w). Dostosuj si�, bo nie zagrasz >:C\n";
		cin >> imie_temp;
	}
	cout << "Od dzisiaj nazywasz si� ";
	setColor(11);
	cout << imie_temp;
	setColor();
	username = imie_temp;
	cout << "!\n\n";

	//bro�
	weapon = new weaponBase(3.5, 0.08, 12);

	//armor
	armor = new armorBase(2, 6);

	system("pause");
}

void player::checkForNewLevel() {
	if (xp >= xpToNextLevel) {
		xp = 0;
		level++;
		xpToNextLevel = level * 50;
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

void player::setWeapon(weaponBase* wB) {
	weapon = wB;
}

void player::setArmor(armorBase* aB) {
	armor = aB;
}

void player::addXP(const int& val) {
	//dopuszczam na tej fazie ujemny punkty XP. Pewnie potem zapomn� to usun��.
	xp += val;
	if (xp < 0)
		xp = 0;
	checkForNewLevel();
}
