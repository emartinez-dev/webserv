#ifndef SERVER_CONF_HPP
#define SERVER_CONF_HPP

# include <iostream>
# include <map>
#include "config.hpp"


class server_conf
{
	private:
		std::map<std::string, std::string> conf;

		// error_pages;
		// listen;
		// location;

	public:
		server_conf();
		// ~server_conf();
		// server_conf(server_conf const &copy);
		// server_conf	&operator=(server_conf const &copy);

		void splitKeyValue(std::string const &line);
};

#endif
