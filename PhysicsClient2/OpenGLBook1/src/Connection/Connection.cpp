#include "Connection.h"

Connection::Connection() {}

void Connection::connectionProtocol() {
	int stage = 0;//Switch to enum?
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
			theMode = Serverless;
			//theModeInt = 0;
			return;
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
			if (TCPCon.TCPConnect()) {
				theMode = TCP;

				return;
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
			} else {
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

			UDPCon.userId = std::stoi(input);
			std::cout << "Userid chosen: " << std::stoi(input) << ": " << UDPCon.userId << std::endl;

			std::cout << "Attempting to connect to a UDP server..." << std::endl;
			//do connection//implement so it returns false if not connected
			//connectCode = con.UDPConnect();
			connectCode = UDPCon.UDPConnect();
			if (connectCode == 1) {
				theMode = UDP;
				return;
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

void Connection::sendData(void* data, int size) {
	if (theMode == Serverless) {
		return;
	} else if (theMode == TCP) {
		TCPCon.TCPsendData(data, size);
	} if (theMode == UDP) {
		//append userID to end
		float* UDPdata = new float[size / sizeof(float) + sizeof(float)];
		for (int i = 0; i < size / sizeof(float); i++) {
			UDPdata[i] = ((float*)data)[i];
		}

		UDPdata[size / sizeof(float)] = UDPCon.userId;
		UDPCon.UDPSend(UDPdata, size + sizeof(float));
		delete[] UDPdata;
	}
}

void Connection::getData(void* data, int& size) {
	if (theMode == Serverless) {
		return;
	} else if (theMode == TCP) {
		TCPCon.TCPGetData(data, size);
	} else if (theMode == UDP) {
		//std::cout << "Get data" << std::endl;

		UDPCon.UDPGetData(data, size);
		//std::cout << "Get data recived: " << size << std::endl;
	}
}

void Connection::close() {
	if (theMode == Serverless) {
		return;
	} else if (theMode == TCP) {
		TCPCon.TCPclose();
	} else if (theMode == UDP) {
		UDPCon.UDPClose();
	}
}

