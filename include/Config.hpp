#ifndef CONFIG_HPP
#define CONFIG_HPP

# include "ServerConfig.hpp"
# include "ErrorPage.hpp"
# include "Listen.hpp"
# include "Location.hpp"
# include "webserv.hpp"

# define LAST std::string::npos

class ServerConfig;

class Config
{
	private:
		Config();
		std::vector<ServerConfig> servers;
	public:
		Config(std::string &inputfile);
		~Config();
		Config(Config const &copy);
		Config	&operator=(Config const &copy);

		int	check_file(std::ifstream &config_file);

		const std::vector<ServerConfig> getServerConfigs() const;

		void add_server(std::ifstream &config_file, std::string line);
		const ServerConfig *getServer(std::string const &host) const;
		int checkValues();
};

#endif
