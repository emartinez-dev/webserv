#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Server.hpp"
# include <iostream>
#include <unordered_map>

# define TIMEOUT_MS 2500
# define BUFFER_SIZE 1024

class Cluster
{
	private:
		std::vector<Server> servers;
		std::vector<int>	servers_fd;
		std::vector<pollfd>	connections;
		std::unordered_map<int, std::vector<char> > connection_buffers;

	public:
		Cluster();
		~Cluster();
		Cluster(Cluster const &copy);
		Cluster	&operator=(Cluster const &copy);

		void	add_server(std::string const &address, int port);
		int		add_client(int server_fd);
		int		accept_client(int server_fd);

		void	poll(void);
		int		read_from_socket(pollfd const &connection);
		int		write_to_socket(pollfd const &connection);
		void	close_and_remove_connection(size_t &i, size_t &initial_size);

		bool	is_server(int fd);
		pollfd  create_pollfd(int fd, short mode);
};

#endif
