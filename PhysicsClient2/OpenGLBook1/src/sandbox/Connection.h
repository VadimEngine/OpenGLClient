#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "SandboxWindow.h"

//REMOVE ALL REDUNDANT UNECESSARY METHODS
class Connection {
public:
	
	SandboxWindow* window;
	//TCP
	SOCKET sock;
	//UDP
	sockaddr_in UDPserver;
	SOCKET UDPout;

	int userId;

	Connection();

	void setWindow(SandboxWindow* myWindow);


	//TCP
	bool TCPConnect();

	void TCPlisten();

	void TCPsendData(void* data, int size);//might need size. TCP?

	void TCPclose();

	//UDP

	void UDPInit();

	bool UDPConnect();

	void UDPListen();

	void UDPSend();

	void UDPSend(char* data, int size);

	void UDPClose();


	//UDP

};