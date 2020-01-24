#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <unordered_map>

#include "Particle.h"
#include "Handler.h"


class ConnectionUDP {
public:

	Handler* handler;
	std::unordered_map<std::string, Particle*> clientMap;

	SOCKET in;

	ConnectionUDP(Handler* handler);

	void init();

	void listen();

	void close();


};
