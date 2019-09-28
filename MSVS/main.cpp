#include <iostream>
#include <ios>
#include "GameDatabase.h"

GameDatabase* GameDatabase::_instance;

int main(int* argc, int ** args) {
	
	//TODO: Create instance of the socket server.
	//TODO: load XML documents to take text.
	//TODO: Create instance of SQLite to manage data?

	GameDatabase::instance();
	std::cin.get();
	return EXIT_SUCCESS;
}