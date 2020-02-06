#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include "../Physics/Particle.h"
#include "../Physics/Handler.h"

/*
Class that establishes TCP connections between this server and 
mutliple clients. Creates a daemon thread for new clients
*/
class ConnectionTCP {
	SOCKET sock, client;
	//Handler here for now
	Handler* handler;
	bool TCP;

public:
	/*
	Constructor that takes in the handler which this connection will read/update depending
	on clients' messages
	*/
	ConnectionTCP(Handler* handler, bool TCP);

	/* 
	Initilizes the connection so a connection can be established
	*/
	void init();

	/*
	Listens for clients and if one is found then a new thread for the client
	is created to listen/send further messages
	*/
	bool listenForClient();

	/*
	Close client and delete related objects
	*/
	void close();

};
