#pragma once

namespace ConnectionConstants {
	//replace with enum
	//when client is doing an inital connection
	static const int CLIENT_CONNECT = 1;
	//when connection is success and sending server info
	//server name, ip, number of client, or server info changed
	static const int SERVER_INFO = 1;

	static const int CLIENT_PARTICLE_PULL = 2;
	static const int SERVER_PARTICLE_POST = 2;
	static const int SERVER_CLIENTID_INVALID = 3;

	static const int CLIENT_PARTICLE_ADD = 4;
	static const int CLIENT_PARTICLE_REMOVE = 5;
	//send the desired paused value, flip whats on the server in order
	//to enforce it being synced
	static const int CLIENT_PAUSE = 6;

	static const int CLIENT_GRAVITY = 7;
}