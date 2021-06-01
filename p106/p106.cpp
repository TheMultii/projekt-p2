#include <iostream>
#include <ctime>
#include <string>
#include "settingsReader.h"

using namespace std;

int main() {
	settingsReader settings;
	cout << "Fajna gra | " << settings.getAPIVersion();
}