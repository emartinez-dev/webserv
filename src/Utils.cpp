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

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = parse_ip(ip_address);
	server_address.sin_port = htons(port);
	return server_address;
}


