#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include "SandboxWindow.h"
#include "Connection.h"

#pragma comment(lib, "ws2_32.lib")

//Check if server connection is lost
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SandboxWindow* myWindow;

	std::cout << "Program started..." << std::endl;

	//TCP
	Connection con;
	//bool serverless = !con.connectionProtocol();



	//UDP
	bool serverless = true;
	con.UDP();
	con.UDPInit();
	//con.UDPSend();


	//end of connection
	std::cout << "End of connecting protocol" << std::endl;

	//Add a Max FPS (other than 60)
	if (serverless) {
		//create window
		myWindow = new SandboxWindow(800, 800, 5);
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

				//client-Server tick TCP
				//float temp[3] = { 1, myWindow->handler->player->position.x, myWindow->handler->player->position.y };
				//con.sendData(temp, 3 * 4);
				//tcp
				//con.listen();
				//UDP
				con.UDPSend();
				con.UDPListen();
				//if (myWindow->leftClick) {
					//float toSend[3] = { -1, myWindow->mouseX, myWindow->mouseY };
					//con.sendData(toSend, 3 * 4);
					//myWindow->leftClick = false;
				//}
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
	//c
	//printf("dfhdfdf\n");


	//java 

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