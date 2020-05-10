#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "ConnectionConstants.h"
//#include "../GUI/Window.h"

/// <summary>
/// Class to handle UDP connection to server. Only works on local host
/// for now. User must select a userId (1-9) in order to connect
/// and differentiate from other users on this IP.
/// </summary>
/// <remarks>
/// Need to define the constants used to differentiate
/// what the send/receive data represents
/// </remarks>
class ConnectionUDP {
private:
	/// <summary>
	/// Socket address of the server
	/// </summary>
	sockaddr_in UDPserver;

	/// <summary>
	/// Port number of the serer.
	/// </summary>
	SOCKET UDPout;

	/// <summary>
	/// User id used to connect. Can only be 1-9 for now.
	/// </summary>
	int userId;

	std::string serverName;
	
public:
	/// <summary>
	/// Empty Constuctor
	/// </summary>
	ConnectionUDP();

	~ConnectionUDP();

	/// <summary>
	/// Inital connection to UDP server. Temporaily only connects
	/// to server on olocal host.
	/// </summary>
	int UDPConnect(int port, std::string ipAddress);

	/// <summary>
	/// Gets data from the server. Fills the float array to
	/// index size-1
	/// </summary>
	void UDPGetData(void* data, int& size);

	/// <summary>
	/// Send data to the UDP server to update accordingly
	/// </summary>
	void UDPSend(void* data, int size);

	/// <summary>
	/// Close winsock and socket.
	/// </summary>
	void UDPClose();

	void setUserID(int id);

	int getUserId();

	std::string getServerName();
};