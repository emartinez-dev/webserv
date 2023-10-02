#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

# include <iostream>
# include <map>
# include <vector>
# include "Config.hpp"
# include "ErrorPage.hpp"
# include "Listen.hpp"
# include "Location.hpp"
# include <iostream>

class ServerConfig
{
	private:
		std::map<std::string, std::string> conf;
		std::vector<Location> locations;
		std::vector<Listen> listens;
		std::vector<ErrorPage> error_pages;

	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(ServerConfig const &copy);
		ServerConfig	&operator=(ServerConfig const &copy);

		void printConfig(void);
		void splitKeyValue(std::string &line, std::ifstream &config_file);
		std::string splitKey(std::string const &line);
		std::string splitValue(std::string const &line);

		const std::map<std::string, std::string> getConf();
		const std::vector<Location> getLocations();
		const std::vector<Listen> getListens();
		const std::vector<ErrorPage> getErrorPages();

		ServerConfig parseServer(std::string &line, std::ifstream &config_file);
};

#endif
