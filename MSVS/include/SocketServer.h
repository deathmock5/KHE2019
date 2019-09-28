#pragma once
#include <thread>
#include <mutex>
#include <vector>

#include "SocketServerClient.h"
#include "clsocket\PassiveSocket.h"

//This Factory, listens and creates clients.
class SocketServer {
public:
	SocketServer(const int& port, const char* ip){
		_socket.Initialize();
		_socket.Listen(ip, port);
		start();
	}

	void start() {
		_running = true;
		_listener = std::thread(&SocketServer::threaded,this);
	}

	void threaded() {
		while (running()) {
			loop();
		}
	}

	void loop() {
		SimpleSocket::CActiveSocket *pClient = NULL;
		if ((pClient = _socket.Accept()) != NULL) {
			_clients.push_back(std::make_shared<SocketServerClient>(pClient));
		}
	}

	void stop() {
		{
			std::lock_guard<std::mutex> temp(_runningmutex);
			_running = false;
		}
		_listener.join();
	}

	bool running() {
		std::lock_guard<std::mutex> temp(_runningmutex);
		return _running;
	}

private:
	std::thread _listener;
	bool _running;
	//running mutex;
	std::mutex _runningmutex;
	SimpleSocket::CPassiveSocket _socket;
	std::vector<std::shared_ptr<SocketServerClient>> _clients;
};