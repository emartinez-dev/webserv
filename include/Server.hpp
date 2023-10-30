#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include "webserv.hpp"
# include <errno.h>

class Server
{
	private:
		sockaddr_in	serverAddress;
		int			serverSocket;
		Server();

	public:
		Server(std::string const &ip_address, int port);
		~Server();
		Server(Server const &copy);
		Server	&operator=(Server const &copy);
		int			  getServerSocket(void) const;
		sockaddr_in	  getServerAddress(void) const;
};

#endif
