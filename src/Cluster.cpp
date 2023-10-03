#include "Cluster.hpp"
#include "Response.hpp"
#include "httpRequest.hpp"

Cluster::Cluster(const Config &config): cluster_config(config)
{
	std::vector<ServerConfig> server_configs = config.getServerConfigs();
	for (int i = server_configs.size() - 1; i >= 0; i--)
	{
		for (int j = server_configs[i].getListens().size() - 1; j >= 0; j--)
		{
			Listen server_listens = server_configs[i].getListens()[j];
			add_server(server_listens.getHost(), server_listens.getPort());
		}
	}
}

Cluster::~Cluster()
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end();it++)
		close(it->get_server_socket());
}

Cluster::Cluster(Cluster const &copy):cluster_config(copy.cluster_config),servers(copy.servers),
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
	servers_fd.push_back(server_poll.fd);
	std::cout << "Server listening at http://" << address << ":" << port << "/" << std::endl;
}

int		Cluster::add_client(int server_fd)
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
		size_t initial_size = connections.size();
		int status = ::poll(connections.data(), connections.size(), TIMEOUT_MS);
		if (status < 0)
			throw SocketPollingError();
		for (size_t i = 0; i < initial_size; i++)
		{
			if (connections[i].revents & POLLIN)
			{
				if (is_server(connections[i].fd))
					 add_client(connections[i].fd);
				else
				{
					HttpRequest request = read_from_socket(connections[i]);
					// check if request is complete
					Response response(request, cluster_config);
					write_to_socket(connections[i], response);
					close_and_remove_connection(i, initial_size);
				}
			}
			else if ((connections[i].revents & POLLHUP) || (connections[i].revents & POLLERR))
			{
				std::cout << "Closing connection due to POLLHUP or POLLERR on fd " << connections[i].fd << std::endl;
				close_and_remove_connection(i, initial_size);
			}
		}
	}
}

HttpRequest  Cluster::read_from_socket(pollfd const &connection)
{
	HttpRequest request;
	ssize_t	  bytes_read;
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
	if (bytes_read == -1 && (errno != EWOULDBLOCK && errno != EAGAIN))
	{
		std::cout << "Error reading\n";
		return request;
	}
	else if (bytes_read <= 0)
	{
		std::cout << "\n----------------------Parte socket----------------------\n\n";
		std::cout << "Bytes read: " << bytes_read << std::endl;
		std::string request_text(connection_buffers[connection.fd].begin(), connection_buffers[connection.fd].end());
		//std::cout << "Read from fd " << connection.fd << std::endl;
		HttpRequest	readRequest(request_text);
		std::cout << "Request text:\n" << request_text << std::endl;
		std::cout << "\n----------------------end socket----------------------\n\n";
		readRequest.printRequest();
		std::cout << std::endl;
		// here we use the buffer and we clear it after, we could create a 
		// Request object and process it here
		request = readRequest;
	}
	return request;
}

int	Cluster::write_to_socket(pollfd const &connection, Response const &response)
{
	if (send(connection.fd, response.getContent().c_str(), strlen(response.getContent().c_str()), 0) == -1)
	{
		std::cout << "Error writing\n";
		return (0);
	}
	return (1);
}

void  Cluster::close_and_remove_connection(size_t &i, size_t &initial_size)
{
	connection_buffers[connections[i].fd].clear();
	close(connections[i].fd);
	connections.erase(connections.begin() + i);
	--i;
	--initial_size;
}

bool  Cluster::is_server(int fd)
{
	if (std::find(servers_fd.begin(), servers_fd.end(), fd) == servers_fd.end())
		return (false);
	return (true);
}

pollfd	Cluster::create_pollfd(int fd, short mode)
{
	pollfd poll_fd;
	poll_fd.fd = fd;
	poll_fd.events = mode;
	poll_fd.revents = 0;
	return poll_fd;
}
