#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>

# include "WebServerExceptions.hpp"

# define MAX_BACKLOG 128

class WebServer
{
	private:
		sockaddr_in	server_address;
		int			server_socket;

	public:
		WebServer();
		WebServer(std::string const &ip_address, int port);
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer	&operator=(WebServer const &copy);

		int			  get_server_socket(void) const;
		sockaddr_in	  get_server_address(void) const;
};


#endif
