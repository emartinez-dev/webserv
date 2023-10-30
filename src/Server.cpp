#include "Server.hpp"
#include "Utils.hpp"
#include "WebServerExceptions.hpp"

Server::Server()
{
}

Server::Server(std::string const &ip_address, int port)
{
	int	opt = 1;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	serverAddress = config_address(ip_address, port);

	if (serverSocket < 0)
		throw SocketCreationException();
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw SocketOptionsException();
	if (bind(serverSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0 && errno != EADDRINUSE)
		throw BindingException();
	if (listen(serverSocket, LISTEN_MAX_BACKLOG) < 0)
		throw ListeningException();
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw SocketOptionsException();
}

Server::~Server()
{
}

Server::Server(Server const &copy):serverAddress(copy.serverAddress),
	serverSocket(copy.serverSocket)
{
}

Server	&Server::operator=(const Server &copy)
{
	if (this != &copy)
	{
		serverAddress = copy.serverAddress;
		serverSocket = copy.serverSocket;
	}
	return *this;
}

int	Server::getServerSocket(void) const
{
	return serverSocket;
}

sockaddr_in	Server::getServerAddress(void) const
{
	return serverAddress;
}

