#include <iostream>
#include <ios>
#include "GameDatabase.h"

#include "SocketServer.h"

GameDatabase* GameDatabase::_instance;

int main(int* argc, int ** args) {
	
	//Start the server.
	SocketServer server(80, "127.0.0.1");

	//TODO: Create instance of the socket server.
	//TODO: load XML documents to take text.
	//TODO: Create instance of SQLite to manage data?

	GameDatabase::instance();
	std::cin.get();
	return EXIT_SUCCESS;
}