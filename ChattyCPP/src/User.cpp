#include <string>
#include "User.h"


User::User(std::string _username, std::string _location) {
	username = _username;
	location = _location;
}

std::string User::GetUsername() {
	return username;
}

std::string User::SetUsername(std::string newusername) {
	username = newusername;
}

std::string User::GetLocation() {
	return location;
}

void User::SetLocation(std::string newlocation) {
	location = newlocation;
}

std::string User::GetCurrentLobby() {
	return current_lobby;
}

void User::SetCurrentLobby(std::string lobby) {
	current_lobby = lobby;
}