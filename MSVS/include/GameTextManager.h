#pragma once
#include <string>

//This class retrives and sets text stored in XML files via a key index system.
class GametextManager {
public:
	static GametextManager* instance() {
		if (!_instance) {
			_instance = new GametextManager();
		}
		return _instance;
	}

	static std::string getText(const int& index) {

	}
private:
	static GametextManager* _instance;
};