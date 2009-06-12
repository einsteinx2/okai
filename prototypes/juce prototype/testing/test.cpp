//#define N02_LINUX
#include "n02.h"
#include "string.h"
n02ClientInterface client;












int main(int argc, char * args[]) {
	n02ResetInterface(&client, INTERFACE_CLIENT);
	strcpy(client.app.name, "Fake Application");
	client.games.clear();
	client.games.add("Fake game", 2);
	client.games.add("Real game", 2);
	client.activate(&client);
	return 0;
} 
