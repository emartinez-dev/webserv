#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include "webserv.hpp"
# include <errno.h>

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
