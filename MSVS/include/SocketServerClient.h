#pragma once
#include "clsocket\PassiveSocket.h"
#include <thread>
#include <mutex>
#include "ClientRequestManager.h"
#include "ResponceHeadder.h"

#define MAX_PACKET 4096

class SocketServerClient {
public:
	SocketServerClient(SimpleSocket::CActiveSocket* clientsock) {
		//std::cout << "Client " << clientsock->GetClientAddr() << " connected" << std::endl;
		_clientsock = clientsock;
		start();
	}

	~SocketServerClient(){
		if (running()) {
			stop();
			_clientsock->Close();
			delete _clientsock;
		}
	}

	void start() {
		_running = true;
		_listener = std::thread(&SocketServerClient::threaded, this);
	}

	void threaded() {
		while (running()) {
			loop();
		}
	}

	void loop() {
		if (_clientsock->Receive(MAX_PACKET)){
			uint8_t* data = _clientsock->GetData();
			int32 size = _clientsock->GetBytesReceived();
			std::string str;
			str.reserve(size);
			str.assign((char*)data, size);
			ClientRequest request(str);
			std::cout << "Client " << _clientsock->GetClientAddr() << " requested " << request.getTarget() << std::endl;
			ResponceHeadder res = ClientRequestManager::processClientRequest(request);
			sendData(res.outPut());
			_clientsock->Close();
		}
		else {
			//std::cout << "End";
			_running = false;
		}
	}
	bool sendData(const std::string& data) {
		return sendData(data.c_str(), data.length());
	}

	bool sendData(const char* data, uint16_t size) {
		_clientsock->Send((uint8_t*)data, size);
		return true;
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
	SimpleSocket::CActiveSocket* _clientsock;
	std::thread _listener;
	std::mutex _runningmutex;
	bool _running;
};