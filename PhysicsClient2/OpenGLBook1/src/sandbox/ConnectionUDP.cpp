#include "ConnectionUDP.h"

ConnectionUDP::ConnectionUDP() {}

void ConnectionUDP::setWindow(SandboxWindow* myWindow) {
	window = myWindow;
}


// make intand return code
int ConnectionUDP::UDPConnect() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Can't start Winsock. " << wsOK << std::endl;
		return false;
	}

	//create a hint structure for the serer
	/*sockaddr_in UDPserver;*/
	UDPserver.sin_family = AF_INET;
	UDPserver.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &UDPserver.sin_addr);

	//Socket creation
	/*SOCKET*/ UDPout = socket(AF_INET, SOCK_DGRAM, 0);
	//END UDPInit()

	//send inital message and then listen
	float toSend[] = { -9, userId };
	int sendok = sendto(UDPout, (char*)toSend, 2 * sizeof(float), 0, (sockaddr*)&UDPserver, sizeof(UDPserver));
	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}

	char buf[1024];
	int serverLength = sizeof(UDPserver);
	int bytesIn = recvfrom(UDPout, buf, 1024, 0, (sockaddr*)&UDPserver, &serverLength);//recvfrom= UDP?

	std::cout << "Recerived on connection: " << bytesIn << std::endl;
	if (bytesIn >= 1) {
		std::cout << "Connection Code: " << ((float*)buf)[0] << std::endl;
	}

	if (bytesIn == -1) {
		return -1;
	} else {
		int code = ((float*)buf)[0];
		if (code == -1) {
			return -2;
		} else {
			return 1;
		}
	}
}


//not used?
void ConnectionUDP::UDPInit() {
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

// Let user know if lost connection?
void ConnectionUDP::UDPListen() {
	char buf[1024];
	int serverLength = sizeof(UDPserver);
	int bytesIn = recvfrom(UDPout, buf, 1024, 0, (sockaddr*)&UDPserver, &serverLength);//recvfrom= UDP?
	if (bytesIn != SOCKET_ERROR) {
		//Draw coordinates
		for (int i = 0; i < bytesIn / sizeof(float); i += 2) {
			float x1 = ((float*)buf)[i];
			float y1 = ((float*)buf)[i + 1];
			//std::cout << i << "Draw at" << x1 << ", " << y1 << std::endl;
			window->drawCoords(x1, y1);//change to be same as in renderer
		}
	}
}

void ConnectionUDP::UDPSend() {
	float toSend[] = { -1, window->handler->player->position.x, window->handler->player->position.y, userId };
	int sendok = sendto(UDPout, (char*)toSend, 4 * sizeof(float), 0, (sockaddr*)&UDPserver, sizeof(UDPserver));
	//std::cout << "Send Arg: " << toSend[0] << ", " << toSend[1] << toSend[2]  << toSend[3] << ": " << sendok << ": " << WSAGetLastError() << std::endl;
	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}
}

void ConnectionUDP::UDPSend(char* data, int size) {
	int sendok = sendto(UDPout, data, size, 0, (sockaddr*)&UDPserver, sizeof(UDPserver));

	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}
}

void ConnectionUDP::UDPClose() {
	//close that socket
	closesocket(UDPout);
	//close down Winsock
	WSACleanup();
}





void UDP() {
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