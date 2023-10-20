#include "webserv.hpp"
#include "Cgi.hpp"

int	main(int argc, char **argv, char **envp)
{
	(void) envp;
	if (argc != 2)
	{
		std::cout << "Wrong number of params. Usage: ./webserv [config_file]\n";
		return (1);
	}
	std::string str(argv[1]);
	Config configuration(str);
	try {
		Cluster	webserv(configuration);
		webserv.run(envp);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
