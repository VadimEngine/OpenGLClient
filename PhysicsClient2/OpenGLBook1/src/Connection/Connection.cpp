#include "Connection.h"

Connection::Connection() {
	this->TCPCon = new ConnectionTCP();
	this->UDPCon = new ConnectionUDP();
	this->theMode = ConnectionMode::SERVERLESS;
}

Connection::~Connection() {
	delete TCPCon;
	delete UDPCon;
}

void Connection::sendData(void* data, int size) {
	if (theMode == ConnectionMode::SERVERLESS) {
		return;
	} else if (theMode == ConnectionMode::TCP) {
		TCPCon->TCPsendData(data, size);
	} if (theMode == ConnectionMode::UDP) {
		//append userID to end
		float* UDPdata = new float[size / sizeof(float) + sizeof(float)];
		for (unsigned int i = 0; i < size / sizeof(float); i++) {
			UDPdata[i] = ((float*)data)[i];
		}

		UDPdata[size / sizeof(float)] = (float)UDPCon->getUserId();
		UDPCon->UDPSend(UDPdata, size + sizeof(float));
		delete[] UDPdata;
	}
}

void Connection::getData(void* data, int& size) {
	if (theMode == ConnectionMode::SERVERLESS) {
		return;
	} else if (theMode == ConnectionMode::TCP) {
		TCPCon->TCPGetData(data, size);
	} else if (theMode == ConnectionMode::UDP) {
		UDPCon->UDPGetData(data, size);
	}
}

void Connection::close() {
	if (theMode == ConnectionMode::SERVERLESS) {
		return;
	} else if (theMode == ConnectionMode::TCP) {
		TCPCon->TCPclose();
	} else if (theMode == ConnectionMode::UDP) {
		UDPCon->UDPClose();
	}
}

ConnectionMode Connection::getMode() {
	return theMode;
}

void Connection::setMode(ConnectionMode theMode) {
	this->theMode = theMode;
}

ConnectionTCP* Connection::getTCPConnect() {
	return TCPCon;
}

ConnectionUDP* Connection::getUDPConnect() {
	return UDPCon;
}