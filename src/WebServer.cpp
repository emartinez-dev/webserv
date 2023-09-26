#include "WebServer.hpp"
#include "Utils.hpp"
#include "WebServerExceptions.hpp"

WebServer::WebServer()
{
}

WebServer::WebServer(std::string const &ip_address, int port)
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

WebServer::~WebServer()
{
	close(server_socket);
}

WebServer::WebServer(WebServer const &copy):server_address(copy.server_address),
	server_socket(copy.server_socket)
{
}

void  WebServer::poll(void)
{
	std::vector<pollfd> fds;

	pollfd server_fd;
	server_fd.fd = server_socket;
	server_fd.events = POLLIN;
	fds.push_back(server_fd);

	while (true)
	{
		int status = ::poll(fds.data(), fds.size(), TIMEOUT_MS);
		if (status < 0)
			throw SocketPollingError();
		if (fds[0].revents & POLLIN)
		{
			sockaddr_in client_address;
			bzero(&client_address, sizeof(client_address));
			socklen_t	client_len = sizeof(client_address);
			int			client_socket = accept(fds[0].fd, (struct sockaddr *)&client_address, &client_len);
			if (client_socket > 0)
			{
				if (fcntl(client_socket, F_SETFL, O_NONBLOCK) != -1)
				{
					pollfd	client;
					client.fd = client_socket;
					client.events = POLLIN;
					fds.push_back(client);
				}
			}
		}
		for (std::vector<pollfd>::iterator it = std::next(fds.begin(), 1); it != fds.end();)
		{
			if (it->revents & POLLIN)
			{
				char buff[1024];
				bzero(buff, sizeof(buff));
				recv(it->fd, buff, sizeof(buff), 0);
				std::cout << "Read from fd " << it->fd << " : "<< buff << std::endl;
				close(it->fd);
				it = fds.erase(it);
			}
			else
				++it;
		}
	}
}

WebServer	&WebServer::operator=(const WebServer &copy)
{
	if (this != &copy)
	{
		server_address = copy.server_address;
		server_socket = copy.server_socket;
	}
	return *this;
}

int	WebServer::get_server_socket(void) const
{
	return server_socket;
}

sockaddr_in	WebServer::get_server_address(void) const
{
	return server_address;
}
