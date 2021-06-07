#include "enemy.h"

enemy::enemy(const double& hp, const double& damage) {
    health = maxHealth = hp;
    this->damage = damage;
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
}

ostream& operator<<(ostream& o, const enemy& e) {
    o << e.health << "/" << e.maxHealth << "HP, " << e.damage << "DMG";
    return o;
}
