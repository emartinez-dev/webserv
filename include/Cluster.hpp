#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Server.hpp"
# include "httpRequest.hpp"
# include <iostream>
# include <unordered_map>
# include "Config.hpp"
# include "Response.hpp"

# define TIMEOUT_MS 2500
# define BUFFER_SIZE 1024

class Cluster
{
	private:
		const Config		&cluster_config;
		std::vector<Server> servers;
		std::vector<int>	servers_fd;
		std::vector<pollfd>	connections;
		std::unordered_map<int, std::vector<char> > connection_buffers;

	public:
		Cluster(const Config &config);
		~Cluster();
		Cluster(Cluster const &copy);
		Cluster	&operator=(Cluster const &copy);

		void	add_server(std::string const &address, int port);
		int		add_client(int server_fd);
		int		accept_client(int server_fd);

		void	poll(void);
		HttpRequest	read_from_socket(pollfd const &connection);
		int		write_to_socket(pollfd const &connection, Response const &response);
		void	close_and_remove_connection(size_t &i, size_t &initial_size);

		bool	is_server(int fd);
		pollfd  create_pollfd(int fd, short mode);
};

#endif
