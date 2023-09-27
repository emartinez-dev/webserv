#include "Server.hpp"
#include "Utils.hpp"
#include "WebServerExceptions.hpp"

Server::Server()
{
}

Server::Server(std::string const &ip_address, int port)
{
	int	opt = 1;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	server_address = config_address(ip_address, port);

	if (server_socket < 0)
		throw SocketCreationException();
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw SocketOptionsException();
	if (bind(server_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0)
		throw BindingException();
	if (listen(server_socket, MAX_BACKLOG) < 0)
		throw ListeningException();
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) < 0)
		throw SocketOptionsException();
}

Server::~Server()
{
	close(server_socket);
}

Server::Server(Server const &copy):server_address(copy.server_address),
	server_socket(copy.server_socket)
{
}

Server	&Server::operator=(const Server &copy)
{
	if (this != &copy)
	{
		server_address = copy.server_address;
		server_socket = copy.server_socket;
	}
	return *this;
}

pollfd Server::create_polldf(int fd, short mode)
{
	struct pollfd server_fd;
	server_fd.fd = fd;
	server_fd.events = mode;
	return server_fd;
}

int	Server::accept_client(int server_fd)
{
	sockaddr_in client_address;
	int			client_socket;
	socklen_t	client_len = sizeof(client_address);

	bzero(&client_address, sizeof(client_address));
	client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
	return client_socket;
}

int	Server::get_server_socket(void) const
{
	return server_socket;
}

sockaddr_in	Server::get_server_address(void) const
{
	return server_address;
}

