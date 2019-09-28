#pragma once
#include "ClientRequest.h"

#include <vector>

//Takes a client request and a client, stores it then processes them in a queue returning the result to the client
class ClientRequestManager {

private:
	std::vector<ClientRequest> _clientRequests;
};