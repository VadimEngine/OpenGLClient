#pragma once
#include "../Physics/Handler.h"
#include <WS2tcpip.h>
#include <thread>
#include "ConnectionConstants.h"

class ConnectionTCP {
public:

	SOCKET sock, client;

	ConnectionTCP();

	void init();

	void communicate(Handler* handler);

	//send to who? pass in client info?
	void sendData(void* data, int size);

	void getData(void* data, int& size);

	void close();
};