#include "Utility.h"
#include <string>
#include <iostream>
#include  <SFML/Graphics.hpp>
using namespace std::literals::string_literals;

bool Utility::RemoveLastCharFromString(std::string& line, int index) {
	if (line.size() != 0) {
		line.erase(index - 1, 1);
		return false;
	}
	return true;
}

std::string Utility::MatchStringToRegex(std::regex regexpr, std::string string_to_match) {
	std::string found;
	std::smatch m;
	std::regex_search(string_to_match, m, regexpr);

	for (auto character : m) {
		found += character.str();
	}
	return found;
}


void Utility::CheckFontLoaded(sf::Font& font, std::string path) {
	if (!font.loadFromFile(path))
	{
		std::cout << "Error loading the font file" << std::endl;
		system("pause");
	}
}
