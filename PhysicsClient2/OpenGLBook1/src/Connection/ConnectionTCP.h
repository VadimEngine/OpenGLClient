#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include "../GUI/Window.h"


/// <summary> 
/// Class that handles TCP communication between the server and this client
/// </summary> 
class ConnectionTCP {
public:
	
	/// <summary> 
	/// Window that displays the data sent between server and client
	/// </summary> 
	Window* window;
	
	/// <summary> 
	/// Sock. TODO: look into what it does
	/// </summary> 
	SOCKET sock;

	/// <summary> 
	/// Default constuctor
	/// </summary> 
	ConnectionTCP();

	/// <summary> 
	/// Set the window that will be updated by this connection
	/// </summary> 
	void setWindow(Window* myWindow);

	/// <summary>
	/// Inital connection to the server. Temporarily only connects to 
	/// server on local host
	/// </summary>
	bool TCPConnect();

	/// <summary>
	/// Listen to server and update internal data accordinly
	/// </summary>
	void TCPlisten();

	/// <summary> 
	/// Send data to the server
	/// </summary>
	void TCPsendData(void* data, int size);//might need size. TCP?

	/// <summary> 
	/// Close the Connection
	/// </summary> 
	void TCPclose();
};