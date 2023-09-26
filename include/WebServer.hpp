#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>

# include "WebServerExceptions.hpp"

class WebServer
{
	private:
		std::string ip_address;
		int			port;

	public:
		WebServer();
		WebServer(std::string const &ip_address, int port);
		~WebServer();
		WebServer(WebServer const &copy);
		WebServer	&operator=(WebServer const &copy);

		int	start(void);
		
		sockaddr_in	  config_address(void);
		unsigned long parse_ip(const std::string &ip_address);

};

#endif
