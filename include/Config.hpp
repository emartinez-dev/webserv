#ifndef CONFIG_HPP
#define CONFIG_HPP

# include "ServerConfig.hpp"
# include "ErrorPage.hpp"
# include "Listen.hpp"
# include "Location.hpp"
# include "webserv.hpp"

class ServerConfig;

class Config
{
	private:
		std::vector<ServerConfig> servers;

		int	  checkFile(std::ifstream &configFile);
		void  parseServer(std::ifstream &configFile, std::string line);
		int	  checkServersConfig();

	public:
		Config(std::string &inputfile);
		~Config();
		Config(Config const &copy);
		Config	&operator=(Config const &copy);

		const std::vector<ServerConfig> &getServerConfigs() const;
		const ServerConfig				*getServer(std::string const &host) const;
};

#endif
