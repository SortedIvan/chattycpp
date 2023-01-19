#pragma once
#include <iostream>
#include <string>
#include  <SFML/Graphics.hpp>
#include <enet/enet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")


class Lobby {

	public:
		bool HostLobby(std::string lobby_ip, std::string lobby_port, std::string lobby_name, std::string hosted_by, int max_participants, ENetHost* server, ENetEvent& enet_event);
		bool JoinLobby(std::string lobby_ip, std::string lobby_port, std::string username, ENetHost* client, ENetEvent& enet_event, ENetPeer* peer);

};
