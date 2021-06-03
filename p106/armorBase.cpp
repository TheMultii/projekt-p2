#include "armorBase.h"
#include <iomanip>

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

std::ostream& operator<<(std::ostream& o, const armorBase& aB) {
	o << std::fixed << std::setw(5) << std::setprecision(2) << aB.baseProtection;
	return o;
}
