#ifndef CONFIG_HPP
#define CONFIG_HPP

# include <iostream>
# include <fstream>
# include "server_conf.hpp"

# define LAST std::string::npos

class Config
{
	private:
		Config();

	public:
		Config(std::string &inputfile);
		~Config();
		Config(Config const &copy);
		Config	&operator=(Config const &copy);

		int	check_file(std::ifstream &config_file);
};

#endif
