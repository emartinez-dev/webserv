#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Server.hpp"
# include "httpRequest.hpp"
# include <iostream>
# include <unordered_map>
# include "Config.hpp"
# include "Response.hpp"
# include <cstring>
# include <algorithm>
# include <sys/time.h>

# define TIMEOUT_MS 2500
# define BUFFER_SIZE 1024
# define TIMEOUT_SEC 3

class Cluster
{
	private:
		const Config		&cluster_config;
		std::vector<Server> servers;
		std::vector<int>	servers_fd;
		std::vector<pollfd>	connections;
		std::unordered_map<int, std::vector<char> > connection_buffers;
		std::unordered_map<int, ssize_t> bytes_sent;
		std::unordered_map<int, HttpRequest> requests;
		std::unordered_map<int, Response> responses;
		std::unordered_map<int, time_t> timeouts;

		int		add_client(int server_fd);
		int		accept_client(int server_fd);
		int		receive(pollfd const &connection);
		ssize_t	read_socket(pollfd const &connection, char *buffer, size_t buffer_size);
		int		send(pollfd const &connection, Response const &response);
		void	close_and_remove_connection(size_t &i, size_t &initial_size);
		bool	is_server(int fd);
		bool	is_timeout(int i);
		pollfd  create_pollfd(int fd, short mode);

	public:
		Cluster(const Config &config);
		~Cluster();
		Cluster(Cluster const &copy);
		Cluster	&operator=(Cluster const &copy);

		void	add_server(std::string const &address, int port);
		void	run(void);
		};

#endif
