#include "Cluster.hpp"
#include <bitset>
#include <sys/errno.h>
#include <sys/poll.h>
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
	std::cout << "New server: socket -> " << server_poll.fd << std::endl;
	servers.push_back(server);
	connections.push_back(server_poll);
	servers_fd.push_back(server_poll.fd);
}

int		Cluster::new_client_connection(int server_fd)
{
	int client_socket = accept_client(server_fd);
	if (client_socket > 0 && fcntl(client_socket, F_SETFL, O_NONBLOCK) != -1)
	{
		connections.push_back(create_pollfd(client_socket, POLLIN));
		connection_buffers[client_socket] = std::vector<char>();
	}
	else
	{
		std::cout << "Error accepting client" << std::endl;
		return (0);
	}
	return (1);
}

pollfd	Cluster::create_pollfd(int fd, short mode)
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

int	Cluster::read_from_socket(pollfd const &connection)
{
	size_t	  bytes_read;
	do
	{
		char  buff[BUFFER_SIZE];
		bzero(buff, BUFFER_SIZE);
		bytes_read = recv(connection.fd, buff, BUFFER_SIZE, 0);
		if (bytes_read > 0)
		{
			connection_buffers[connection.fd].insert(
				connection_buffers[connection.fd].end(),
				buff,
				buff + bytes_read
			);
		}
	} while (bytes_read > 0);
	if (bytes_read == 0)
	{
		std::cout << "Read from fd " << connection.fd << " : "<< connection_buffers[connection.fd].data() << std::endl;
		connection_buffers[connection.fd].clear();
	}
	if (bytes_read == (size_t)-1 && (errno != EWOULDBLOCK && errno != EAGAIN))
		std::cout << "Error reading\n";
	return (1);
}

int	Cluster::write_to_socket(pollfd const &connection)
{
	/*
	const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 48\r\n\r\n<html><body><h1>Hello, World</h1></body></html>\r\n\r\n";

	send(connection.fd, response, strlen(response), 0);
	*/
	(void) connection;
	return (0);
}

void  Cluster::poll(void)
{
	while (true)
	{
		size_t initial_size = connections.size();
		int status = ::poll(connections.data(), connections.size(), TIMEOUT_MS);
		if (status < 0)
			throw SocketPollingError();
		for (size_t i = 0; i < initial_size; i++)
		{
			if (connections[i].revents & POLLIN)
			{
				if (is_server(connections[i].fd))
					 new_client_connection(connections[i].fd);
				else 
				{
					read_from_socket(connections[i]);
					write_to_socket(connections[i]);
					close(connections[i].fd);
					connections.erase(connections.begin() + i);
					--i;
					--initial_size;
				}
			}
			else if ((connections[i].revents & POLLHUP) || (connections[i].revents & POLLERR))
			{
				std::cout << "Closing connection due to POLLHUP or POLLERR on fd " << connections[i].fd << std::endl;
				close(connections[i].fd);
				connections.erase(connections.begin() + i);
				--i;
				--initial_size;
			}
		}
	}
}

int	Cluster::is_server(int fd)
{
	if (std::find(servers_fd.begin(), servers_fd.end(), fd) == servers_fd.end())
		return (0);
	return (1);
}
