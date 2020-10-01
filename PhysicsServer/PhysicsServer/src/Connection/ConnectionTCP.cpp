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

// cancel warning for now until optimization and discover how to replace:
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
			//send server info and then begin the thread
			std::thread(listenClient2, (void*)&client, handler).detach();
		}
	}
}

void ConnectionTCP::sendData(void* data, int size) {}

void ConnectionTCP::getData(void* data, int& size) {}

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

	char buff[200];
	while (recv(Client, buff, 200, 0)) {
		if (((float*)buff)[0] == ConnectionConstants::CLIENT_CONNECT) {
			//send server name			
			char toSend[255];
			for (int i = 0; i < handler->worldName.size() + 1; i++) {
				if (i == handler->worldName.size()) {
					toSend[i+4] = '\0';
				} else {
					toSend[i+4] = handler->worldName[i];
				}
			}	
			((float*)toSend)[0] = ConnectionConstants::SERVER_INFO;
			send(Client, (char*)toSend, 255, 0);
		} else if (((float*)buff)[0] == ConnectionConstants::CLIENT_PARTICLE_PULL) {
			p->x = ((float*)buff)[1];
			p->y = ((float*)buff)[2];
			std::vector<float> sendData = handler->getSendData();
			send(Client, (char*)sendData.data(), sendData.size() * sizeof(float), 0);
		} else if (((float*)buff)[0] == ConnectionConstants::CLIENT_PARTICLE_ADD) {
			//add particle at sent location
			handler->add(((float*)buff)[1], ((float*)buff)[2]);
		} else if (((float*)buff)[0] == ConnectionConstants::CLIENT_PARTICLE_REMOVE) {
			//remove particle with sent id if it exists
			handler->removeParticle(((float*)buff)[1]);
		} else if (((float*)buff)[0] == ConnectionConstants::CLIENT_GRAVITY) {
			if (((float*)buff)[1] == 0) {
				handler->gravtiy = false;
			} else {
				handler->gravtiy = true;
			}
		} else if (((float*)buff)[0] == ConnectionConstants::CLIENT_PAUSE) {
			if (((float*)buff)[1] == 0) {
				handler->paused = false;
			} else {
				handler->paused = true;
			}
		}

	}

	p->isDelete = true;
	std::cout << "Client disconnected" << std::endl;
}