#include "webserv.hpp"
#include "Cgi.hpp"

int	main(int argc, char **argv, char **envp)
{
	(void) argv;
	if (argc != 2)
	{
		std::cout << "Wrong number of params. Usage: ./webserv [config_file]\n";
		return (1);
	}

	// Necesito aceptar una del cliente y obtener un descriptor del socket
	std::string output = runCGI("/usr/bin/python", "test.py", envp);
	std::cout << output << std::endl;




	// std::string str(argv[1]);
	// Cluster	webserv;
	// Config configuration(str);
	// std::vector<ServerConfig> server_configs = configuration.getServerConfigs();

	// try {
	// 	for (int i = server_configs.size() - 1; i >= 0; i--)
	// 	{
	// 		for (int j = server_configs[i].getListens().size() - 1; j >= 0; j--)
	// 		{
	// 			Listen server_listens = server_configs[i].getListens()[j];
	// 			webserv.add_server(server_listens.getHost(), server_listens.getPort());
	// 		}
	// 	}
	// 	webserv.poll();
	// } catch (std::exception &e) {
	// 	std::cerr << e.what() << std::endl;
	// 	return (1);
	// }
	return (0);
}
