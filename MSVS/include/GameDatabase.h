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
	static std::string getPage(const char* pagename) {
		return instance()->doQuery(std::string("SELECT `pagetext` FROM 'pages' WHERE `pindex`='") + pagename + "';").get<std::string>(0);
	}
	static std::vector<std::pair<std::string, std::string>> getButtons(const char* pagename) {
		std::vector<std::pair<std::string, std::string>> output;
		sqlite::query q = instance()->doQuery(std::string("SELECT * FROM 'buttons' WHERE `bindex`='") + pagename + "';");
		while (q.result_code() != SQLITE_DONE)
		{
			output.push_back(std::pair <std::string, std::string>(q.get<std::string>(1), q.get<std::string>(2)));
			q.step();
		}
		return output;
	}
private:

	GameDatabase() {
		db = sqlite::database::type_ptr(new sqlite::database::type("mydb.db"));
		if (db->result_code() == SQLITE_OK) {
			std::cout << "DATABASE OK" << std::endl;
		}
		if (createTable("CREATE TABLE `pages` (`pindex` TEXT,`pagetext` TEXT);")) {
			std::cout << "Created 'pages' table" << std::endl;
			if (insert("INSERT INTO `pages` (`pindex`, `pagetext`) VALUES ('', 'welcome to the world');")) {
				std::cout << "Created \ page" <<std::endl;
			}
		}
		if (createTable("CREATE TABLE `buttons` (`bindex` TEXT,`buttonname` TEXT,`buttonaction` TEXT);")) {
			std::cout << "Created 'buttons' table" << std::endl;
			if (insert("INSERT INTO `buttons` (`bindex`, `buttonname`, `buttonaction`) VALUES ('', 'proceede', 'proceede');")) {
				std::cout << "Created \ page" << std::endl;
			}
		}
	}

	//Static variables
	static GameDatabase* _instance;

	//instance memebers
	bool createTable(const std::string& tablename) {
		sqlite::query create_table(db, tablename);
		create_table.step();
		return create_table.result_code() == SQLITE_DONE;
	}

	bool insert(const std::string& line) {
		sqlite::query insert(db, line);
		insert.step();
		return insert.result_code() == SQLITE_DONE;
	}

	sqlite::query doQuery(const std::string& query) {
		sqlite::query select(db, query);
		select.step();
		return select;
	}

	//Instance Variables
	sqlite::database::type_ptr db;
};