#pragma once
#include <thread>
#include <mutex>

#include "clsocket\PassiveSocket.h"

//This Factory, listens and creates clients.
class SocketServer {
public:
	SocketServer(const int& port, const char* ip) : _ip(ip), _port(port){
		_socket.Initialize();
		_socket.Listen(ip, port);
		start();
	}

	void start() {
		_listener = std::thread(&SocketServer::listen,this);
	}

	void listen() {
		SimpleSocket::CActiveSocket *pClient = NULL;
		while (running()) {
			if ((pClient = _socket.Accept()) != NULL) {
				//TODO: Create client.
			}
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
	const int _port;
	const char* _ip;
	std::thread _listener;
	bool _running;
	//running mutex;
	std::mutex _runningmutex;
	SimpleSocket::CPassiveSocket _socket;
};