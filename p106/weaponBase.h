#pragma once
#include "equipment.h"
#include <iostream>

class weaponBase: public equipment {
private:
	double baseDamage;
	double criticalChance;

	friend std::ostream& operator<<(std::ostream& o, const weaponBase& wB);
public:
	weaponBase(const double& bD, const double& cC, const double& p);
	double getPrice();
	double getBaseDamage();
	double getCriticalChance();
};

