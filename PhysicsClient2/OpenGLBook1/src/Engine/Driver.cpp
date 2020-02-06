#include <iostream>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include "../GUI/Window.h"

#pragma comment(lib, "ws2_32.lib")


/// <summary>
/// Main method that starts the application. Starts the window and promts the user
/// with connectionProtocol() to allow user to decide on the mode the application
/// will run on. The loop is set conisistently update/render the application based
/// on data in the winodw->handler and is limited to 60fps.
/// </summary>
int main() {
	//Memory leak check
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::cout << "Program started..." << std::endl;
	Window* myWindow = new Window(800, 800, 0);
	myWindow->handler->connectionProtocol();

	//Add a Max FPS option (other than 60)
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
	std::cout << "Begin closing" << std::endl;
	glfwTerminate();//have this in Window destructor?

	//gracefully close down everything. Only do server end if previosly connected to server
	myWindow->closeConnection();

	delete myWindow;
	return 0;
}