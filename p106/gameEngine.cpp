#include "gameEngine.h"
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <iomanip>
#include <Windows.h>

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
    //system("cls");
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
    //system("cls");
    if (is_parsed) {
        
    } else {
        cout << "Coœ siê zepsu³o przy zapisywaniu. :/";
    }
    curl_global_cleanup();
    system("pause");
}

// koniec zapisywania

void gameEngine::loadGame(const std::string& uuid) {
    //
}
