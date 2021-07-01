#pragma once
#include "equipment.h"
#include <iostream>

class armorBase: public equipment {
protected:
	double baseProtection;
public:
	armorBase(const double& bP, const double& p);
	virtual double getPrice();
	virtual double getBaseProtection();
};

