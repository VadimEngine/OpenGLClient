#include <WS2tcpip.h>
#include <thread>
#include <iostream>
#include <chrono>
#include "Handler.h"
#include "Connection.h"
#include "ConnectionUDP.h"

#pragma comment(lib, "ws2_32.lib")

void updateParticles();


Handler handler(10);

//Create a exit protocol
int main() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	std::cout << "Run Server as TCP(1) or UDP(2)" << std::endl;
	std::string input;
	std::cin >> input;

	while (input != "1" || input != "2") {
		std::cout << "Invalid input. Run Server as TCP(1) or UDP(2)" << std::endl;
		std::cin >> input;
	} 
	std::thread(updateParticles).detach();
	printf("Server starting....\n");

	if (input == "1") {
		std::cout << "Running as TCP" << std::endl;
		//TCP
		Connection con(&handler, true);
		con.init();
		while (con.listenForClient()) {}
		con.close();
	} else {
		std::cout << "Running as UDP" << std::endl;
		
		ConnectionUDP con(&handler);
		con.init();
		/*
		WSADATA data;
		WORD version = MAKEWORD(2, 2);
		int wsOK = WSAStartup(version, &data);
		if (wsOK != 0) {
			std::cout << "Can't start Winsock. " << wsOK << std::endl;
			return -1;
		}

		//Bind socket to ip address and port
		SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);//SOCK_DGRAM = UDP
		sockaddr_in serverHint;
		serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
		serverHint.sin_family = AF_INET;
		serverHint.sin_port = htons(54000); //Convert from little to big endian, host to network shorts

		if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
			std::cout << "Can't bind socket. " << WSAGetLastError() << std::endl;
			return -1;
		}

		sockaddr_in client;
		int clientLength = sizeof(client);
		ZeroMemory(&client, clientLength);//zero memory fills block with 0s (doesnt c++ already do that?)

		char buf[1024];

		Particle* p = new Particle(0, 0, true);
		handler.addClient(p);

		//enter a loop
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

			std::cout << "Message recv from " << clientIp << " : " << (float*)buf[0] << ", " << (float*)buf[1] << (float*)buf[2] << std::endl;
			if (((float*)buf)[0] == -1) {
				p->x = ((float*)buf)[1];
				p->y = ((float*)buf)[2];
				//std::cout << "P:" << p->x << ", " << p->y << std::endl;
			}
			float* coords = handler.getSendData();
			sendto(in, (char*)coords, (handler.objs.size() * 2 + handler.clientObjs.size() * 2) * sizeof(float), 0, (sockaddr*)&client, sizeof(client));
			//std::cout << "Sending: " << (handler.objs.size() * 2 + handler.clientObjs.size() * 2) << std::endl;
			delete coords;
		}

		//Close socket
		closesocket(in);

		//shut down winsock
		WSACleanup();
		*/
	}	
	//UDP

	return 0;
}

//do this in handler, have an exit conditon?
void updateParticles() {
	auto last = std::chrono::high_resolution_clock::now();
	while (true) {
		double dt = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last).count() / 1000000000.0;;
		if (dt >= 1.0/60.0) {
			handler.update(dt);
			last = std::chrono::high_resolution_clock::now();
		} else {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
}
