#include <iostream>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>
#include "../GUI/Window.h"
#include "../Connection/ConnectionTCP.h"
#include "../Connection/ConnectionUDP.h"

#pragma comment(lib, "ws2_32.lib")

/// <summary>
/// The TCP connection object. Is set by connectionProtocol()
/// </summary>
ConnectionTCP con;

/// <summary>
/// The UDP connection object, is set by connectionProtocol()
/// </summary>
ConnectionUDP conUDP;

/// <summary>
/// If TCP or UDP connection.
/// </summary>
bool TCP;
bool connectionProtocol();

/// <summary>
/// Main method that first promts in console to decide if they want to run with server or serverless.
/// then runs the program accordingly. The user can addtionally decide if they want to connect to the server
/// with TCP or UDP. The game loop will run and be limited to 60FPS.
/// </summary>
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Window* myWindow;
	std::cout << "Program started..." << std::endl;

	bool serverless = !connectionProtocol();

	//end of connection
	std::cout << "End of connecting protocol" << std::endl;

	//Add a Max FPS option (other than 60)
	//combine serverless and server code to reduce reduntant code
	if (serverless) {
		//create window
		myWindow = new Window(800, 800, 5);
		myWindow->serverMode = false;
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
		myWindow = new Window(800, 800, 0);
		myWindow->serverMode = true;
		myWindow->setServer(myWindow->serverMode);
		if (TCP) {
			con.setWindow(myWindow);
		} else {
			conUDP.setWindow(myWindow);
		}

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

				if (TCP) {
					//TCP
					float temp[3] = { 1, myWindow->handler->player->position.x, myWindow->handler->player->position.y };
					con.TCPsendData(temp, 3 * 4);
					con.TCPlisten();

					if (myWindow->leftClick) {
						float toSend[3] = { -1, myWindow->mouseX, myWindow->mouseY };
						con.TCPsendData(toSend, 3 * 4);
						myWindow->leftClick = false;
					}
				} else {
					//UDP
					float toSend[] = { -1, myWindow->handler->player->position.x, myWindow->handler->player->position.y, conUDP.userId };
					conUDP.UDPSend((char*)toSend, sizeof(float) * 4);
					conUDP.UDPListen();

					//click action
					if (myWindow->leftClick) {
						float toSend[4] = { -2, myWindow->mouseX, myWindow->mouseY , conUDP.userId};
						conUDP.UDPSend((char*)toSend, 4 * sizeof(float));
						myWindow->leftClick = false;
					}

				}							
	
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
		if (TCP) {
			con.TCPclose();
		} else {
			//con.UDPClose();
			conUDP.UDPClose();
		}
	}

	delete myWindow;
	return 0;
}

/// <summary>
/// Promts the user to chose if they want to the application with a server
/// or serverless. If the user choses server then the user can decide to run
/// with TCP or UDP.
/// </summary>
/// <remarks>
///	Move outside this class. Maybe in window or handler. Add :q to quit.
/// </remarks>
bool connectionProtocol() {
	int stage = 0;
	bool connected = false;
	int connectCode;
	std::string input;

	while (true) {
		switch (stage) {
		case 0://Decide if serverless or servermode
			std::cout << "Would you like to connect to server? y/n" << std::endl << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Would you like to connect to server? y/n" << std::endl << "> ";
				std::cin >> input;
			}
		
			if (input == "y") {//serverless mode chosen
				stage = 2;
			} else {//server mode chosen
				stage = 1;
			}
			break;

		case 1://serverless chosen. Running serverless
			std::cout << "Running program serverless." << std::endl;
			return false;
			break;

		case 2://Server mode chosen, decide if TCP or UDP
			std::cout << "Connect to server using TCP(1) or UDP(2)?" << std::endl << "> ";
			std::cin >> input;
			while (input != "1" && input != "2") {
				std::cout << "Invalid input. Connect to server using TCP(1) or UDP(2)?" << std::endl << "> ";
				std::cin >> input;
			}

			if (input == "1") {//TCP mode chosen
				stage = 3;
			} else {//UDP mode chosen
				stage = 4;
			}
			break;

		case 3://Decided TCP, if cant connect then decide if need to reconnect
			std::cout << "Attempting to connect to a TCP server..." << std::endl;
			//do connection
			if (con.TCPConnect()) {
				TCP = true;
				return true;
				break;
			}

			//failed to connect
			std::cout << "Failed to connect to a TCP server. Would you like to try again? y/n" << std::endl << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a TCP server. Would you like to try again? y/n" << std::endl << "> ";
				std::cin >> input;
			}
			if (input == "y") {
				stage = 3;
			} else  {
				stage = 0;
			}
			break;

		case 4://Decided UDP, if cant connect then decide if need to reconnect

			//ask for user id
			std::cout << "Enter user a user id integer between 1-9" << std::endl << "> ";
			std::cin >> input;
			while (!isdigit(input[0])) {
				std::cout << "Invalid input. Enter user a user id integer between 1-9" << std::endl << "> ";
				std::cin >> input;
			}

			//con.userId = std::stoi(input);
			conUDP.userId = std::stoi(input);
			std::cout << "Userid chosen: " << std::stoi(input) << ": " << conUDP.userId << std::endl;

			std::cout << "Attempting to connect to a UDP server..." << std::endl;
			//do connection//implement so it returns false if not connected
			//connectCode = con.UDPConnect();
			connectCode = conUDP.UDPConnect();
			if (connectCode == 1) {
				TCP = false;
				return true;
				break;
			} else if (connectCode == -2) {
				std::cout << "UserId already taken, please choose a different UserId" << std::endl;
			} else {
				std::cout << "Failed to connect to a UDP server. Would you like to try again? y/n" << std::endl << "> ";
				std::cin >> input;
				while (input != "y" && input != "n") {
					std::cout << "Invalid input. Failed to connect to a UDP server. Would you like to try again? y/n" << std::endl << "> ";
					std::cin >> input;
				}

				if (input == "y") {
					stage = 4;
				} else {
					stage = 0;
				}
			}
			break;

		default:
			break;
		}
	}
}