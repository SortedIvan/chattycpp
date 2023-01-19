#pragma once
#include <iostream>
#include <string>
#include  <SFML/Graphics.hpp>



class Lobby {

	public:
		bool HostLobby(std::string lobby_ip, std::string lobby_port, std::string lobby_name, std::string hosted_by, int max_participants);
		bool JoinLobby(std::string lobby_ip, std::string lobby_port, std::string username);

};
