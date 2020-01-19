#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "SandboxWindow.h"

class Connection {
public:

	SOCKET sock;
	SandboxWindow* window;

	sockaddr_in UDPserver;
	SOCKET UDPout;



	Connection();

	void setWindow(SandboxWindow* myWindow);

	bool connectionProtocol();

	void listen();

	void sendData(void* data, int size);//might need size

	void close();

	void UDPInit();

	void UDPListen();

	void UDPSend();

	void UDPClose();

	void UDP();

};