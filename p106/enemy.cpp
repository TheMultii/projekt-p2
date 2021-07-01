#include "enemy.h"
#include <algorithm>
#include <string>

enemy::enemy(const double& hp, const double& damage) {
    health = maxHealth = hp;
    this->damage = damage;
}

enemy::enemy(const double& hp, const double& damage, string& name) {
    health = maxHealth = hp;
    this->damage = damage;
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    string* test = std::find(std::begin(allowedNames), std::end(allowedNames), name);
    if (test != std::end(allowedNames)) {
        //cout << "Istnieje " << std::distance(allowedNames, test) << endl;
        this->name = name;
    } else {
        //cout << "Nie istnieje" << endl;
    }
}

double enemy::getHealth() {
    return health;
}

double enemy::getMaxHealth() {
    return maxHealth;
}

double enemy::getDamage() {
    return damage;
}

string enemy::getName() {
    return name;
}

void enemy::setHealth(const double& hp) {
    health = hp;
    if (health < 0)
        health = 0;
}

bool enemy::isAlive() {
    return health > 0;
}

void enemy::twarz() {
    if (name == "GOBLIN") {
        cout << "             ,      ,\n";
        cout << "          /(.-''-.)\\\n";
        cout << "      |\\  \\/      \\/  /|\n";
        cout << "      | \\ / =.  .= \\ / |\n";
        cout << "      \\( \\   o\/o   / )/\n";
        cout << "       \\_, '-/  \-' ,_/\n";
        cout << "         /   \\__/   \\\n";
        cout << "         \\ \\__/\\__/ /\n";
        cout << "       ___\\ \\|--|/ /___\n";
        cout << "     /`    \\      /    `\\\n";
        cout << "    /       '----'       \\\n";
    } else {
        //WAMPIR
        cout << "               __.......__\n";
        cout << "            .-:::::::::::::-.\n";
        cout << "          .:::''':::::::''':::.\n";
        cout << "        .:::'     `:::'     `:::. \n";
        cout << "   .'\\  ::'   ^^^  `:'  ^^^   '::  /`.\n";
        cout << "  :   \\ ::   _.__       __._   :: /   ;\n";
        cout << " :     \\`: .' ___\\     /___ `. :'/     ; \n";
        cout << ":       /\   (_|_)\\   /(_|_)   /\       ;\n";
        cout << ":      / .\\   __.' ) ( `.__   /. \      ;\n";
        cout << ":      \\ (        {   }        ) /      ; \n";
        cout << " :      `-(     .  ^ ^  .     )-'      ;\n";
        cout << "  `.       \\  .'<`-._.-'>'.  /       .'\n";
        cout << "    `.      \\    \\;`.';/    /      .'\n";
        cout << "      `._    `-._       _.-'    _.'\n";
        cout << "       .'`-.__ .'`-._.-'`. __.-'`.\n";
        cout << "     .'       `.         .'       `.\n";
        cout << "   .'           `-.   .-'           `.\n";
    }
}

ostream& operator<<(ostream& o, const enemy& e) {
    o << e.health << "/" << e.maxHealth << "HP, " << e.damage << "DMG";
    return o;
}
