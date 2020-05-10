#include "ConnectionUDP.h"


ConnectionUDP::ConnectionUDP() {}

void ConnectionUDP::init() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOK = WSAStartup(version, &data);
	if (wsOK != 0) {
		std::cout << "Can't start Winsock. " << wsOK << std::endl;
		return;
	}

	//Bind socket to ip address and port
	in = socket(AF_INET, SOCK_DGRAM, 0);//SOCK_DGRAM = UDP
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000); //Convert from little to big endian, host to network shorts

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
		std::cout << "Can't bind socket. " << WSAGetLastError() << std::endl;
		return;
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
		std::cerr << "Error. Bad host lookup." << std::endl;
		//return 1;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		std::cout << "Address " << i << ": " << inet_ntoa(addr) << std::endl;
	}
}

void ConnectionUDP::communicate(Handler* handler) {
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);//zero memory fills block with 0s (doesnt c++ already do that?)
	char buf[1024];

	while (true) {
		ZeroMemory(buf, 1024);
		//Wait for message
		int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);//recvfrom= UDP?

		if (bytesIn == SOCKET_ERROR) {
			std::cout << "Error receiving from client " << WSAGetLastError << std::endl;
			continue;
		}
		//Display message and Client info
		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::string clientId;

		//new version
		if (((float*)buf)[0] == ConnectionConstants::CLIENT_CONNECT) {
			int tempId = ((float*)buf)[1];
			std::cout << "User connect with id:" << tempId << std::endl;

			clientId = clientIp + tempId;
			std::cout << "CliendID: " << clientId << "  " << tempId << std::endl;

			if (clientMap.find(clientId) == clientMap.end()) {
				//add client
				clientMap[clientId] = new Particle(0, 0, true);
				handler->addClient(clientMap[clientId]);

				char toSend[255];
				for (int i = 0; i < handler->worldName.size() + 1; i++) {
					if (i == handler->worldName.size()) {
						toSend[i + 4] = '\0';
					} else {
						toSend[i + 4] = handler->worldName[i];
					}
				}
				((float*)toSend)[0] = ConnectionConstants::SERVER_INFO;


				//float success[1] = { 1 };
				sendto(in, (char*)toSend, 255, 0, (sockaddr*)&client, sizeof(client));

			} else {
				// else client already exists
				float errorMessage[1] = { ConnectionConstants::SERVER_CLIENTID_INVALID };
				sendto(in, (char*)errorMessage, (1) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));

			}
		} else {
			//get client id and proceed with processing
			int tempId = ((float*)buf)[3];
			clientId = clientIp + tempId;
		}

		Particle* temp = clientMap[clientId];

		if (temp != nullptr && ((float*)buf)[0] == ConnectionConstants::CLIENT_PARTICLE_PULL) {
			temp->x = ((float*)buf)[1];
			temp->y = ((float*)buf)[2];
			//std::cout << "P:" << p->x << ", " << p->y << std::endl;
		}

		if (((float*)buf)[0] == ConnectionConstants::CLIENT_PARTICLE_ADD) {
			handler->add(((float*)buf)[1], ((float*)buf)[2]);
		}

		if (((float*)buf)[0] == ConnectionConstants::CLIENT_PARTICLE_REMOVE) {
			handler->removeParticle(((float*)buf)[1]);
		}

		if (((float*)buf)[0] == ConnectionConstants::CLIENT_PARTICLE_PULL) {
			std::vector<float> toSend = handler->getSendData();
			sendto(in, (char*)toSend.data(), (toSend.size()) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));
		}

		if (((float*)buf)[0] == ConnectionConstants::CLIENT_GRAVITY) {
			if (((float*)buf)[1] == 0) {
				handler->gravtiy = false;
			} else {
				handler->gravtiy = true;
			}
		}

		if (((float*)buf)[0] == ConnectionConstants::CLIENT_PAUSE) {
			if (((float*)buf)[1] == 0) {
				handler->paused= false;
			} else {
				handler->paused = true;
			}
		}
	}
}

//send to who? pass in client info?
void ConnectionUDP::sendData(void* data, int size) {

}

void ConnectionUDP::getData(void* data, int& size) {

}

void ConnectionUDP::close() {}