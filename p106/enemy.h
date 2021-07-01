#pragma once
#include <iostream>
#include <string>

using namespace std;

class enemy {
	double health, maxHealth;
	double damage;
	string name = "GOBLIN";
	string allowedNames[2] = { "GOBLIN", "WAMPIR" };

	friend ostream& operator<<(ostream& o, const enemy& e);

	public:
		enemy(const double& hp, const double& damage);
		enemy(const double& hp, const double& damage, string& name);
		double getHealth();
		double getMaxHealth();
		double getDamage();
		string getName();
		void setHealth(const double& hp);
		bool isAlive();

		void twarz();
};

