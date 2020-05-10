#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "ConnectionConstants.h"
//#include "../GUI/Window.h"


/// <summary> 
/// Class that handles TCP communication between the server and this client
/// </summary> 
class ConnectionTCP {
private:
	/// <summary> 
	/// Sock. TODO: look into what it does
	/// </summary> 
	SOCKET sock;

	//int port; //Listening port number on the server
	//std::string ipAddress;

	/// <summary> 
	/// Default constuctor
	/// </summary> 
public:
	ConnectionTCP();

	ConnectionTCP(int port, std::string ipAddress);

	~ConnectionTCP();
	/// <summary>
	/// Inital connection to the server. Temporarily only connects to 
	/// server on local host
	/// </summary>
	bool TCPConnect(int port, std::string ipAddress);

	/// <summary
	/// Method to retreive data from the server. fills the
	/// float array up to index size-1.
	/// </summary>
	void TCPGetData(void* data, int& size);

	/// <summary> 
	/// Send data to the server.
	/// </summary>
	void TCPsendData(void* data, int size);//might need size. TCP?

	/// <summary> 
	/// Close the Connection
	/// </summary> 
	void TCPclose();
};