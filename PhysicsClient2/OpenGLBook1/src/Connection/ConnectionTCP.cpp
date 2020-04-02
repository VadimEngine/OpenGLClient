#include "ConnectionTCP.h"


ConnectionTCP::ConnectionTCP() {
	//assume prot 54000 and local host
	//port = 54000;
	//ipAddress = "127.0.0.1";
	//"192.168.0.108";//got from ipconfig
}

ConnectionTCP::ConnectionTCP(int port, std::string ipAddress) {
	//assume prot 54000 and local host
	//port = port;
	//this->ipAddress = ipAddress;
}


bool ConnectionTCP::TCPConnect(int port, std::string ipAddress) {
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
	//hint.sin_addr.s_addr = INADDR_ANY;

	//connect to the server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server, Error # " << WSAGetLastError() << std::endl;
	} else {
		return true;
	}
	return false;
}

void ConnectionTCP::TCPGetData(void* data, int& size) {
	char buf[4096];
	ZeroMemory(buf, 4096);
	int bytesRecieved = recv(sock, buf, 4096, 0);

	size = bytesRecieved / sizeof(float);
	for (int i = 0; i < size; i++) {
		//execption here when server is disconnect suddenly, check befre going into this line
		// access violation writing location
		((float*)data)[i] = ((float*)buf)[i];
	}
}

void ConnectionTCP::TCPsendData(void* data, int size) {
	int sendResult = send(sock, (char*)data, size, 0);
}

void ConnectionTCP::TCPclose() {
	closesocket(sock);
	WSACleanup();
}