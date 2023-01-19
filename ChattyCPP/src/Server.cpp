#include "Server.h"
#include <iostream>
#include <enet/enet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")


int Server::RunServer() {


	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occured while initializing enet! \n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
	
	// 
	ENetHost* server;

}


