#pragma once
#include <iostream>
#include <string>

using namespace std;

class enemy {
	double health, maxHealth;
	double damage;
	string name = "GOBLIN";

	friend ostream& operator<<(ostream& o, const enemy& e);

	public:
		enemy(const double& hp, const double& damage);
		double getHealth();
		double getMaxHealth();
		double getDamage();
		string getName();
		void setHealth(const double& hp);
		bool isAlive();

		void twarz();
};

