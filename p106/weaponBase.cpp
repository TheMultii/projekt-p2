#include "weaponBase.h"
#include <iomanip>

weaponBase::weaponBase(const double& bD, const double& cC, const double& p) {
	baseDamage = bD;
	criticalChance = cC;
	price = p;
}

double weaponBase::getPrice() {
	return price;
}

double weaponBase::getBaseDamage() {
	return baseDamage;
}

double weaponBase::getCriticalChance() {
	return criticalChance;
}

std::ostream& operator<<(std::ostream& o, const weaponBase& wB) {
	o << std::fixed << std::setw(5) << std::setprecision(2) << wB.baseDamage;
	o << " | ";
	o << std::fixed << std::setw(5) << std::setprecision(2) << (wB.criticalChance * 100);
	return o;
}
