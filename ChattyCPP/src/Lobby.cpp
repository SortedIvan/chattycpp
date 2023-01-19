#include "Lobby.h"
#include "Utility.h"
#include <string>
#include <iostream>
#include <enet/enet.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")


void DrawAllMessages(std::vector<sf::Text>& all_messages, sf::RenderWindow& lobby_window);
void SendMessage(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font, sf::Text& current_message_text);
void OnTextEntered(sf::Event event, std::string& current_text_message, int max_size_allowed, sf::Text& current_message_text);

bool Lobby::HostLobby(std::string lobby_ip, std::string lobby_port, std::string lobby_name, std::string hosted_by, int max_participants) {
 
	sf::RenderWindow lobby_window(sf::VideoMode(800, 1000), lobby_name);
	sf::Event event;
	Utility utility;
	sf::Font text_font;
	utility.CheckFontLoaded(text_font, "../8bitfont.ttf");


	sf::Text current_message_text;
	std::string current_text_message;


	sf::RenderTexture bgTex;
	bgTex.create(800, 1000);

	sf::RectangleShape chat_box;
	chat_box.setSize(sf::Vector2f(700.f, 80.f));
	chat_box.setPosition(sf::Vector2f(50.f, 900.f));
	chat_box.setOutlineColor(sf::Color::White);
	bgTex.draw(chat_box);
	bgTex.display();

	sf::Sprite background_overlay(bgTex.getTexture());

	std::vector<sf::Text> all_messages_text;
	std::vector<std::string> all_messages;

	current_message_text.setString("TEST");
	current_message_text.setFont(text_font);
	current_message_text.setFillColor(sf::Color::Black);

	current_message_text.setPosition(chat_box.getPosition());

	int max_message_size = 80; // CHARACTERS ALLOWED IN MESSAGE

	while (lobby_window.isOpen()) {

		// DRAWING
		lobby_window.clear(sf::Color::Black);
		lobby_window.draw(background_overlay);
		lobby_window.draw(current_message_text);

		DrawAllMessages(all_messages_text, lobby_window);


		// DISPLAYING
		lobby_window.display();

		while (lobby_window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				lobby_window.close();
				return true;
			}

			if (event.type == sf::Event::KeyReleased) 
			{
				SendMessage(all_messages, all_messages_text, current_text_message, event, max_message_size, text_font, current_message_text);
			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) 
				{
					OnTextEntered(event, current_text_message, max_message_size, current_message_text);
				}
			}
		}
	}

	return false;

}

bool Lobby::JoinLobby(std::string lobby_ip, std::string lobby_port, std::string username, ENetHost& client, ENetEvent& enet_event, ENetPeer& peer){

	sf::RenderWindow lobby_window(sf::VideoMode(800, 1000), lobby_name);
	sf::Event event;
	Utility utility;
	sf::Font text_font;
	utility.CheckFontLoaded(text_font, "../8bitfont.ttf");


	sf::Text current_message_text;
	std::string current_text_message;


	sf::RenderTexture bgTex;
	bgTex.create(800, 1000);

	sf::RectangleShape chat_box;
	chat_box.setSize(sf::Vector2f(700.f, 80.f));
	chat_box.setPosition(sf::Vector2f(50.f, 900.f));
	chat_box.setOutlineColor(sf::Color::White);
	bgTex.draw(chat_box);
	bgTex.display();

	sf::Sprite background_overlay(bgTex.getTexture());

	std::vector<sf::Text> all_messages_text;
	std::vector<std::string> all_messages;

	current_message_text.setString("TEST");
	current_message_text.setFont(text_font);
	current_message_text.setFillColor(sf::Color::Black);

	current_message_text.setPosition(chat_box.getPosition());

	int max_message_size = 80; // CHARACTERS ALLOWED IN MESSAGE

	while (lobby_window.isOpen()) {

		// MAIN LOOP

		while (enet_host_service(client, &enet_event, 1000) > 0) 
		{
			switch (enet_event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received from %s on channel %u.\n",
						enet_event.packet->dataLength,
						enet_event.packet->data,
						enet_event.peer->data,
						enet_event.channelID);
					break;
			}

		}

		// DRAWING
		lobby_window.clear(sf::Color::Black);
		lobby_window.draw(background_overlay);
		lobby_window.draw(current_message_text);

		DrawAllMessages(all_messages_text, lobby_window);


		// DISPLAYING
		lobby_window.display();

		while (lobby_window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				lobby_window.close();
				return true;
			}

			if (event.type == sf::Event::KeyReleased)
			{
				SendMessage(all_messages, all_messages_text, current_text_message, event, max_message_size, text_font, current_message_text);
			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128)
				{
					OnTextEntered(event, current_text_message, max_message_size, current_message_text);
				}
			}
		}
	}

	enet_peer_disconnect(peer, 0);

	while (enet_host_service(client, &enet_event, 3000) > 0) 
	{
		switch (event.type) 
		{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(enet_event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnection succeeeded");
				break;
		}
	}


	return false;

}



void DrawAllMessages(std::vector<sf::Text>& all_messages, sf::RenderWindow& lobby_window) {

	float positioning_multiplier = 30.f;

	for (int i = 0; i < all_messages.size(); i++) {

		if (i == 0) {
			all_messages[i].setPosition(sf::Vector2f(50.f, positioning_multiplier));
			all_messages[i].setFillColor(sf::Color::White);
			lobby_window.draw(all_messages[i]);
			continue;
		}

		all_messages[i].setFillColor(sf::Color::White);
		all_messages[i].setPosition(sf::Vector2f(50.f, 30.f + (i * positioning_multiplier)));
		lobby_window.draw(all_messages[i]);
	}
}


void SendMessage(std::vector<std::string>& all_messages,std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font, sf::Text& current_message_text) {

	if (current_message.size() > max_size_allowed) {
		return;
	}

	if (event.key.code == sf::Keyboard::Enter) {

		sf::Text message_text;
		message_text.setFont(font);

		// ADD TO TEXT POOL
		message_text.setString(current_message);
		// ADD TO MESSAGE POOL
		all_messages.push_back(current_message);
		all_texts.push_back(message_text);

		std::cout << all_messages.size() << std::endl;

		// ---------- SEND HERE ---------
		// 
		// -------------------

		// Make current message empty after
		current_message = "";
		current_message_text.setString("");

	}
}

void OnTextEntered(sf::Event event, std::string& current_text_message, int max_size_allowed, sf::Text& current_message_text) {
	if (event.text.unicode != '\b' && event.text.unicode != 13 && event.text.unicode != '37' && event.text.unicode != '39' && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) 
	{
		// If the next entered character exceeds the set limit, don't add the character
		if (current_text_message.size() + 1 > max_size_allowed) {
			return;
		}
		current_text_message += event.text.unicode;
		std::cout << current_text_message << std::endl;
		current_message_text.setString(current_text_message);
	}
}
