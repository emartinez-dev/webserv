#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include <poll.h>

# include "WebServerExceptions.hpp"

# define MAX_BACKLOG 128
# define MAX_FDS 10
# define TIMEOUT_MS 2500

class WebServer
{
	private:
		sockaddr_in	server_address;
		int			server_socket;
		WebServer();

	public:
		WebServer(std::string const &ip_address, int port);
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer	&operator=(WebServer const &copy);
		void		  poll(void);

		int			  get_server_socket(void) const;
		sockaddr_in	  get_server_address(void) const;
};


#endif
