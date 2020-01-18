#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include "Particle.h"
#include "Handler.h"


class ConnectionUDP {
public:

	Handler* handler;

	ConnectionUDP(Handler* handler);

	void init();

	bool listenForClient();

	void close();


};
