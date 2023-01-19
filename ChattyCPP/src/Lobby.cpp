#include "Lobby.h"
#include "Utility.h"
#include <string>
#include <iostream>

bool Lobby::HostLobby(std::string lobby_ip, std::string lobby_port, std::string lobby_name, std::string hosted_by, int max_participants) {
 
	sf::RenderWindow lobby_window(sf::VideoMode(1500, 1000), lobby_name);
	sf::Event event;
	sf::Text message_text;
	


	std::vector<sf::Text> all_messages_text;
	std::vector<std::string> all_messages;



	while (lobby_window.isOpen()) {

		std::cout << "Window is open" << std::endl;
		lobby_window.clear(sf::Color::Black);

		while (lobby_window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				lobby_window.close();
				return true;
			}
		}
	}

	return false;

}