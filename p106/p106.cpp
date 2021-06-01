#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include "settingsReader.h"
#include "gameUtilities.h"
#include <thread>
#include <chrono>
#include <limits>
#define NOMINMAX
#include <Windows.h>
#include "gameEngine.h"

using namespace std;

int main() {
	settingsReader settings;
	cout << "Fajna gra | " << settings.getAPIVersion() << "\n";
	gameUtilities gUtilities;
	gUtilities.getMenu();
	cout << "Twój wybór:    ";
	int temp;
	cin >> setw(45) >> temp;
	while (cin.fail() || temp < 1 || temp > 3) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nTo nie jest cyfra z zakresu 1-3!\n";
		this_thread::sleep_for(chrono::milliseconds(500));
		system("cls");
		cin >> temp;
	}
	system("cls");
	switch (temp) {
		case 1:
			{
				gameEngine gE;
			}
			break;
		case 2:
			//tu będzie synchronizacja (ładowanie postaci)
			break;
		case 3:
			gUtilities.exitGame();
			return 0;
			break;
	}
	return 0;
}