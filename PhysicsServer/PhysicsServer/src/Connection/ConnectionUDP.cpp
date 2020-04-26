#include "ConnectionUDP.h"


ConnectionUDP::ConnectionUDP() {

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

		//Inital connection
		if (((float*)buf)[0] == -9) {
			int tempId = ((float*)buf)[1];
			std::cout << "User connect with id:" << tempId << std::endl;

			clientId = clientIp + tempId;
			std::cout << "CliendID: " << clientId << "  " << tempId << std::endl;

			if (clientMap.find(clientId) == clientMap.end()) {
				//add client
				clientMap[clientId] = new Particle(0, 0, true);
				handler->addClient(clientMap[clientId]);
				float success[1] = { 1 };
				sendto(in, (char*)success, (1) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));

			} else {
				// else client already exists
				float errorMessage[1] = { -1.0f };
				sendto(in, (char*)errorMessage, (1) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));

			}
			//check if id is taken
		} else {
			int tempId = ((float*)buf)[3];
			clientId = clientIp + tempId;
		};

		Particle* temp = clientMap[clientId];

		if (temp != nullptr && ((float*)buf)[0] == 1) {
			temp->x = ((float*)buf)[1];
			temp->y = ((float*)buf)[2];
			//std::cout << "P:" << p->x << ", " << p->y << std::endl;
		}

		if (((float*)buf)[0] == -2) {
			handler->add(((float*)buf)[1], ((float*)buf)[2]);
		}
		//update to getSendData3 for UDP
		//float* coords = handler->getSendData();
		//sendto(in, (char*)coords, (handler->objs.size() * 2 + handler->clientObjs.size() * 2) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));
		//std::cout << "Sending: " << (handler.objs.size() * 2 + handler.clientObjs.size() * 2) << std::endl;
		//delete coords;

		std::vector<float> toSend = handler->getSendData3();
		sendto(in, (char*)toSend.data(), (toSend.size()) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));
	}
}

//send to who? pass in client info?
void ConnectionUDP::sendData(void* data, int size) {

}

void ConnectionUDP::getData(void* data, int& size) {

}

void ConnectionUDP::close() {

}