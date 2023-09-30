#include "webserv.hpp"
#include "Config.hpp"
# include "ServerConfig.hpp"
# include "ErrorPage.hpp"
# include "Listen.hpp"
# include "Location.hpp"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	std::vector<ServerConfig> server_configs;

	std::string str(argv[1]);
	Config pepito(str);
	server_configs = pepito.getServerConfigs();

	std::cout << server_configs[0].getLocations()[2].getAllowMethods() << std::endl;

	std::cout << server_configs[1].getConf()["name"] << std::endl;
	// std::cout << server_configs[2].getConf()["name"] << std::endl;
	return (0);
}
