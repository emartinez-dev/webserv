#include "webserv.hpp"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	Cluster	webserv;

	webserv.add_server("0.0.0.0", 8081);
	webserv.add_server("127.0.0.1", 8080);
	webserv.poll();
	return (0);
}
