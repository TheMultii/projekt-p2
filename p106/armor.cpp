#include "armor.h"
#include <iomanip>
#include <iostream>

std::ostream& operator<<(std::ostream& o, const armor& a) {
	o << a.nazwa << ": " << std::fixed << std::setw(5) << std::setprecision(2) << a.baseProtection;
	return o;
}

string armor::getName() {
	return nazwa;
}
