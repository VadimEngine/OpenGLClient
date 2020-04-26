#include "Connection.h"


Connection::Connection(Handler* handler) {
	this->handler = handler;
}

void Connection::connectProtocol() {
	std::cout << "Run Server as TCP(1) or UDP(2)\n>";
	std::string input;
	std::cin >> input;

	while (input != "1" && input != "2") {
		std::cout << "Invalid input. Run Server as TCP(1) or UDP(2)\n>";
		std::cin >> input;
	}

	if (input == "1") {//TCP mode chosen
		std::cout << "Running as TCP" << std::endl;
		theMode = ConnectionMode::TCP;
	} else {
		std::cout << "Running as UDP" << std::endl;
		theMode = ConnectionMode::UDP;
	}

	init();
	communicate();
	close();
}

void Connection::init() {
	if (theMode == TCP) {
		conTCP = new ConnectionTCP;
		conTCP->init();
	} else if (theMode == UDP) {
		conUDP = new ConnectionUDP();
		conUDP->init();
	}
}

void Connection::close() {
	if (theMode == TCP) {
		conTCP->close();
	} else if (theMode == UDP) {
		conUDP->close();
	}
}


void Connection::communicate() {
	if (theMode == TCP) {
		conTCP->communicate(this->handler);
	} else if (theMode == UDP) {
		conUDP->communicate(this->handler);
	}
}

void Connection::sendData(void* data, int size) {
	if (theMode == TCP) {

	} else if (theMode == UDP) {

	}
}

void Connection::getData(void* data, int& size) {
	if (theMode == TCP) {

	} else if (theMode == UDP) {
		
	}
}