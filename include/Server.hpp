#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include <poll.h>

# include "WebServerExceptions.hpp"

# define MAX_BACKLOG 128

class Server
{
	private:
		sockaddr_in	server_address;
		int			server_socket;
		Server();

	public:
		Server(std::string const &ip_address, int port);
		~Server();
		Server(Server const &copy);
		Server	&operator=(Server const &copy);
		int			  get_server_socket(void) const;
		sockaddr_in	  get_server_address(void) const;
};

#endif