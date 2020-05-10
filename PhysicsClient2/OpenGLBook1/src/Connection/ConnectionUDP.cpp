#include "ConnectionUDP.h"

ConnectionUDP::ConnectionUDP() {}

ConnectionUDP::~ConnectionUDP() {
	UDPClose();
}

// make int and return code
int ConnectionUDP::UDPConnect(int port, std::string ipAddress) {
	std::cout << "UDPConnect" << std::endl;
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Can't start Winsock. " << wsOK << std::endl;
		return -1;//return error int save as a constant
	}
	//create a hint structure for the server
	UDPserver.sin_family = AF_INET;
	UDPserver.sin_port = htons(port);

	inet_pton(AF_INET, ipAddress.c_str(), &UDPserver.sin_addr);

	//Socket creation
	UDPout = socket(AF_INET, SOCK_DGRAM, 0);

	//send inital message and then listen
	float toSend[] = { ConnectionConstants::CLIENT_CONNECT, userId };
	int sendok = sendto(UDPout, (char*)toSend, 2 * sizeof(float), 0, (sockaddr*)&UDPserver, sizeof(UDPserver));
	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}

	char buf[1024];
	int serverLength = sizeof(UDPserver);
	int bytesIn = recvfrom(UDPout, buf, 1024, 0, (sockaddr*)&UDPserver, &serverLength);//recvfrom= UDP?

	if (bytesIn == -1) {
		return -1;
	} else {
		int code = ((float*)buf)[0];
		if (code == ConnectionConstants::SERVER_CLIENTID_INVALID) {
			return ConnectionConstants::SERVER_CLIENTID_INVALID;
		} else {
			//set servername from input
			std::string theServerName;

			for (int i = 4; i < 255 && ((char*)buf)[i] != '\0'; i++) {
				theServerName += ((char*)buf)[i];
			}

			this->serverName = theServerName;
			std::cout <<"ServerName: " << serverName << std::endl;

			return ConnectionConstants::SERVER_INFO;
		}
	}
}

void ConnectionUDP::UDPGetData(void* data, int& size) {
	char buf[1024];
	int serverLength = sizeof(UDPserver);
	int bytesIn = recvfrom(UDPout, buf, 1024, 0, (sockaddr*)&UDPserver, &serverLength);//recvfrom= UDP?
	if (size != SOCKET_ERROR) {
		size = bytesIn / 3;
		for (int i = 0; i < size; i++) {
			((float*)data)[i] = ((float*)buf)[i];
		}
	}
}

void ConnectionUDP::UDPSend(void* data, int size) {
	int sendok = sendto(UDPout, (char*)data, size, 0, (sockaddr*)&UDPserver, sizeof(UDPserver));

	if (sendok == SOCKET_ERROR) {
		std::cout << "Sendto error: " << WSAGetLastError() << std::endl;
	}
}

void ConnectionUDP::UDPClose() {
	std::cout << "UDPClose" << std::endl;
	closesocket(UDPout);
	WSACleanup();
}

void ConnectionUDP::setUserID(int id) {
	this->userId = id;
}

int ConnectionUDP::getUserId() {
	return userId;
}

std::string ConnectionUDP::getServerName() {
	return serverName;
}