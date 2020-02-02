#include "ConnectionTCP.h"

ConnectionTCP::ConnectionTCP() {}


void ConnectionTCP::setWindow(SandboxWindow* myWindow) {
	window = myWindow;
}

void ConnectionTCP::TCPlisten() {
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
void ConnectionTCP::TCPsendData(void* data, int size) {
	int sendResult = send(sock, (char*)data, size, 0);
	//sendResult != SOCKET_ERROR
}

//Only if connected?
void ConnectionTCP::TCPclose() {
	closesocket(sock);
	WSACleanup();
}

bool ConnectionTCP::TCPConnect() {
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
		return true;
	}

	return false;
}