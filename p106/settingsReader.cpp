#include "settingsReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <regex>
#include <ctime>
#include <random>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <json/json.h>
#include <curl/curl.h>
#include <locale.h>

using namespace std;

string gen_random(const int& len) {
	string str("0123456789abcdef");

	random_device rd;
	mt19937 generator(rd());

	shuffle(str.begin(), str.end(), generator);

	return str.substr(0, len);  
}

string get_uuid() {
	//generator UUID v4, nawet przechodzi walidację(!)
	string res;
	char temp;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distr(0, 3);
	switch (distr(gen)) {
		case 0:
			temp = '8'; break;
		case 1:
			temp = '9'; break;
		case 2:
			temp = 'a'; break;
		case 3:
		default:
			temp = 'b'; break;
	}
	res = gen_random(8) + "-" + gen_random(4) + "-4" + gen_random(3) + "-" + temp + gen_random(3) + "-" + gen_random(12);
	return res;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

bool sync_server(Json::Value& obj) {
	CURL* curl;
	CURLcode res;
	string readBuffer;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://v4.api.mganczarczyk.pl/v4");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	Json::CharReaderBuilder builder{};
	auto reader = unique_ptr<Json::CharReader>(builder.newCharReader());
	string errors{};
	const auto is_parsed = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.length(), &obj, &errors);
	return is_parsed;
}

settingsReader::settingsReader() {
	setlocale(LC_CTYPE, "Polish");
	system("cls");
	cout << "Trwa wczytywanie...";
	ifstream settingsFile, linecount;
	settingsFile.open(".settings"); 
	linecount.open(".settings");
	if (settingsFile.is_open()) {
		if (count(istreambuf_iterator<char>(linecount), istreambuf_iterator<char>(), '\n')) {
			//usuń, stwórz
			fstream ofs;
			ofs.open(".settings", ios::out | ios::trunc);
			uuid = get_uuid();
			ofs << uuid;
			ofs.close();
		} else {
			string line;
			while (getline(settingsFile, line)) {
				if (regex_match(line, regex("[a-f0-9]{8}-?[a-f0-9]{4}-?4[a-f0-9]{3}-?[89ab][a-f0-9]{3}-?[a-f0-9]{12}"))) {
					//regex przeszedł, string to uuid v4
					uuid = line;
				} else {
					// ciąg znaków to nie uuid v4, użytkownik się bawił z plikiem
					uuid = get_uuid();
					ofstream settingsFileSave;
					settingsFileSave.open(".settings");
					settingsFileSave << uuid;
					settingsFileSave.close();
				}
			}
		}
	} else {
		// nie ma takiego pliku, stwórz nowy
		uuid = get_uuid();
		ofstream settingsFileSave;
		settingsFileSave.open(".settings");
		settingsFileSave << uuid;
		settingsFileSave.close();
		settingsFile.close();
	}
	settingsFile.close();
	linecount.close();
	//this_thread::sleep_for(chrono::milliseconds(1250)); //użytkownik pomyśli, że bardzo skomplikowana aplikacja się ładuje
	system("cls");
	cout << "Rozpoczynam synchronizację z serwerem...\n";
	Json::Value obj{};
	if (sync_server(obj)) {
		system("cls");
		v4Base = obj;
		cout << "Pomyślnie zsynchronizowano z serwerem.\nWersja API, z której korzystam: " << this->getAPIVersion();
		string temp = "title Fajna gra - ";
		temp.append(this->getAPIVersion());
		system(temp.c_str());
	} else {
		system("cls");
		cout << "Nie udało się zsynchronizować danych z serwerem. :(";
	}
	this_thread::sleep_for(chrono::milliseconds(1250));
	system("cls");
}