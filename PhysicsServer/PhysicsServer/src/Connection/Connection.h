#pragma once
#include <iostream>
#include "ConnectionTCP.h"
#include "ConnectionUDP.h"
#include "../Physics/Handler.h";


/// <summary>
/// The possible server mode options.
/// </summary>
enum ConnectionMode { TCP, UDP };

/// <summary>
/// Class to handler communications with clients.
/// takes in a hander which will be updated by data from
/// clients. User selects if the communications will be
/// TCP or UDP
/// </sumamry>
class Connection {
public:
	
	/// <summary>
	/// Which server mode the server-client will be
	/// </summary>
	ConnectionMode theMode;

	/// <summary>
	/// Object to handle TCP communication
	/// </summary>
	ConnectionTCP conTCP;

	/// <summary>
	/// Object to handler UDP communication
	/// </summary>
	ConnectionUDP conUDP;

	/// <summary>
	/// The Handler that will hold data that will be updated/sent
	/// between server-client communication
	/// </summary>
	Handler* handler;

	Connection(Handler* handler);

	void init();

	void connectProtocol();

	void close();

	void communicate();

	void sendData(void* data, int size);

	void getData(void* data, int& size);

};
