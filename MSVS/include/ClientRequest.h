#pragma once
#include "SocketServerClient.h"

class ClientRequest {

private:
	SocketServerClient* _client;
	std::string _request;
};