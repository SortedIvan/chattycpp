#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#include  <SFML/Graphics.hpp>
#include <iostream>
#include <enet/enet.h>
#include "Lobby.h"
#include "Client.h"
#include "Server.h"

int main() {

	Client client;
	Server server;
	Lobby lobby;

	int choice;
	std::cout << "Type 0 for server, 1 for client" << std::endl;
	std::cin >> choice;

	switch (choice) {

		case 0:
			server.RunServer();
		case 1:
			client.ConnectClient();
	}

	return 0;
}