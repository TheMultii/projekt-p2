#pragma once
#include "player.h"
#include <string>
#include "settingsReader.h"
#include "gameUtilities.h"

class gameEngine {
private:
	player* Player;
public:
	gameEngine(); //konstr. domyœlny (nowa gra)
	gameEngine(const std::string& uuid); //konstr. z wczytywania gry

	player* getPlayer();
	void saveGame(const std::string& uuid);
	void play(settingsReader& sR, gameUtilities& gU);

};