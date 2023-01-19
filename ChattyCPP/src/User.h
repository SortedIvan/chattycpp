#pragma once

#include <string>

class User {
	private:
		std::string username;
		std::string location;
		std::string current_lobby;

	public:
		User(std::string _username, std::string _location);
		std::string GetUsername();
		std::string GetLocation();
		std::string GetCurrentLobby();
		
		void SetUsername(std::string newusername);
		void SetLocation(std::string newlocation);
		void SetCurrentLobby(std::string lobby);

};