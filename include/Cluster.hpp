#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Server.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "Response.hpp"
# include "webserv.hpp"

class Cluster
{
	private:
		const Config		&clusterConfig;
		std::vector<Server> servers;
		std::vector<int>	serversFd;
		std::vector<pollfd>	connections;
		std::unordered_map<int, std::vector<char> > connectionBuffers;
		std::unordered_map<int, ssize_t> bytesSent;
		std::unordered_map<int, Request> requests;
		std::unordered_map<int, Response> responses;
		std::unordered_map<int, time_t> timeouts;

		int		addClient(int serverFd);
		int		acceptClient(int serverFd);
		int		receive(pollfd const &connection);
		ssize_t	readSocket(pollfd const &connection, char *buffer, size_t bufferSize);
		int		send(pollfd const &connection, Response const &response);
		void	closeConnection(size_t &i, size_t &initialSize);
		bool	isServer(int fd);
		bool	isTimeout(int i);
		pollfd  newPollfd(int fd, short mode);

	public:
		Cluster();
		Cluster(const Config &config);
		~Cluster();
		Cluster(Cluster const &copy);
		Cluster	&operator=(Cluster const &copy);

		void	addServer(std::string const &address, int port);
		void	run(void);
};

#endif
