#pragma once
#include "armorBase.h"
#include <iostream>
#include <string>
#include <random>

using namespace std;

class armor: public armorBase {
private:
	string nazwa;
	friend std::ostream& operator<<(std::ostream& o, const armor& a);
public:
    armor(const double& bP, const double& p, string nazwa = "") : armorBase(bP, p) {
        if (nazwa == "") {
            string nazwy[5] = {
                "Skorzana zbroja",
                "Zelazna zbroja",
                "Stalowa zbroja",
                "Adamantowa zbroja",
                "Diamentowa zbroja"
            };
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distr(0, 4);
            nazwa=nazwy[distr(gen)];
        }
        this->nazwa = nazwa;
    }
    string getName();
};

