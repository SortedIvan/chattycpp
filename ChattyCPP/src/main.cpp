#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

#include  <SFML/Graphics.hpp>
#include <iostream>
#include <enet/enet.h>


int main(int argc, char** argv) {

	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occured while initializing enet! \n");
		return EXIT_FAILURE;
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Not Tetris");

	sf::Event event;

	while (window.isOpen()) {

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				window.close();
			}
		}
	}

	return 0;

}