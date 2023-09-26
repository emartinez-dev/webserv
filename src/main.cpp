#include "webserv.hpp"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	WebServer webserv("127.0.0.1", 8080);
	//client accept code
	while (1)
	{
		sockaddr_in client_address;
		bzero(&client_address, sizeof(client_address));
		socklen_t	client_len = sizeof(client_address);
		int			client_socket = accept(webserv.get_server_socket(), (struct sockaddr *)&client_address, &client_len);
		char		buff[1024];
		bzero(buff, sizeof(buff));

		if (client_socket < 0)
			continue;
		std::cout << "Accept OK!" << std::endl;
		while (read(client_socket, buff, 1023) > 0)
			std::cout << buff;
		std::cout << std::endl;
		close(client_socket);
	}
	return (0);
}
