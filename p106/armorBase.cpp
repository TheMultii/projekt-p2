#include "armorBase.h"
#include <iomanip>

armorBase::armorBase(const double& bP, const double& p) {
	baseProtection = bP;
	price = p;
}

std::ostream& operator<<(std::ostream& o, const armorBase& aB) {
	o << std::fixed << std::setw(5) << std::setprecision(2) << aB.baseProtection;
	return o;
}
