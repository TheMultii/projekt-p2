#pragma once
#include <string>
#include <json/json.h>

using namespace std;

class settingsReader {
	string uuid;
	Json::Value v4Base;
public:
	settingsReader();

	string getUUID() {
		return uuid;
	}

	string getAPIVersion() {
		if (v4Base != NULL) {
			return v4Base["api_version"].asString();
		}
		return ">ERROR<";
	}
};