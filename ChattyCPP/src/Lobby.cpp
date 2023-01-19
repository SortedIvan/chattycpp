#include "Lobby.h"
#include "Utility.h"
#include <string>
#include <iostream>



void DrawAllMessages(std::vector<sf::Text>& all_messages, sf::RenderWindow& lobby_window);
void SendMessage(std::vector<std::string>& all_messages, std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font);
void OnTextEntered(sf::Event event, std::string& current_text_message, int max_size_allowed);

bool Lobby::HostLobby(std::string lobby_ip, std::string lobby_port, std::string lobby_name, std::string hosted_by, int max_participants) {
 
	sf::RenderWindow lobby_window(sf::VideoMode(800, 1000), lobby_name);
	sf::Event event;
	sf::Text message_text;
	Utility utility;
	sf::Font text_font;
	utility.CheckFontLoaded(text_font, "../8bitfont.ttf");



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

	message_text.setString("TEST");
	message_text.setFont(text_font);
	message_text.setColor(sf::Color::Black);
	message_text.setOrigin(chat_box.getOrigin());

	int max_message_size = 80; // CHARACTERS ALLOWED IN MESSAGE

	while (lobby_window.isOpen()) {

		std::cout << "Window is open" << std::endl;

		// DRAWING
		lobby_window.clear(sf::Color::Black);
		lobby_window.draw(message_text);
		lobby_window.draw(background_overlay);
		lobby_window.draw(current_message);

		// DISPLAYING
		lobby_window.display();

		while (lobby_window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				lobby_window.close();
				return true;
			}

			if (event.type == sf::Event::KeyReleased) 
			{

			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) 
				{

				}
			}
		}
	}

	return false;

}

void DrawAllMessages(std::vector<sf::Text>& all_messages, sf::RenderWindow& lobby_window) {

	float positioning_multiplier = 20.f;

	for (int i = 0; i < all_messages; i++) {

		if (i == 0) {
			all_messages[i].setPosition(sf::Vector2f(50.f, 30.f));
			lobby_window.draw(all_messages[i]);
			continue;
		}
		
		all_messages[i].setPosition(sf::Vector2f(50.f, 30.f + (i * positioning_multiplier)));
		lobby_window.draw(all_messages[i]);
	}
}


void SendMessage(std::vector<std::string>& all_messages,std::vector<sf::Text>& all_texts, std::string& current_message, sf::Event event, int& max_size_allowed, sf::Font& font) {

	if (message.size() > max_size_allowed) {
		return;
	}

	if (event.key.code == sf::Keyboard::Enter) {

		sf::Text message_text;
		message_text.setFont(font);

		// ADD TO TEXT POOL
		message_text.setString(current_message);
		// ADD TO MESSAGE POOL
		all_messages.push_back(current_message);

		// ---------- SEND HERE ---------
		// 
		// -------------------

		// Make current message empty after
		current_message = "";

	}
}

void OnTextEntered(sf::Event event, std::string& current_text_message, int max_size_allowed) {
	if (event.text.unicode != '\b' && event.text.unicode != 13 && event.text.unicode != '37' && event.text.unicode != '39' && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) 
	{
		// If the next entered character exceeds the set limit, don't add the character
		if (current_text_message.size() + 1 > max_size_allowed) {
			return;
		}
		current_text_message += event.text.unicode;
	}
}
