#include "webserv.hpp"
#include "Config.hpp"
#include "Cluster.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong number of params. Usage: ./webserv [config_file]\n";
		return (1);
	}
	std::string str(argv[1]);
	try {
		Config configuration(str);
		Cluster	webserv(configuration);
		webserv.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
