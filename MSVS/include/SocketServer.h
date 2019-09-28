#pragma once
#include <thread>
#include <mutex>

#include "clsocket\ActiveSocket.h"

class SocketServer {
public:
	SocketServer(const int& port, const char* ip) : _ip(ip), _port(port){
		//TODO: thread out socket server lsitener to create
	}

	void start() {
		_listener = std::thread(&SocketServer::listen,this);
	}

	void listen() {
		while (running()) {
			//TODO: Listen for client blocking with timout
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
};