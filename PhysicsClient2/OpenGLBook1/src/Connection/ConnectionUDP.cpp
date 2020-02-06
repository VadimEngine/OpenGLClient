#include "ConnectionUDP.h"

ConnectionUDP::ConnectionUDP() {}

// make int and return code
int ConnectionUDP::UDPConnect() {
	std::cout << "UDPConnect" << std::endl;
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Can't start Winsock. " << wsOK << std::endl;
		return false;
	}
	//create a hint structure for the server
	UDPserver.sin_family = AF_INET;
	UDPserver.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &UDPserver.sin_addr);

	//Socket creation
	UDPout = socket(AF_INET, SOCK_DGRAM, 0);

	//send inital message and then listen
	float toSend[] = { -9, userId };
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
		if (code == -1) {
			return -2;
		} else {
			return 1;
		}
	}
}

void ConnectionUDP::UDPGetData(void* data, int& size) {
	std::cout << "UDPGetData" << std::endl;
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
	std::cout << "UDPSend" << std::endl;
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