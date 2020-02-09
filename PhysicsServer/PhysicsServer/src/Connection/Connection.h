#pragma once
#include <iostream>
#include "ConnectionTCP.h"
#include "ConnectionUDP.h"
#include "../Physics/Handler.h";

enum ConnectionMode { TCP, UDP };


class Connection {
public:
	
	ConnectionMode theMode;

	ConnectionTCP conTCP;

	ConnectionUDP conUDP;

	Handler* handler;

	Connection(Handler* handler);

	void init();

	void connectProtocol();

	void close();

	void communicate();

	void sendData(void* data, int size);

	void getData(void* data, int& size);

};
