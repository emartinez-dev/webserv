#include "webserv.hpp"

unsigned long parse_ip(const std::string &ip_address)
{
	std::vector<int>  ip;
	int				  dot_pos = 0;

	while (ip_address.find('.', dot_pos) != std::string::npos)
	{
		ip.push_back(atoi(&ip_address[dot_pos]));
		dot_pos = ip_address.find('.', dot_pos) + 1;
	}
	ip.push_back(atoi(&ip_address[dot_pos]));
	if (ip.size() != 4)
		throw InvalidIPException();
	return htonl((ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + ip[3]);
}

sockaddr_in	config_address(std::string const &ip_address, int port)
{
	sockaddr_in	server_address;

	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = parse_ip(ip_address);
	server_address.sin_port = htons(port);
	return server_address;
}

std::string splitKey(std::string const &line) {
	size_t pointsPos = line.find(":");
	std::string key = line.substr(0, pointsPos);
    key.erase(0, key.find_first_not_of(" \t\r"));
    key.erase(key.find_last_not_of(" \t\r") + 1);
	return key;
}

std::string splitValue(std::string const &line) {
	size_t pointsPos = line.find(":");
	std::string value = line.substr(pointsPos + 1);
    value.erase(0, value.find_first_not_of(" \t\r"));
    value.erase(value.find_last_not_of(" \t\r") + 1);
	return value;
}
