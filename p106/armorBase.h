#pragma once
#include <iostream>

class armorBase {
	double baseProtection;
	double price;

	friend std::ostream& operator<<(std::ostream& o, const armorBase& aB);
public:
	armorBase(const double& bP, const double& p);
};

