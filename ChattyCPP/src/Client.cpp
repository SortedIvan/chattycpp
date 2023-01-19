#include "Client.h"
#include "Lobby.h"
#include <iostream>
#include <enet/enet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#include <chrono>
#include <thread>

int Client::ConnectClient(const char* server_ip, int server_port) {

	Lobby lobby;
	
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occured while initializing enet! \n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	// 
	ENetHost* client;
	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (client == NULL) 
	{
		std::cout << "Client is null. Please try again!" << std::endl;
		return EXIT_FAILURE;
	}

	ENetAddress adress; // IP AND PORT
	ENetEvent event; 
	ENetPeer* peer;

	enet_address_set_host(&adress, server_ip);
	adress.port = server_port;

	peer = enet_host_connect(client, &adress, 1, 0);

	if (peer == NULL) 
	{
		std::cout << "No avaialble peers to connect to!" << std::endl;
		return EXIT_FAILURE;
	}

	// IF WE RECIEVE SOMETHING FROM SERVER
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		std::cout << "Connection succesful" << std::endl;
	}
	else 
	{
		enet_peer_reset(peer);
		std::cout << "Connection unsuccesful" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		return EXIT_SUCCESS;
	}

	// MAIN LOOP

	lobby.JoinLobby("127.0.0.1", "7777", "Ivan",client, event, peer);

}