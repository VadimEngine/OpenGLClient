#pragma once
#include "ConnectionTCP.h"
#include "ConnectionUDP.h"
#include "ConnectionConstants.h"
#include <string>


/// <summary>
/// Enum for possible connection modes of the application
/// </summary>
enum class ConnectionMode : char {SERVERLESS, TCP, UDP};

/// <summary>
/// Class to handler all connection logic.
/// </summary>
/// <remarks>
/// In future take in input to decide which server to connect to
/// such as Ip or port number. input can help to allow
/// connecting to multiple servers at once. Or even allowing
/// this client to be a server is needed
///
/// Might need to make this work on a different thread?
/// keep the server data saved and surrounded by lock(s)
/// and have tick/render read the data of most recently saved
/// data from server, also need connection time out and ping?
/// server data update might not need to be 60 times a second?
///
/// </remarks>
class Connection {

private:

	//server name
	std::string serverName;


	/// <summary>
	/// The mode that the connection is handled
	/// change to pointer
	/// </summary>
	ConnectionMode theMode;

	/// <summary>
	/// TCP connection object, used to handle
	/// connecting, sending, and getting data
	/// from TCP sever
	/// </summary>
	ConnectionTCP* TCPCon;

	/// <summary>
	/// UDP connection object, used to handle
	/// connecting, sending, and getting data from
	/// UDP server
	/// </summary>
	ConnectionUDP* UDPCon;
public:
	/// <summary>
	/// Constructor. Initializes TCPCon and UDPCon
	/// </summary>
	Connection();

	~Connection();

	/// <summary>
	/// Send data to the connected server based
	/// on the mode the application is running on
	/// </summary>
	void sendData(void* data, int size);

	/// <summary>
	/// Get data from the connected served based
	/// on the mode the application is running on
	/// </summary>
	void getData(void* data, int& size);

	/// <summary>
	/// Close down the connection to the server
	/// </summary>
	void close();

	ConnectionMode getMode();

	void setMode(ConnectionMode theMode);

	ConnectionTCP* getTCPConnect();

	ConnectionUDP* getUDPConnect();
};