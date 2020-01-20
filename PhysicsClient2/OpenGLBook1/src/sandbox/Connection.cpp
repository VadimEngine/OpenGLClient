#include "Connection.h"

Connection::Connection() {}


void Connection::setWindow(SandboxWindow* myWindow) {
	window = myWindow;
}

void Connection::listen() {
	char buf[4096];
	ZeroMemory(buf, 4096);
	int bytesRecieved = recv(sock, buf, 4096, 0);

	//for now do this here
	for (int i = 0; i < bytesRecieved / sizeof(float); i +=2) {
		float x1 = ((float*)buf)[i];
		float y1 = ((float*)buf)[i + 1];
		window->drawCoords(x1, y1);//change to be same as in renderer
	}
}

//create copy or send by refernce?
//return the result of the send?
void Connection::sendData(void* data, int size) {
	int sendResult = send(sock, (char*)data, size, 0);
	//sendResult != SOCKET_ERROR
}

//Only if connected?
void Connection::close() {
	closesocket(sock);
	WSACleanup();
}

//clean up required
bool Connection::connectionProtocol() {
	bool ret = false;

	std::cout << "Would you like to connect to a server? y/n" << std::endl << ">";
	std::string input;
	std::cin >> input;

	while (!(input == "y" || input == "n")) {
		std::cout << "Invalid input. Would you like to connect to a server? y/n" << std::endl << ">";
		std::cin >> input;
	}

	bool connectFlag = false;
	while (input == "y" && !connectFlag) {
		//connect to server
		std::cout << "Startig client..." << std::endl;

		std::string ipAddress = "127.0.0.1";	//ip address of the server
		int port = 54000;				//Listening port number on the server

		//Initilize winsock
		WSADATA data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0) {
			std::cerr << "Can't start Winsock, Error # " << wsResult << std::endl;
			return false;

		}

		//create socket
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			std::cerr << "Can't create socket, error # " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}

		//Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		//connect to the server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR) {
			std::cerr << "Can't connect to server, Error # " << WSAGetLastError() << std::endl;
		} else {
			connectFlag = true;
			return true;
		}

		//still not connected
		std::cout << "No server found, would you like to attempt to connect to server again? y/n" << std::endl << ">";
		std::cin >> input;
		while (input != "y" && input != "n") {
			std::cout << "Invalid invalid input, would you like to attempt to connect to server again? y/n" << std::endl << ">";
			std::cin >> input;
		}

		if (input == "n") {
			std::cout << "Running simulator serverless" << std::endl;
			closesocket(sock);
			WSACleanup();
			return false;
		}

		if (input == "y") {
			std::cout << "Attempting to connect to server..." << std::endl;
			connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
			if (connResult == SOCKET_ERROR) {
				std::cerr << "Can't connect to server, Error # " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				connectFlag = false;
			} else {
				connectFlag = true;
				return true;
			}
		}
	}

	if (input == "n") {
		std::cout << "Running program serverless" << std::endl;
		return false;
	}

	return ret;
}




void Connection::UDPInit() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Can't start Winsock. " << wsOK << std::endl;
		return;
	}

	//create a hint structure for the serer
	/*sockaddr_in UDPserver;*/
	UDPserver.sin_family = AF_INET;
	UDPserver.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &UDPserver.sin_addr);

	//Socket creation
	/*SOCKET*/ UDPout = socket(AF_INET, SOCK_DGRAM, 0);
}

void Connection::UDPListen() {
	char buf[1024];
	int serverLength = sizeof(UDPserver);
	int bytesIn = recvfrom(UDPout, buf, 1024, 0, (sockaddr*)&UDPserver, &serverLength);//recvfrom= UDP?
	
	//std::cout << "Received: " << bytesIn << std::endl;

	//Draw coordinates
	for (int i = 0; i < bytesIn / sizeof(float); i += 2) {
		float x1 = ((float*)buf)[i];
		float y1 = ((float*)buf)[i + 1];
		//std::cout << i << "Draw at" << x1 << ", " << y1 << std::endl;
		window->drawCoords(x1, y1);//change to be same as in renderer
	}
	
	//std::cout << "Server>" << buf << std::endl;
}

void Connection::UDPSend() {
	//std::string s("Testing123");
	//send player position
	float toSend[] = {-1, window->handler->player->position.x, window->handler->player->position.y};
	int sendok = sendto(UDPout, (char*)toSend, 3*sizeof(float), 0, (sockaddr*)&UDPserver, sizeof(UDPserver));
	//std::cout << "Send Arg: " << toSend[0] << ", " << toSend[1] << toSend[2] << ": " << sendok << ": " << WSAGetLastError() << std::endl;
	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}
}

void Connection::UDPClose() {
	//close that socket
	closesocket(UDPout);
	//close down Winsock
	WSACleanup();
}


void Connection::UDP() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Can't start Winsock. " << wsOK << std::endl;
		return;
	}

	//create a hint structure for the serer
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	//Socket creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	//write out to that socket
	std::string s("Testing123");

	//sendto = UDP
	int sendok = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));
	std::cout << "Arg: " << s << std::endl;
	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}

	//close that socket
	closesocket(out);
	//close down Winsock
	WSACleanup();
}