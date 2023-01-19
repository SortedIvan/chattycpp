#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#include  <SFML/Graphics.hpp>
#include <iostream>
#include <enet/enet.h>
#include "Lobby.h"
#include "Client.h"
#include "Server.h"
#include <string>


int main() {

	Client client;
	Server server;
	Lobby lobby;

	std::string server_ip;
	int server_port;

	int choice;
	std::cout << "Type 0 for server, 1 for client" << std::endl;
	std::cin >> choice;

	switch (choice) {


		case 0:
			server.RunServer();
		case 1:
			std::cout << "Enter server ip: " << std::endl;
			std::cin >> server_ip;

			std::cout << "Enter server port: " << std::endl;
			std::cin >> server_port;

			client.ConnectClient(server_ip.c_str(),server_port);
	}

	return 0;
}