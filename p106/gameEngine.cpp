#include "gameEngine.h"
#include "gameUtilities.h"
#include "settingsReader.h"
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
    Player = new player("", 0, 0, 0, 0, 0, 0, 0, 0);
    CURL* curl;
    CURLcode res;
    string readBuffer, errors{};
    Json::Value obj;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    system("cls");
    cout << "Rozpoczynam synchronizacjê z serwerem...\n";
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
        if (obj.isMember("error")) {
            cout << "[B£¥D SERWERA] Nie uda³o siê wczytaæ postêpów. Zamknij aplikacjê jeœli nie chcesz utraciæ postêpów, w przciwnym razie konto zostanie nadpisane profilem nowej gry.";
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
            Player = new player(
                username,
                level,
                xp,
                money,
                weapon_baseDamage,
                weapon_critChance,
                weapon_price,
                armor_baseProtection,
                armor_price
            );
        }
    } else {
        cout << "Nie uda³o siê wczytaæ postêpów. Zamknij aplikacjê jeœli nie chcesz utraciæ postêpów, w przciwnym razie konto zostanie nadpisane profilem nowej gry.";
        system("pause");
        delete Player;
        Player = new player;
    }
}

// zapsiywanie

string generatePostBody(string imie, int level, int xp, double money, double weapon_price, double armor_price, double weapon_critChance, double weapon_baseDamage, double armor_baseProtection) {
    string str = "username=" + imie
        + "&level=" + to_string(level)
        + "&xp=" + to_string(xp)
        + "&money=" + to_string(money)
        + "&weapon_price=" + to_string(weapon_price)
        + "&armor_price=" + to_string(armor_price)
        + "&weapon_critChance=" + to_string(weapon_critChance)
        + "&weapon_baseDamage=" + to_string(weapon_baseDamage)
        + "&armor_baseProtection=" + to_string(armor_baseProtection);
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
        string body = generatePostBody(Player->getUsername(), Player->getLevel(), Player->getXP(), Player->getMoney(), Player->getWeapon()->getPrice(), Player->getArmor()->getPrice(), Player->getWeapon()->getCriticalChance(), Player->getWeapon()->getBaseDamage(), Player->getArmor()->getBaseProtection());
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
        cout << "Coœ siê zepsu³o przy zapisywaniu. :/\n";
    }
    curl_global_cleanup();
    this_thread::sleep_for(chrono::milliseconds(1000));
}

// koniec zapisywania

int randomRange(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

void gotoxy(short x, short y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void yourChoiceTemplate() {
    gotoxy(0, 20);
    cout << "+ ----------------------------------------------------------------------------------------------- +\n";
    cout << "|  Twój wybór | \n";
    cout << "+ ----------------------------------------------------------------------------------------------- +\n";
    gotoxy(98, 21);
    cout << "|";
    gotoxy(16, 21);
}

void gameEngine::play(settingsReader& sR, gameUtilities& gU) {
    boolean stopMainLoop = false;
    int choice;
    while (!stopMainLoop) {
        system("cls");
        cout << *getPlayer();
        gotoxy(0, 3);
        cout << "1. IdŸ na przygodê\n"
             << "2. SprawdŸ ekwipunek\n"
             << "3. IdŸ do sklepu\n"
             << "4. Zapisz grê\n"
             << "5. WyjdŸ z gry\n";

        yourChoiceTemplate();
        cin >> choice;
        gotoxy(0, 23);
        while (cin.fail() || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n|  To nie jest cyfra z zakresu 1-5, wpisz jeszcze raz | ";
            this_thread::sleep_for(chrono::milliseconds(500));
            cin >> choice;
        }
        switch (choice) {
        case 1:
        case 2:
            cout << "\nJeszcze niezrobione. :<\n";
            system("pause");
            break;
        case 3: {
            system("cls");
            cout << *getPlayer();
            gotoxy(0, 3);
            gU.setColor(15);
            string powitania[8] = {
                "Poka¿ mi swoje towary...",
                "Czego potrzebujesz?",
                "Reklama dŸwigni¹ handlu, tu reklamy nie ma",
                "Zwrotów nie przyjmujemy",
                "Broñ nie posiada atestów",
                "Dobrze, ¿e nie widaæ, jaki ba³agan w tym sklepie",
                "Programista p³aka³, jak pisa³",
                "Gdzie lezie? Ma kase? Trzy tysi¹ce dukatów albo spadaj!"
            };
            cout << powitania[randomRange(0, 7)] << "\n";
            gU.setColor();

            gotoxy(0, 8);
            cout << "+ ------------------------------------------- ";
            gU.setColor(12);
            cout << "BRONIE";
            gU.setColor();
            cout << " -------------------------------------------- + \n";
            cout << "\n"; //content
            cout << "+ ----------------------------------------------------------------------------------------------- + ";
            
            cout << "\n\n";
            
            cout << "+ ------------------------------------------ ";
            gU.setColor(12);
            cout << "PANCERZE";
            gU.setColor();
            cout << " ------------------------------------------- +\n";
            cout << "\n"; //content
            cout << "+ ----------------------------------------------------------------------------------------------- + ";

            string innerChoice;
            yourChoiceTemplate();
            cin >> innerChoice;
            gotoxy(0, 23);
            system("pause");
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
