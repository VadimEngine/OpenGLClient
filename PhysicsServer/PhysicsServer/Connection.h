#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include "Particle.h"
#include "Handler.h"

class Connection {
	SOCKET sock, client;
	//Handler here for now
	Handler* handler;
	bool TCP;

public:
	Connection(Handler* handler, bool TCP);

	void init();

	bool listenForClient();

	void close();

};
