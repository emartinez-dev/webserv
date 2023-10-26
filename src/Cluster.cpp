#include "Cluster.hpp"
#include "Response.hpp"
#include "Request.hpp"

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

void  Cluster::run(void)
{
	while (true)
	{
		size_t initial_size = connections.size();
		int events = ::poll(connections.data(), connections.size(), POLL_TIMEOUT_MS);
		if (events < 0)
			throw SocketPollingError();
		else
		{
			for (size_t i = 0; i < initial_size; i++)
			{
				int fd = connections[i].fd;
				if (!is_server(fd) && is_timeout(fd))
				{
					responses[fd] = Response(HTTP_STATUS_REQUEST_TIMEOUT);
					connections[i].events = POLLOUT;
				}
				if (connections[i].revents & POLLIN)
				{
					if (is_server(fd))
						 add_client(fd);
					else
					{
						int finish = receive(connections[i]);
						/* I can't get the server that is receiving the request, so I can't cut the connection 
						 * earlier if the request is too long */
						if (finish == 1)
						{
							connections[i].events = POLLOUT;
							bytes_sent[fd] = 0;
							requests[fd] = Request(connection_buffers[fd]);
							responses[fd] = Response(requests[fd], cluster_config);
						}
						else if (finish == -1)
							close_and_remove_connection(i, initial_size);
					}
				}
				if ((connections[i].revents & POLLOUT))
				{
					int finish = 0;
					if (responses.find(fd) != responses.end())
						finish = send(connections[i], responses[fd]);
					if (finish == 1 || finish == -1)
					{
						requests.erase(fd);
						responses.erase(fd);
						close_and_remove_connection(i, initial_size);
					}
				}
				if ((connections[i].revents & POLLHUP) || (connections[i].revents & POLLERR))
				{
					std::cout << "Closing connection due to POLLHUP or POLLERR on fd " << connections[i].fd << std::endl;
					close_and_remove_connection(i, initial_size);
				}
			}
		}
	}
}

Cluster::~Cluster()
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end();it++)
		close(it->get_server_socket());
}

Cluster::Cluster(Cluster const &copy):cluster_config(copy.cluster_config),servers(copy.servers),
	connections(copy.connections),connection_buffers(copy.connection_buffers),bytes_sent(copy.bytes_sent),
	requests(copy.requests),responses(copy.responses)
{
}

Cluster	&Cluster::operator=(const Cluster &copy)
{
	if (this != &copy) {
		servers = copy.servers;
		servers_fd = copy.servers_fd;
		connections = copy.connections;
		connection_buffers = copy.connection_buffers;
		bytes_sent = copy.bytes_sent;
		requests = copy.requests;
		responses = copy.responses;
	}
	return *this;
}

int		Cluster::add_client(int server_fd)
{
	int client_socket = accept_client(server_fd);
	if (client_socket > 0 && fcntl(client_socket, F_SETFL, O_NONBLOCK) != -1)
	{
		connections.push_back(create_pollfd(client_socket, POLLIN));
		connection_buffers[client_socket] = std::vector<char>();
		timeouts[client_socket] = time(NULL) + CLIENT_TIMEOUT_S;
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

int  Cluster::receive(pollfd const &connection)
{
	ssize_t	  bytes_read;
	char	  buff[READ_BUFFER_SIZE];

	bytes_read = read_socket(connection, buff, READ_BUFFER_SIZE);
	if (bytes_read > 0)
	{
		connection_buffers[connection.fd].insert(
			connection_buffers[connection.fd].end(),
			buff,
			buff + bytes_read
		);
	}
	if (bytes_read == -1)
	{
		std::cout << "Error reading\n";
		return -1;
	}
	else
	{
		// This way of reading if a request is complete is slowing our program a lot I think
		Request	readRequest(connection_buffers[connection.fd]);
		if (readRequest.receivedHeaders() && readRequest.receivedBody())
			return 1;
	}
	return 0;
}

ssize_t	Cluster::read_socket(pollfd const &connection, char *buffer, size_t buffer_size)
{
	std::memset(buffer, 0, buffer_size);
	ssize_t bytes_read = recv(connection.fd, buffer, buffer_size, 0);
	return bytes_read;
}

int	Cluster::send(pollfd const &connection, Response const &response)
{
	ssize_t sent = bytes_sent[connection.fd];

	int status = ::send(connection.fd, response.getContent().data() + sent, response.getContent().size() - sent, 0);
	if (status == -1)
		return (-1);
	else if (sent + status == static_cast<ssize_t>(response.getContent().size()))
		return (1);
	bytes_sent[connection.fd] += status;
	return (0);
}

void  Cluster::close_and_remove_connection(size_t &i, size_t &initial_size)
{
	connection_buffers[connections[i].fd].clear();
	timeouts.erase(connections[i].fd);
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

bool  Cluster::is_timeout(int fd)
{
	return (timeouts[fd] < time(NULL));
}

pollfd	Cluster::create_pollfd(int fd, short mode)
{
	pollfd poll_fd;
	poll_fd.fd = fd;
	poll_fd.events = mode;
	poll_fd.revents = 0;
	return poll_fd;
}
