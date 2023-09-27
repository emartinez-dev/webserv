#include "Cluster.hpp"
#include <bitset>
#include <sys/signal.h>

Cluster::Cluster()
{
}

Cluster::~Cluster()
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end();it++)
		close(it->get_server_socket());
}

Cluster::Cluster(Cluster const &copy):servers(copy.servers),
	connections(copy.connections)
{
}

Cluster	&Cluster::operator=(const Cluster &copy)
{
	if (this != &copy) {
		servers = copy.servers;
		connections = copy.connections;
	}
	return *this;
}

void	Cluster::add_server(std::string const &address, int port)
{
	Server	server(address, port);
	pollfd	server_poll;

	server_poll = create_pollfd(server.get_server_socket(), POLLIN);
	servers.push_back(server);
	connections.push_back(server_poll);
	servers_fd.push_back(server.get_server_socket());
	std::cout << "New server fd: " << server.get_server_socket() << std::endl;
}

pollfd Cluster::create_pollfd(int fd, short mode)
{
	pollfd server_fd;
	server_fd.fd = fd;
	server_fd.events = mode;
	server_fd.revents = 0;
	return server_fd;
}

int	Cluster::accept_client(int server_fd)
{
	sockaddr_in client_address;
	int			client_socket;
	socklen_t	client_len = sizeof(client_address);

	bzero(&client_address, sizeof(client_address));
	client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
	return client_socket;
}

void  Cluster::poll(void)
{
	while (true)
	{
		int status = ::poll(connections.data(), connections.size(), TIMEOUT_MS);
		if (status < 0)
			throw SocketPollingError();
		for (std::vector<pollfd>::iterator it = connections.begin(); it != connections.end();)
		{
			if (it->revents & POLLIN)
			{
				if (is_server(it->fd))
				{
					int client_socket = accept_client(it->fd);
					if (client_socket > 0 && fcntl(client_socket, F_SETFL, O_NONBLOCK) != -1)
						connections.push_back(create_pollfd(client_socket, POLLIN));
					++it;
				}
				else 
				{
					char buff[1024];
					bzero(buff, sizeof(buff));
					recv(it->fd, buff, sizeof(buff), 0);
					std::cout << "Read from fd " << it->fd << " : "<< buff << std::endl;
					close(it->fd);
					it = connections.erase(it);
				}
			}
			else if ((it->revents & POLLHUP) || (it->revents & POLLERR))
			{
				std::cout << "Closing connection due to POLLHUP or POLLERR on fd " << it->fd << std::endl;
				close(it->fd);
				it = connections.erase(it);
			}
			else
				++it;
		}
	}
}

int	Cluster::is_server(int fd)
{
	if (std::find(servers_fd.begin(), servers_fd.end(), fd) == servers_fd.end())
		return (0);
	std::cout << "fd " << fd << " is a server\n";
	return (1);
}
