#include "WebServer.hpp"
#include "WebServerExceptions.hpp"

WebServer::WebServer()
{
}

WebServer::WebServer(std::string const &ip_address, int port):ip_address(ip_address),port(port)
{
}

WebServer::~WebServer()
{
}

WebServer::WebServer(WebServer const &copy):ip_address(copy.ip_address),port(copy.port)
{
}

WebServer	&WebServer::operator=(const WebServer &copy)
{
	if (this != &copy) {
		ip_address = copy.ip_address;
		port = copy.port;
	}
	return *this;
}

unsigned long WebServer::parse_ip(const std::string &ip_address)
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

sockaddr_in	WebServer::config_address(void)
{
	sockaddr_in	server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = parse_ip(ip_address);
	server_address.sin_port = htons(port);
	return server_address;
}

int	WebServer::start()
{
	int	listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in	server_address = config_address();

	if (listening_socket == -1)
		throw SocketCreationException();
	if (bind(listening_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) == -1)
		throw BindingException();
	if (listen(listening_socket, 128) == -1)
		throw ListeningException();

	while (1)
	{
		sockaddr_in client_address;
		socklen_t	client_len = sizeof(client_address);
		int			client_socket = accept(listening_socket, (struct sockaddr *)&client_address, &client_len);

		if (client_socket == -1)
		{
			std::cout << "Accept failed\n";
			continue;
		}
		std::cout << "Accept OK!\n";
		close(client_socket);
	}
	return (0);
}

