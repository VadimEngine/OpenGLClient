#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include "SandboxWindow.h"
#include "Connection.h"

#pragma comment(lib, "ws2_32.lib")

Connection con;
bool TCP;
void connectionProtocol();


//Check if server connection is lost
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SandboxWindow* myWindow;

	std::cout << "Program started..." << std::endl;

	//TCP
	//Connection con;Connection con;
	//bool serverless = !con.connectionProtocol();//make method in window/handler

	connectionProtocol();

	bool serverless = true;

	//UDP
	//bool serverless = true;
	//con.UDP();
	//con.UDPInit();


	//end of connection
	std::cout << "End of connecting protocol" << std::endl;

	//Add a Max FPS (other than 60)
	if (serverless) {
		//create window
		myWindow = new SandboxWindow(800, 800, 5);
		//myWindow->connectionProtocol();
		int frames = 0;
		double unprocessedSeconds = 0;
		auto lastTime = std::chrono::high_resolution_clock::now();
		double secondsPerTick = 1 / 60.0;
		int tickCount = 0;


		while (!myWindow->shouldClose()) {
			auto now = std::chrono::high_resolution_clock::now();
			auto passedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - lastTime).count();
			lastTime = now;
			if (passedTime < 0) {
				passedTime = 0;
			}

			if (passedTime > 100'000'000) {
				passedTime = 100'000'000;
			}
			unprocessedSeconds += passedTime / 1000000000.0;
			bool ticked = false;
			while (unprocessedSeconds > secondsPerTick) {
				myWindow->update();//tick();
				unprocessedSeconds -= secondsPerTick;
				ticked = true;
				tickCount++;
				if (tickCount % 60 == 0) {
					std::cout << "FPS: " << frames << std::endl;
					auto t1000ns = std::chrono::nanoseconds(1000);
					lastTime += t1000ns;
					frames = 0;
				}
			}
			if (ticked) {
				myWindow->render();
				frames++;
			} else {
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}
	} else {
		//create window
		myWindow = new SandboxWindow(800, 800, 0);
		con.setWindow(myWindow);

		int frames = 0;
		double unprocessedSeconds = 0;
		auto lastTime = std::chrono::high_resolution_clock::now();
		double secondsPerTick = 1 / 60.0;
		int tickCount = 0;

		while (!myWindow->shouldClose()) {
			auto now = std::chrono::high_resolution_clock::now();
			auto passedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - lastTime).count();
			lastTime = now;
			if (passedTime < 0) {
				passedTime = 0;
			}

			if (passedTime > 100'000'000) {
				passedTime = 100'000'000;
			}
			unprocessedSeconds += passedTime / 1000000000.0;
			bool ticked = false;
			while (unprocessedSeconds > secondsPerTick) {
				myWindow->update();

				//UDP
				//con.UDPSend();
				//con.UDPListen();


				//client-Server tick TCP
				float temp[3] = { 1, myWindow->handler->player->position.x, myWindow->handler->player->position.y };
				con.sendData(temp, 3 * 4);
				//tcp
				con.listen();
	
				if (myWindow->leftClick) {
					float toSend[3] = { -1, myWindow->mouseX, myWindow->mouseY };
					con.sendData(toSend, 3 * 4);
					myWindow->leftClick = false;
				}
				//end of client-Server tick

				unprocessedSeconds -= secondsPerTick;
				ticked = true;
				tickCount++;
				if (tickCount % 60 == 0) {
					std::cout << "FPS: " << frames << std::endl;
					auto t1000ns = std::chrono::nanoseconds(1000);
					lastTime += t1000ns;
					frames = 0;
				}
			}
			if (ticked) {
				myWindow->render();
				frames++;
			} else {
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}
	}

	std::cout << "Begin closing" << std::endl;

	glfwTerminate();//have this in Window destructor?

	//gracefully close down everything. Only do server end if previosly connected to server
	if (!serverless) {
		//TCP
		con.close();
		con.UDPClose();
	}

	delete myWindow;
	return 0;
}


void connectionProtocol() {
	bool finished = false;
	std::string input;
	//use stage to do a switch
	int stage = 0;
	bool connected = false;

	while (!finished) {
		switch (stage) {
		case 0://Decide if serverless or servermode
			std::cout << "Would you like to connect to server? y/n" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Would you like to connect to server? y/n" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "y") {//serverless mode chosen
				stage = 2;
			}

			if (input == "n") {//server mode chosen
				stage = 1;
			}

			break;

		case 1://serverless chosen
			std::cout << "Running program serverless." << std::endl;
			finished = true;
			break;


		case 2:
			std::cout << "Connect to server using TCP(1) or UDP(2)?" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "1" && input != "2") {
				std::cout << "Invalid input. Connect to server using TCP(1) or UDP(2)?" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "1") {//TCP mode chosen
				stage = 3;
			}

			if (input == "2") {//UDP mode chosen
				stage = 4;
			}
			break;

		case 3:
			std::cout << "Attempting to connect to a TCP server..." << std::endl;

			//do connection
			connected = con.tcpConnect();
			if (connected) {
				finished = true;
				TCP = true;
				break;
			}

			//failed to connect
			std::cout << "Failed to connect to a TCP server. Would you like to try again? y/n" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a TCP server. Would you like to try again? y/n" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "y") {
				stage = 3;
			}

			if (input == "n") {
				stage = 0;
			}

			break;

		case 4:
			std::cout << "Attempting to connect to a UDP server..." << std::endl;

			//do connection
			con.udpConnect();
			finished = true;
			TCP = false;
			break;


			//failed to connect
			std::cout << "Failed to connect to a UDP server. Would you like to try again? y/n" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a UDP server. Would you like to try again? y/n" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "y") {
				stage = 4;
			}
			if (input == "n") {
				stage = 0;
			}
			break;

		default:
			break;
		}
	}
}