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
	cout << "Zanim zaczniesz, wpisz swoje imiê/nick:\n\nNazywam siê...";
	string imie_temp;

	cin >> imie_temp;
	while(imie_temp.length()>25) {
		system("cls");
		cout << "\nTwoje imiê/nick nie spe³niaj¹ kryteriów (1-25 znaków). Dostosuj siê, bo nie zagrasz >:C\n";
		cin >> imie_temp;
	}
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
	//dopuszczam na tej fazie ujemny punkty XP. Pewnie potem zapomnê to usun¹æ.
	xp += val;
	if (xp < 0)
		xp = 0;
	checkForNewLevel();
}
