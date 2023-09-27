#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Server.hpp"
# include <iostream>

# define TIMEOUT_MS 2500

class Cluster
{
	private:
		std::vector<Server> servers;
		std::vector<int>	servers_fd;
		std::vector<pollfd>	connections;

	public:
		Cluster();
		~Cluster();
		Cluster(Cluster const &copy);
		Cluster	&operator=(Cluster const &copy);

		void	add_server(std::string const &address, int port);
		pollfd  create_pollfd(int fd, short mode);
		int		accept_client(int server_fd);
		void	poll(void);
		int		is_server(int fd);
};

#endif
