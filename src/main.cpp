#include "webserv.hpp"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	WebServer webserv("127.0.0.1", 8080);
	webserv.poll();
	return (0);
}
