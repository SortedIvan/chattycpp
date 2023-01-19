#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#include  <SFML/Graphics.hpp>
#include <iostream>
#include <enet/enet.h>
#include "Lobby.h"


int main() {


	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occured while initializing enet! \n");
		return EXIT_FAILURE;
	}

	Lobby lobby;
	lobby.HostLobby("212.187.55.58", "7777", "First Chat Lobby", "Ivan", 5);

	return 0;
}