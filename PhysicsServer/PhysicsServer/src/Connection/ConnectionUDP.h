#pragma once
#include "../Physics/Handler.h"
#include <unordered_map>
#include <WS2tcpip.h>

class ConnectionUDP {

public:

	Handler* handler;
	std::unordered_map<std::string, Particle*> clientMap;
	SOCKET in;

	ConnectionUDP();

	void init();

	void communicate(Handler* handler);

	//send to who? pass in client info?
	void sendData(void* data, int size);

	void getData(void* data, int& size);

	void close();
};