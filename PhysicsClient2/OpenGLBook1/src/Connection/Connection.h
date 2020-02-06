#pragma once
#include "ConnectionTCP.h"
#include "ConnectionUDP.h"
#include <string>

enum ConnectionMode {Serverless, TCP, UDP};


/// <summary>
/// Class to handler all connection logic
/// </summary>
class Connection {

public:

	//ConnectionMode theMode;

	int theModeInt;

	bool connected;

	//TCP
	SOCKET sock;
	ConnectionTCP TCPCon;

	//UDP
	ConnectionUDP UDPCon;
	sockaddr_in UDPserver;
	SOCKET UDPout;
	int UDPUserId;

	Connection();

	void connectionProtocol();

	int connect();

	void sendData(void* data, int size);

	void getData(void* data, int& size);

	void close();

};
