#pragma once
#include <iostream>
#include <string>
#include "player.h"

using namespace std;

class gameUtilities {
private:
public:
	gameUtilities() {}

	void getMenu();
	void exitGame();
	void setColor(const int& color_l = 7);
};

