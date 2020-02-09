#pragma comment(lib, "ws2_32.lib")

#include "./Physics/Handler.h"
#include "./Connection/Connection.h"

// flag if memory leaks should be checked
#define MEMLEAK false


//Create a exit protocol
int main() {
#if MEMLEAK
	std::cout << "MEMleak check " << std::endl;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//Uncomment this line to check for memory leaks
#endif

	Handler* handler = new Handler(10);

	std::thread(Handler::UpdatePhysics, handler).detach();
	Connection* connect = new Connection(handler);
	connect->connectProtocol();

	delete handler;
	return 0;
}