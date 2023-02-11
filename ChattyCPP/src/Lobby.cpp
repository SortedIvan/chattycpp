#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")
#include "Lobby.h"
#include "Utility.h"
#include "User.h"
#include <string>
#include <iostream>
#include <enet/enet.h>
#include <windows.h>
using std::map;

//TODO: remove all of the function definitions and add them to a seperate class
std::string GetCurrentDirectory();
void DrawAllMessages(std::vector<sf::Text>& all_messages, sf::RenderWindow& lobby_window);
void SendMessage(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font, sf::Text& current_message_text, ENetPeer* peer);
void OnTextEntered(sf::Event event, std::string& current_text_message, int max_size_allowed, sf::Text& current_message_text);
void SendPacket(ENetPeer* peer, const char* data);
void SendMessageServer(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font, sf::Text& current_message_text);
std::string converter(uint8_t* str);
void BroadcastPacket(ENetHost* host, const char* data);
void ReceiveMessage(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string received_message, sf::Font& font);
bool DeleteUserByUsername(std::map<int*, std::string>& users, std::string username);

bool Lobby::HostLobby(std::string lobby_ip, std::string lobby_port, std::string lobby_name, std::string hosted_by, int max_participants, ENetHost* server, ENetEvent& enet_event) {
 

	sf::RenderWindow lobby_window(sf::VideoMode(800, 1000), lobby_name);
	sf::Event event;
	Utility utility;
	sf::Font text_font;
	utility.CheckFontLoaded(text_font, GetCurrentDirectory() + "/8bitfont.ttf");

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
	int client_id_counter = 0;
	std::map<int*, std::string> users;


	while (lobby_window.isOpen()) {


		while (enet_host_service(server, &enet_event, 1000) > 0)
		{
			std::string data_string;
			std::string user_has_connected = "connected_user|";
			std::string temp_username; 
			std::string message;

			switch (enet_event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					printf("A new client connected from: %x:%u.\n",
						enet_event.peer->address.host,
						enet_event.peer->address.port);

					enet_event.peer->data = new int(client_id_counter); // new client joins, set their id
					users.insert({ static_cast<int*>(enet_event.peer->data), "" });
					client_id_counter++;

					break;
				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received from %s on channel %u.\n",
						enet_event.packet->dataLength,
						enet_event.packet->data,
						enet_event.peer->data,
						enet_event.channelID);

					data_string = reinterpret_cast<char*>(enet_event.packet->data);

					if (data_string.find(user_has_connected) != std::string::npos) {
						// User initial connection
						users.insert_or_assign(static_cast<int*>(enet_event.peer->data), data_string.replace(data_string.find(user_has_connected), user_has_connected.length(), ""));
						temp_username = users[static_cast<int*>(enet_event.peer->data)];
						temp_username = temp_username + " has connected! Say hi!";
						BroadcastPacket(server, temp_username.c_str());

						break;
					}


					temp_username = users[static_cast<int*>(enet_event.peer->data)];
					message = temp_username + " : " + data_string;
					

					// broadcast message to other peers connected to server when something is received
					BroadcastPacket(server, message.c_str());


					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					temp_username = users[static_cast<int*>(enet_event.peer->data)];
					// User must be removed from all users before his data is deleted
					DeleteUserByUsername(users, temp_username);
					message = temp_username + " has disconnected!";
					BroadcastPacket(server, message.c_str());
					printf("%x:%u disconnected.\n",
						enet_event.peer->address.host,
						enet_event.peer->address.port,
						enet_event.peer->data = NULL
					);
					

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
				SendMessageServer(all_messages, all_messages_text, current_text_message, event, max_message_size, text_font, current_message_text);
			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) 
				{
					OnTextEntered(event, current_text_message, max_message_size, current_message_text);
				}
			}
		}
	}

	enet_host_destroy(server);

	return false;

}

// Once the user disconnects, the only way to know it was actually them is 
// searching for their username
bool DeleteUserByUsername(std::map<int*,std::string>& users,std::string username) 
{
	for (std::pair<int*, std::string> x : users) 
	{
		if (x.second == username) 
		{
			users.erase(x.first);
			return true;
		}
	}
	return false;
}

bool Lobby::JoinLobby(std::string lobby_ip, std::string lobby_port, std::string username, ENetHost* client, ENetEvent& enet_event, ENetPeer* peer){

	sf::RenderWindow lobby_window(sf::VideoMode(800, 1000), username);
	sf::Event event;
	Utility utility;
	sf::Font text_font;
	
	utility.CheckFontLoaded(text_font, GetCurrentDirectory() + "/8bitfont.ttf");

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

		while (enet_host_service(client, &enet_event, 200) > 0) 
		{
			switch (enet_event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received from %s on channel %u.\n",
						enet_event.packet->dataLength,
						enet_event.packet->data,
						enet_event.peer->data,
						enet_event.channelID);

					// DATA RECEIVED HERE, ADD MESSAGE TO MESSAGE POOl
					std::string received_message_string = converter(enet_event.packet->data);
					ReceiveMessage(all_messages, all_messages_text, received_message_string, text_font);


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
				SendMessage(all_messages, all_messages_text, current_text_message, event, max_message_size, text_font, current_message_text,peer);
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
		switch (enet_event.type)
		{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(enet_event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				puts("Disconnection succeeeded");
				break;
		}
	}
	return EXIT_SUCCESS;
}


// All of the messages contained in the text vector get drawn using a positioning multiplier
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

// -- Whenever a new message is detected, it gets added to the message pool
void ReceiveMessage(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string received_message, sf::Font& font) {
	sf::Text message_text;
	message_text.setFont(font);
	// ADD TO TEXT POOL
	message_text.setString(received_message);
	// ADD TO MESSAGE POOL
	all_messages.push_back(received_message);
	all_texts.push_back(message_text);
}

void SendMessage(std::vector<std::string>& all_messages,std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font, sf::Text& current_message_text, ENetPeer* peer) {

	if (current_message.size() > max_size_allowed) {
		return;
	}

	if (event.key.code == sf::Keyboard::Enter) {

		// WE DO NOT ADD TO TEXT POOL AND SAVE MESSAGE
		// Reason: the message is broadcasted to all peers, therefore it should not be saved here

		const char* data = current_message.c_str();


		SendPacket(peer, data);
		// Make current message empty after
		current_message = "";
		current_message_text.setString("");

	}
}

void SendMessageServer(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font, sf::Text& current_message_text) {

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

void SendPacket(ENetPeer* peer, const char* data) 
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void BroadcastPacket(ENetHost* host, const char* data) 
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, 0, packet);
}

std::string GetCurrentDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
}

std::string converter(uint8_t* str) {
	return std::string((char*)str);
}