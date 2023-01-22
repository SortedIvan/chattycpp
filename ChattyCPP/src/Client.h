#pragma once
#include <string>

class Client {
	public:
		int ConnectClient(const char* server_ip, int server_port, std::string username);

};