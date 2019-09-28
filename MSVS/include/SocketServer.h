#pragma once
#include "ActiveSocket.h"


class SocketServer {
public:
	SocketServer(const int& port, const char* ip) : _ip(ip), _port(port){
		//TODO: thread out socket server lsitener to create
	}


private:
	const int _port;
	const char* _ip;
};