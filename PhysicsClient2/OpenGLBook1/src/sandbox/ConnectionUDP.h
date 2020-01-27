#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "SandboxWindow.h"


class ConnectionUDP {
public:

	SandboxWindow* window;

	sockaddr_in UDPserver;
	SOCKET UDPout;
	int userId;

	ConnectionUDP();

	void setWindow(SandboxWindow* myWindow);

	void UDPInit();

	int UDPConnect();

	void UDPListen();

	void UDPSend();

	void UDPSend(char* data, int size);

	void UDPClose();

};