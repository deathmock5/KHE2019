#pragma once
#include "sqlite\sqlite.h"

//This class contains the SQLite DB instance, and is used to process transactions between the threads
class GameDatabase {
public:
	static GameDatabase* instance() {
		if (!_instance) {
			_instance = new GameDatabase();
		}
		return _instance;
	}

private:

	GameDatabase() {
		db = sqlite::database::type_ptr(new sqlite::database::type("mydb.db"));
		if (db->result_code() == SQLITE_OK) {
			std::cout << "DATABASE OK";
		}

	}

	//Static variables
	static GameDatabase* _instance;

	//instance memebers
	bool createTable(const std::string& tablename) {
		//TODO: create table
		return true;
	}

	bool insert(const std::string& line) {
		//TODO: insert
		return true;
	}

	//Instance Variables
	sqlite::database::type_ptr db;
};