#include "ConnectionTCP.h"

void listenClient2(void* data, Handler* handler);

ConnectionTCP::ConnectionTCP() {}

void ConnectionTCP::init() {
	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(54000);//2222
	addr.sin_addr.s_addr = INADDR_ANY;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);//2.2 
	if (iResult) {
		std::cout << "WSA startup failed" << std::endl;
		exit(-1);
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cout << "Invalid socket" << std::endl;
		exit(-1);
	}

	iResult = bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (iResult) {
		std::cout << "Build failed " << GetLastError() << std::endl;
		exit(-1);
	}

	iResult = listen(sock, SOMAXCONN);
	if (iResult) {
		std::cout << "iResult failed " << GetLastError() << std::endl;
		exit(-1);//Exit() instead?
	}

//cancel warning for now until optimization and discover how to replace:
// 'gethostbyname': Use getaddrinfo() or GetAddrInfoW()
// 'inet_ntoa': Use inet_ntop() or InetNtop() instead
#pragma warning( disable : 4996)

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << std::endl;
		//return 1;
	}
	std::cout << "Host name is " << ac << "." << std::endl;

	struct hostent* phe = gethostbyname(ac);
	if (phe == 0) {
		std::cerr << "Yow! Bad host lookup." << std::endl;
		//return 1;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		std::cout << "Address " << i << ": " << inet_ntoa(addr) << std::endl;
	}


}

void ConnectionTCP::communicate(Handler* handler) {
	//sned world name to client when they first connect
	while(true) {
		client = accept(sock, 0, 0);
		if (client == INVALID_SOCKET) {
			std::cout << "Invalid client socket " << GetLastError() << std::endl;
		} else {
			std::thread(listenClient2, (void*)&client, handler).detach();
		}
	}
}

void ConnectionTCP::sendData(void* data, int size) {

}

void ConnectionTCP::getData(void* data, int& size) {

}

void ConnectionTCP::close() {
	closesocket(client);
	closesocket(sock);
	WSACleanup();
}

void listenClient2(void* data, Handler* handler) {
	//SOCKET* client = (SOCKET*)data;
	SOCKET Client = *((SOCKET*)data);
	std::cout << "Client connected" << std::endl;

	Particle* p = new Particle(0, 0, true);//temp position
	handler->addClient(p);

	char chunk[200];
	while (recv(Client, chunk, 200, 0)) {
		if (((float*)chunk)[0] == 1) {
			p->x = ((float*)chunk)[1];
			p->y = ((float*)chunk)[2];

			int size2;
			float* temp2;
			//Comment out until handler is figured out
			std::tie(size2, temp2) = handler->getSendData2();
			send(Client, (char*)temp2, size2 * sizeof(float), 0);

			delete[] temp2;
		} else if (((float*)chunk)[0] == -2) {
			handler->add(((float*)chunk)[1], ((float*)chunk)[2]);
		}

	}

	p->isDelete = true;
	std::cout << "Client disconnected" << std::endl;
}