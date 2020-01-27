#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "SandboxWindow.h"

//REMOVE ALL REDUNDANT UNECESSARY METHODS
class ConnectionTCP {
public:
	
	SandboxWindow* window;
	//TCP
	SOCKET sock;

	ConnectionTCP();

	void setWindow(SandboxWindow* myWindow);


	//TCP
	bool TCPConnect();

	void TCPlisten();

	void TCPsendData(void* data, int size);//might need size. TCP?

	void TCPclose();
};