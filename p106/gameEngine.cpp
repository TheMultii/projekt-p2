#include "gameEngine.h"
#include "gameUtilities.h"
#include "settingsReader.h"
#include "enemy.h"
#include <iostream>
#include <random>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <iomanip>
#include <thread>
#include <chrono>
#include <limits>
#include <Windows.h>
#undef max

using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

gameEngine::gameEngine() {
    Player = new player;
}

gameEngine::gameEngine(const std::string& uuid) {
    CURL* curl;
    CURLcode res;
    string readBuffer, errors{};
    Json::Value obj;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    system("cls");
    cout << "Rozpoczynam synchronizację z serwerem...\n";
    if (curl) {
        string url = "https://v4.api.mganczarczyk.pl/v4/projekt-p2/";
        url += uuid;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::CharReaderBuilder builder{};
    auto reader = unique_ptr<Json::CharReader>(builder.newCharReader());
    const auto is_parsed = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.length(), &obj, &errors);    system("cls");
    if (is_parsed) {
        if (!obj.size()) {
            //serwer zwrócił np. 500 (wtedy coś zepsułem i muszę to naprawić :/) lub zapis w bazie nie istnieje
            cout << "Nie udało się wczytać postępów. Zamknij aplikację jeżli nie chcesz utracić postępów, w przciwnym razie konto zostanie nadpisane profilem nowej gry.\n\n";
            system("pause");
            delete Player;
            Player = new player;
        } else {
            delete Player;
            string username = obj["username"].asString();
            int level = obj["level"].asInt();
            int xp = atoi(obj["xp"].asString().c_str());
            double money = obj["money"].asDouble();
            double weapon_baseDamage = obj["weapon_baseDamage"].asDouble();
            double weapon_critChance = obj["weapon_critChance"].asDouble();
            double weapon_price = obj["weapon_price"].asDouble();
            double armor_baseProtection = obj["armor_baseProtection"].asDouble();
            double armor_price = obj["armor_price"].asDouble();
            int smallPotions = obj["smallPotions"].asInt();
            int bigPotions = obj["bigPotions"].asInt();
            Player = new player(
                username,
                level,
                xp,
                money,
                weapon_baseDamage,
                weapon_critChance,
                weapon_price,
                armor_baseProtection,
                armor_price,
                smallPotions,
                bigPotions
            );
        }
    } else {
        cout << "Nie udało się wczytać postępów. Zamknij aplikację jeżli nie chcesz utracić postępów, w przciwnym razie konto zostanie nadpisane profilem nowej gry.\n\n";
        system("pause");
        delete Player;
        Player = new player;
    }
}

// zapsiywanie

string generatePostBody(string imie, int level, int xp, double money, double weapon_price, double armor_price, double weapon_critChance, double weapon_baseDamage, double armor_baseProtection, int smallPotions, int bigPotions) {
    string str = "username=" + imie
        + "&level=" + to_string(level)
        + "&xp=" + to_string(xp)
        + "&money=" + to_string(money)
        + "&weapon_price=" + to_string(weapon_price)
        + "&armor_price=" + to_string(armor_price)
        + "&weapon_critChance=" + to_string(weapon_critChance)
        + "&weapon_baseDamage=" + to_string(weapon_baseDamage)
        + "&armor_baseProtection=" + to_string(armor_baseProtection)
        + "&smallPotions=" + to_string(smallPotions)
        + "&bigPotions=" + to_string(bigPotions);
    return str;
}

player* gameEngine::getPlayer() {
    return Player;
}

void gameEngine::saveGame(const string& uuid) {
    CURL* curl;
    CURLcode res;
    string readBuffer, errors{};
    Json::Value obj;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    system("cls");
    cout << "Trwa zapisywanie...\n";
    if (curl) {
        string url = "https://v4.api.mganczarczyk.pl/v4/projekt-p2/";
        url += uuid;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        string body = generatePostBody(Player->getUsername(), Player->getLevel(), Player->getXP(), Player->getMoney(), Player->getWeapon()->getPrice(), Player->getArmor()->getPrice(), Player->getWeapon()->getCriticalChance(), Player->getWeapon()->getBaseDamage(), Player->getArmor()->getBaseProtection(), Player->getPotionsCount("smallPotion"), Player->getPotionsCount("bigPotion"));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::CharReaderBuilder builder{};
    auto reader = unique_ptr<Json::CharReader>(builder.newCharReader());
    const auto is_parsed = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.length(), &obj, &errors);
    system("cls");
    if (is_parsed) {
        cout << "Zapisano!\n";
    } else {
        cout << "Coś się zepsuło przy zapisywaniu. :/\n";
    }
    curl_global_cleanup();
    this_thread::sleep_for(chrono::milliseconds(1000));
}

// koniec zapisywania


double randomRangeDouble(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distr(min, max);
    return distr(gen);
}

int randomRange(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

double to2Decimal(double x) {
    return ceil(x * 100.0) / 100.0;
}

void gotoxy(short x, short y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void yourChoiceTemplate() {
    gotoxy(0, 25);
    cout << "+ ----------------------------------------------------------------------------------------------- +\n";
    cout << "|  Twój wybór | \n";
    cout << "+ ----------------------------------------------------------------------------------------------- +\n";
    gotoxy(98, 26);
    cout << "|";
    gotoxy(16, 26);
}

void gameEngine::play(settingsReader& sR, gameUtilities& gU) {
    int choice;
    while (1) {
        system("cls");
        cout << *getPlayer();
        gotoxy(0, 3);
        cout << "1. Idź na przygodę\n"
            << "2. Sprawdź ekwipunek\n"
            << "3. Idź do sklepu\n"
            << "4. Zapisz grę\n"
            << "5. Wyjdź z gry\n";

        yourChoiceTemplate();
        cin >> choice;
        gotoxy(0, 28);
        while (cin.fail() || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n|  To nie jest cyfra z zakresu 1-5, wpisz jeszcze raz | ";
            this_thread::sleep_for(chrono::milliseconds(500));
            cin >> choice;
        }
        switch (choice) {
        case 1:
        {
            bool isGameOnGoing = true;
            while (isGameOnGoing) {
                system("cls");
                cout << *getPlayer() << "\n";
                int msToAction = randomRange(1000, 5500);
                cout << "\nSzukasz wyzwania";
                for (int i = 0; i < 6; i++) {
                    cout << ".";
                    this_thread::sleep_for(chrono::milliseconds(msToAction / 6));
                }
                int randomAction = randomRange(0, 100);
                if (randomAction < 20) {
                    //potka, jeśli nie ma miejsca, to pieniądze 1$/2$
                    cout << "\nZnalazłeś potkę";
                    int poz;
                    for (int pozycja = 0; pozycja < 15; pozycja++) {
                        if (getPlayer()->getPotions()[pozycja] == NULL) {
                            poz = pozycja;
                            break;
                        }
                        if (pozycja == 14) {
                            poz = -1;
                        }
                    }
                    if (poz != -1) {
                        cout << "! ";
                        this_thread::sleep_for(chrono::milliseconds(500));
                        cout << "Przyjrzałeś się bliżej i jest to ";
                        if (randomAction % 3 == 0) {
                            gU.setColor(2);
                            cout << "DUŻA POTKA!";
                            gU.setColor();
                            bigPotion* bigPotka = new bigPotion;
                            getPlayer()->getPotions()[poz] = bigPotka;
                            //duża
                        } else {
                            gU.setColor(10);
                            cout << "MAŁA POTKA!";
                            gU.setColor();
                            smallPotion* smallPotka = new smallPotion;
                            getPlayer()->getPotions()[poz] = smallPotka;
                            //mała
                        }
                    } else {
                        cout << ", ale nie masz na nią miejsca w ekwpinku, więc dostaniesz rekompensatę w wysokości ";
                        if (randomAction % 3 == 0) {
                            cout << "2$";
                            getPlayer()->setMoney(getPlayer()->getMoney() + 2.0);
                            //duża
                        } else {
                            cout << "1$";
                            getPlayer()->setMoney(getPlayer()->getMoney() + 1.0);
                            //mała
                        }
                    }
                } else if (randomAction > 80) {
                    cout << "\n\n\nChęci były, ale nic nie znalazłeś.\n\nNie smuć się, dostaniesz rekompensatę w wysokości ";
                    gU.setColor(11);
                    int randomXP = randomRange(1, 6);
                    cout << randomXP << "XP!\n";
                    gU.setColor();
                    getPlayer()->addXP(randomXP, true);
                } else {
                    //bitka
                    gU.setColor(12);
                    cout << "\n\nNapotkałeś przeciwnika!\n";
                    gU.setColor();
                    enemy* przeciwnik = new enemy(to2Decimal(randomRangeDouble(10, 50)), to2Decimal(randomRangeDouble(1.25, 2.5)));
                    cout << "Jest to: " << przeciwnik->getName() << " " << *przeciwnik;
                    cout << "\n\nCo robisz?\n1) Walczę!\n2) Próbuję uciec (40% szansy)";
                    int innerChoice;
                    bool isRetreatPossible = randomRange(1, 10) <= 4;
                    yourChoiceTemplate();
                    cin >> innerChoice;
                    while (cin.fail() || innerChoice < 1 || innerChoice > 2) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "\n|  To nie jest cyfra z zakresu 1-2, wpisz jeszcze raz | ";
                        cin >> innerChoice;
                    }
                    gotoxy(0, 28);
                    if (innerChoice == 2) {
                        if (isRetreatPossible) {
                            system("cls");
                            cout << *getPlayer();
                            cout << "\n\nUdało ci się uciec!";
                        } else {
                            system("cls");
                            cout << *getPlayer();
                            cout << "\n\nNie udało ci się uciec!";
                        }
                        this_thread::sleep_for(chrono::milliseconds(1500));
                    }
                    if (innerChoice == 1 || (innerChoice == 2 && !isRetreatPossible)) {
                        //walka
                        bool yourTurn = true;
                        string innerGameChoice;
                        while (przeciwnik->isAlive()) {
                            system("cls");
                            cout << *getPlayer() << "\n\n"<< przeciwnik->getName() <<": " << *przeciwnik << "\n\n";
                            gU.setColor(2);
                            przeciwnik->twarz();
                            gU.setColor();
                            cout << "\n\n";

                            if (yourTurn) {
                                //akcja gracza
                                //templatka wyżej z możliwościami - potka mała, duża, atak, nic nie rób
                                gotoxy(0, 24);
                                gU.setColor(15);
                                cout << "M - mała potka; D - duża potka; W - walcz; N - nic nie rób";
                                gU.setColor();

                                yourChoiceTemplate();
                                cin >> innerGameChoice;
                                //sprawdz cin
                                bool isChecked = false;
                                while (!isChecked && (innerGameChoice == "m" || innerGameChoice == "M" || innerGameChoice == "d" || innerGameChoice == "D")) {
                                    //sprawdz czy potka istnieje
                                    if (getPlayer()->getPotionsCount(innerGameChoice == "m" || innerGameChoice == "M"?"smallPotion":"bigPotion")) {
                                        isChecked = true;
                                    } else {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cout << "\n|  Nie masz takiej potki, wpisz jeszcze raz swój wybór | ";
                                        cin >> innerGameChoice;
                                    }
                                }
                                while (cin.fail() || (innerGameChoice != "m" && innerGameChoice != "M" && innerGameChoice != "d" && innerGameChoice != "D" && innerGameChoice != "w" && innerGameChoice != "W" && innerGameChoice != "n" && innerGameChoice != "N")) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "\n|  To nie jest poprawna opcja, wpisz jeszcze raz | ";
                                    cin >> innerGameChoice;
                                }
                                gotoxy(0, 28);
                                if (innerGameChoice == "m" || innerGameChoice == "M" || innerGameChoice == "d" || innerGameChoice == "D") {
                                    getPlayer()->usePotion(innerGameChoice == "m" || innerGameChoice == "M" ? "smallPotion" : "bigPotion");
                                } else if (innerGameChoice == "w" || innerGameChoice == "W") {
                                    double base_temp = getPlayer()->getWeapon()->getBaseDamage();
                                    double damageInflicted = to2Decimal(randomRangeDouble((base_temp * 0.8), (base_temp * 1.2)));
                                    bool isCritical = (getPlayer()->getWeapon()->getCriticalChance() * 100) > randomRangeDouble(0, 100);
                                    if (isCritical)
                                        damageInflicted *= 2;
                                    cout << "Zaatakowałeś " << przeciwnik->getName() << ". Zadałeś mu " << damageInflicted << " obrażeń.";
                                    if (isCritical) {
                                        gU.setColor(6);
                                        cout << " Był to critical! (DMGx2)";
;                                       gU.setColor();
                                    }
                                    przeciwnik->setHealth(przeciwnik->getHealth() - damageInflicted);
                                    cout << " Zostało mu " << przeciwnik->getHealth() << "HP\n";
                                }
                                system("pause");
                                if (!przeciwnik->isAlive()) {
                                    gU.setColor(12);
                                    cout << "\nPokonałeś "<< przeciwnik->getName() <<"!\n";
                                    gU.setColor();
                                    double randomMoney = to2Decimal(randomRangeDouble(0.8, 2.5));
                                    int randomXP = randomRange(1, 10);
                                    cout << "Zdobyłeś: ";
                                    gU.setColor(11);
                                    cout << randomXP << "XP";
                                    gU.setColor();
                                    cout << ", ";
                                    gU.setColor(10);
                                    cout << randomMoney << "$\n";
                                    gU.setColor();
                                    getPlayer()->addXP(randomXP);
                                    getPlayer()->setMoney(getPlayer()->getMoney() + randomMoney);
                                }
                                yourTurn = !yourTurn;
                            } else {
                                //atak przeciwnika
                                double base_temp = przeciwnik->getDamage();
                                double damageInflicted = to2Decimal(randomRangeDouble((base_temp * 0.8), (base_temp * 1.2)));
                                damageInflicted = to2Decimal(damageInflicted * ((100.0 - getPlayer()->getArmor()->getBaseProtection()) / 100));
                                cout << "Zaatakował cię " << przeciwnik->getName() << ". Zadał ci " << damageInflicted << " obrażeń.";
                                getPlayer()->setHealth(getPlayer()->getHealth() - damageInflicted);
                                cout << " Zostało ci " << getPlayer()->getHealth() << "HP.\n";
                                system("pause");

                                if (getPlayer()->getHealth() <= 0) {
                                    system("cls");
                                    gU.setColor(12);
                                    cout << "PRZEGRAŁEŚ!\n\n";
                                    gU.setColor();
                                    cout << "Pokanał cię " << przeciwnik->getName() << *przeciwnik << "\n\n";
                                    gU.setColor(2);
                                    przeciwnik->twarz();
                                    gU.setColor();
                                    cout << "\n\n";
                                    system("pause");
                                }
                                yourTurn = !yourTurn;
                            }
                        }

                    }
                    //koniec walki/ucieczka
                    delete przeciwnik;
                    if (getPlayer()->getHealth() <= 0) {
                        gU.exitGame();
                        exit(0);
                    }
                }
                cout << "\n";
                system("pause && cls");
                cout << *getPlayer() << "\n\n"
                    << "Napotkani podróżni mówią ci, że wyglądasz na zmęczonego. Patrzysz na nich i odpowiadasz...\n\n"
                    << "1) Toż to nieprawda! Kontynuję podróż!\n2) OK, wracam.\n\n";
                int innerChoice;
                yourChoiceTemplate();
                cin >> innerChoice;
                while (cin.fail() || innerChoice < 1 || innerChoice > 2) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n|  To nie jest cyfra z zakresu 1-2, wpisz jeszcze raz | ";
                    cin >> innerChoice;
                }
                gotoxy(0, 28);
                if (innerChoice == 2)
                    isGameOnGoing = false;
            }
        }
        break;
        case 2:
        {
            system("cls");
            cout << *getPlayer() << "\n";

            //żyćko
            cout << "+ -- Życie -- +\n"
                << "  " << getPlayer()->getHealth() << "/" << getPlayer()->getMaxHealth() << "HP\n"
                << "+ ----------- +\n\n\n";

            //broń
            cout << "+ -- Broń -- +\n"
                << "  " << fixed << setprecision(2) << getPlayer()->getWeapon()->getBaseDamage() << " DMG\n"
                << "  " << fixed << setprecision(2) << getPlayer()->getWeapon()->getCriticalChance() * 100 << "[%] CRIT CHANCE\n"
                << "  " << fixed << setprecision(2) << getPlayer()->getWeapon()->getPrice() << "$\n"
                << "+ ---------- +\n\n\n";

            //armor
            cout << "+ -- Armor -- +\n"
                << "  " << fixed << setprecision(2) << getPlayer()->getArmor()->getBaseProtection() << " PROT\n"
                << "  " << fixed << setprecision(2) << getPlayer()->getArmor()->getPrice() << "$\n"
                << "+ ----------- +\n\n\n";

            //potki
            cout << "+ -- Potki -- +\n";
            int bP = 0, sP = 0;
            for (int i = 0; i < 15; i++) {
                if (getPlayer()->getPotions()[i] != NULL) {
                    if (getPlayer()->getPotions()[i]->getType() == "smallPotion") {
                        sP++;
                    } else {
                        bP++;
                    }
                }
            }
            cout << "  M: " << sP << ", D: " << bP << "\n"
                << "+ ----------- +\n\n\n";

            //dolary, złotówki, eurogąbki
            cout << "+ -- Pieniądze -- +\n"
                << "  " << getPlayer()->getMoney() << "$\n"
                << "+ --------------- +\n\n\n";

            system("pause");
        }
        break;
        case 3: {
            weaponBase bronie[] = {
                weaponBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(0.01, 0.05) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel())),
                weaponBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(0.01, 0.05) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel())),
                weaponBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(0.01, 0.05) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel()))
            };
            armorBase pancerze[] = {
                armorBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel())),
                armorBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel())),
                armorBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel()))
            };
            string powitania[8] = {
                    "Pokaż mi swoje towary...",
                    "Czego potrzebujesz?",
                    "Reklama dźwignią handlu, ale tu reklamy nie ma.",
                    "Zwrotów nie przyjmujemy.",
                    "Broń nie posiada atestów.",
                    "Dobrze, że nie widać, jaki bałagan w tym sklepie.",
                    "Programista płakał, jak pisał.",
                    "Gdzie lezie? Ma kase? Trzy tysiące dukatów albo spadaj!"
            };
            string powitanie = powitania[randomRange(0, 7)];
            boolean isShopOpen = true;
            while (isShopOpen) {
                system("cls");
                cout << *getPlayer();
                gotoxy(0, 4);
                gU.setColor(15);
                cout << powitanie << "\n";
                gU.setColor();
                gotoxy(0, 8);
                cout << "+ ------------------------------------------- ";
                gU.setColor(12);
                cout << "BRONIE";
                gU.setColor();
                cout << " -------------------------------------------- + \n";
                cout << "|      OBR, KRYT (%), CENA\n";
                gotoxy(98, 9);
                cout << "|";
                for (int i = 0; i < 3; i++) {
                    gotoxy(0, 10 + i);
                    cout << "| ";
                    gU.setColor(9);
                    cout << "B" << i << ". ";
                    gU.setColor(7);
                    cout << fixed << setprecision(2) << bronie[i].getBaseDamage() << ",     " << setprecision(2) << (bronie[i].getCriticalChance() * 100) << ", ";
                    gU.setColor(2);
                    cout << fixed << setprecision(2) << bronie[i].getPrice() << "$\n";
                    gU.setColor();
                    gotoxy(98, 10 + i);
                    cout << "|";
                }
                gotoxy(0, 13);
                cout << "+ ----------------------------------------------------------------------------------------------- + ";
                cout << "\n\n";
                cout << "+ ------------------------------------------ ";
                gU.setColor(12);
                cout << "PANCERZE";
                gU.setColor();
                cout << " ------------------------------------------- + \n";
                cout << "|     PROT,  CENA\n";
                gotoxy(98, 16);
                cout << "|";
                for (int i = 0; i < 3; i++) {
                    gotoxy(0, 17 + i);
                    cout << "| ";
                    gU.setColor(9);
                    cout << "P" << i << ". ";
                    gU.setColor(7);
                    cout << fixed << setprecision(2) << pancerze[i].getBaseProtection() << ", ";
                    gU.setColor(2);
                    cout << fixed << setprecision(2) << pancerze[i].getPrice() << "$\n";
                    gU.setColor();
                    gotoxy(98, 17 + i);
                    cout << "|";
                }
                gotoxy(0, 20);

                cout << "+ ----------------------------------------------------------------------------------------------- +\n\n\n\n";

                gU.setColor(15);
                cout << "W - wyjdź; B0,B1,B2,P0,P1,P2 - kup odpowiednią broń/pancerz, O - odśwież ofertę sklepu";
                gU.setColor();
                string innerChoice;
                yourChoiceTemplate();
                cin >> innerChoice;
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> innerChoice;
                }
                gotoxy(0, 28);
                if (innerChoice == "O" || innerChoice == "o" || innerChoice == "0") {
                    for (int i = 0; i < 3; i++) {
                        bronie[i] = weaponBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(0.01, 0.05) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel()));
                        pancerze[i] = armorBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel()));
                    }
                } else if (innerChoice == "W" || innerChoice == "w") {
                    isShopOpen = false;
                } else {
                    //kup jeśli ma pieniądze, daj zwrot 20% za aktualnie posiadaną
                    if (innerChoice.length() == 2) {
                        char fLetter = innerChoice[0];
                        int number = innerChoice[1] - '0';
                        if (number >= 0 && number <= 2) {
                            if (fLetter == 'P' || fLetter == 'p') { //pancerz
                                if (getPlayer()->getMoney() >= pancerze[number].getPrice()) { //czy stać
                                    getPlayer()->setMoney(getPlayer()->getMoney() - pancerze[number].getPrice() + to2Decimal(getPlayer()->getArmor()->getPrice() * 0.2));
                                    getPlayer()->setArmor(new armorBase(pancerze[number].getBaseProtection(), pancerze[number].getPrice()));
                                    for (int i = 0; i < 3; i++) {
                                        pancerze[i] = armorBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel()));
                                    }
                                    cout << "Pomyślnie zakupiono pancerz P" << number << "! Nie zapomnij zapisać gry :)\n";
                                } else {
                                    cout << "Nie masz wystarczających środków na zakup pancerza P" << number << "!\n";
                                }
                            } else if (fLetter == 'B' || fLetter == 'b') { //broń
                                if (getPlayer()->getMoney() >= bronie[number].getPrice()) { //czy stać
                                    getPlayer()->setMoney(getPlayer()->getMoney() - bronie[number].getPrice() + to2Decimal(getPlayer()->getArmor()->getPrice() * 0.2));
                                    getPlayer()->setWeapon(new weaponBase(bronie[number].getBaseDamage(), bronie[number].getCriticalChance(), bronie[number].getPrice()));
                                    for (int i = 0; i < 3; i++) {
                                        bronie[i] = weaponBase(to2Decimal(randomRangeDouble(1.0, 3.0) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(0.01, 0.05) * getPlayer()->getLevel()), to2Decimal(randomRangeDouble(3.0, 25.0) * getPlayer()->getLevel()));
                                    }
                                    cout << "Pomyślnie zakupiono broń B" << number << "! Nie zapomnij zapisać gry :)\n";
                                } else {
                                    cout << "Nie masz wystarczających środków na zakup broni B" << number << "!\n";
                                }
                            }
                        }

                    }
                    this_thread::sleep_for(chrono::milliseconds(1000));
                }
            }
        } break;
        case 4:
            saveGame(sR.getUUID());
            break;
        case 5:
            gU.exitGame();
            exit(0);
            break;
        }
    }
}
