#pragma once
#include "player.h"


class gameEngine {
private:
	player* Player;
public:
	gameEngine(); //konstr. domy�lny (nowa gra)
	// gameEngine(const player& p); //konstr. z wczytywania gry
};

