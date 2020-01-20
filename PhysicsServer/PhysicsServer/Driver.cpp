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

	while (input != "1" && input != "2") {
		std::cout << "Invalid input. Run Server as TCP(1) or UDP(2)" << std::endl;
		std::cin >> input;
	} 
	std::thread(updateParticles).detach();
	printf("Server starting....\n");

	if (input == "1") {
		std::cout << "Running as TCP" << std::endl;
		//TCP
		Connection conTCP(&handler, true);
		conTCP.init();
		while (conTCP.listenForClient()) {}
		conTCP.close();
	} else {
		//UDP
		std::cout << "Running as UDP" << std::endl;
		
		ConnectionUDP conUDP(&handler);
		conUDP.init();
		//while...
		//con.close()
	}	
	
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
