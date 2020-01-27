#include "ConnectionUDP.h"


//have clients send a userid
ConnectionUDP::ConnectionUDP(Handler* handler) {
	this->handler = handler;
}


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
}

//disconnect user if no update for too long
void ConnectionUDP::listen() {
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

		//Inital connection
		if (((float*)buf)[0] == -9) {
			int temp = ((float*)buf)[1];
			std::cout << "User connect with id:" << temp << std::endl;

			clientId = clientIp+temp;

			if (clientMap.find(clientId) == clientMap.end()) {
				//add client
				clientMap[clientId] = new Particle(0, 0, true);
				handler->addClient(clientMap[clientId]);
				float success[1] = {1};
				sendto(in, (char*)success, (1) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));

			} else {
				// else client already exists
				float errorMessage[1] = { -1.0f };
				sendto(in, (char*)errorMessage, (1) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));

			}
			//check if id is taken
		} else {
			int temp = ((float*)buf)[3];
			clientId = clientIp + temp;
		};

		Particle* temp = clientMap[clientId];

		if (temp != nullptr && ((float*)buf)[0] == -1) {
			temp->x = ((float*)buf)[1];//exception thrown here? access violation
			temp->y = ((float*)buf)[2];
			//std::cout << "P:" << p->x << ", " << p->y << std::endl;
		}

		if (((float*)buf)[0] == -2) {
			handler->add(((float*)buf)[1], ((float*)buf)[2]);
		}

		float* coords = handler->getSendData();
		sendto(in, (char*)coords, (handler->objs.size() * 2 + handler->clientObjs.size() * 2) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));
		//std::cout << "Sending: " << (handler.objs.size() * 2 + handler.clientObjs.size() * 2) << std::endl;
		delete coords;
	}
}

//proper close method
void ConnectionUDP::close() {
	/*
	//close that socket
	closesocket(UDPout);
	//close down Winsock
	WSACleanup();
	*/
}