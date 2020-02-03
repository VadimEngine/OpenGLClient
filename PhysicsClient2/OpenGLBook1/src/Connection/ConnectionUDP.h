#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "../GUI/Window.h"

/// <summary>
/// Class to handle UDP connection to server. Only works on local host
/// for now.
/// </summary>
class ConnectionUDP {
public:

	/// <summary>
	/// Window that will be updated by the server
	/// </summary>
	Window* window;

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

	/// <summary>
	/// Empty Constuctor
	/// </summary>
	ConnectionUDP();

	/// <summary>
	/// Sets the window that will be updated by this connection
	/// </summary>
	void setWindow(Window* myWindow);

	/// <summary>
	/// Inital connection to UDP server. Temporaily only connects
	/// to server on olocal host.
	/// </summary>
	int UDPConnect();

	/// <summary>
	/// Listens to UDP server and updates the client data accordingly.
	/// Only listens for server particle coordinates that will be rendered.
	/// </summary>
	void UDPListen();

	/// <summary>
	/// Send data to the UDP server to update accordingly
	/// </summary>
	void UDPSend(char* data, int size);

	/// <summary>
	/// Close winsock and socket.
	/// </summary>
	void UDPClose();

};