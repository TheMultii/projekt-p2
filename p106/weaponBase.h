#pragma once
#include <iostream>

class weaponBase {
private:
	double baseDamage;
	double criticalChance;
	double price;

	friend std::ostream& operator<<(std::ostream& o, const weaponBase& wB);
public:
	weaponBase(const double& bD, const double& cC, const double& p);
	double getPrice();
	double getBaseDamage();
	double getCriticalChance();
};

