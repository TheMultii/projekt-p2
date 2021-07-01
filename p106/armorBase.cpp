#include "armorBase.h"

armorBase::armorBase(const double& bP, const double& p) {
	baseProtection = bP;
	price = p;
}

double armorBase::getPrice() {
	return price;
}

double armorBase::getBaseProtection() {
	return baseProtection;
}
