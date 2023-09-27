#include "webserv.hpp"
#include "config.hpp"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	std::string str(argv[1]);
	Config config(str);
	return (0);
}
