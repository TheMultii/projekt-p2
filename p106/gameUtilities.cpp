#include "gameUtilities.h"
#include <iomanip>
#include <Windows.h>

void gameUtilities::getMenu() {
	cout << "\n/************ MENU ************\\\n\n";
	cout << "1)" << setw(31) << setfill('.') << "Nowa gra\n";
	cout << "2)" << setw(31) << setfill('.') << "Wczytaj grę\n";
	cout << "3)" << setw(31) << setfill('.') << "Wyjdź z gry\n";
	cout << "\n/************ MENU ************\\\n\n";
}

void gameUtilities::exitGame() {
	system("cls");
	cout << "Dziękuję za grę <3\n\n";
}

void gameUtilities::setColor(const int& color_l) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color_l);
}
