#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "ErrorPage.hpp"
# include "Config.hpp"
# include "Listen.hpp"
# include "Location.hpp"
# include "Utils.hpp"
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

		void printConfig(void) const;
		void splitKeyValue(std::string &line, std::ifstream &config_file);

		std::map<std::string, std::string> getConf();
		std::vector<Location> getLocations();
		std::vector<Listen> getListens();
		std::vector<ErrorPage> getErrorPages();
		const std::string getValue(std::string const &key) const;

		ServerConfig parseServer(std::string &line, std::ifstream &config_file);
		bool  matchesHostname(std::string const &host) const;
		bool  matchesIP(std::string const &host) const;
		const Location *getLocation(std::string const &url) const;
		std::string getErrorPage(int status_code) const;
		int checkServerConfig();
};

#endif
