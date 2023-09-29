#ifndef CONFIG_HPP
#define CONFIG_HPP

# include <iostream>
# include <fstream>
# include "ServerConfig.hpp"
# include "ErrorPage.hpp"
# include "Listen.hpp"
# include "Location.hpp"

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

		std::vector<ServerConfig> getServerConfigs();

};

#endif
