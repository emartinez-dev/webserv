#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

# include "ErrorPage.hpp"
# include "Listen.hpp"
# include "Location.hpp"
# include "webserv.hpp"

class ServerConfig
{
	private:
		std::map<std::string, std::string> conf;
		std::vector<Location> locations;
		std::vector<Listen> listens;
		std::vector<ErrorPage> errorPages;

	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(ServerConfig const &copy);
		ServerConfig	&operator=(ServerConfig const &copy);

		void printConfig(void) const;
		void splitKeyValue(std::string &line, std::ifstream &configFile);

		std::map<std::string, std::string> getConf();
		std::vector<Location> getLocations();
		std::vector<Listen> getListens();
		std::vector<ErrorPage> getErrorPages();
		const std::string getValue(std::string const &key) const;

		ServerConfig parseServer(std::string &line, std::ifstream &configFile);
		bool  matchesHostname(std::string const &host) const;
		bool  matchesIP(std::string const &host) const;
		const Location *getLocation(std::string const &url) const;
		std::string getErrorPage(int statusCode) const;
		int checkServerConfig();
};

#endif
