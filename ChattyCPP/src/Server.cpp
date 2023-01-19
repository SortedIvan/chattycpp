#include "Server.h"
#include "Lobby.h"
#include <iostream>
#include <enet/enet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")


int Server::RunServer() {

	Lobby lobby;

	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occured while initializing enet! \n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
	
	ENetAddress address;
	ENetEvent event;
	ENetHost* server;

	address.host = ENET_HOST_ANY;
	//enet_address_set_host(&address, "212.187.55.58");

	address.port = 7777;


	// 

	server = enet_host_create(&address, 10, 1, 0, 0);

	if (server == NULL) {
		fprintf(stderr, "An error occured with the server");
		return EXIT_FAILURE;
	}

	lobby.HostLobby("test", "7777", "NANANANAN", "IVAN", 10, server, event);

}


