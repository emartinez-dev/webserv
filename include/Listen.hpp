#ifndef LISTEN_HPP
#define LISTEN_HPP

# include <iostream>
#include <cstdlib>

class Listen
{
	private:
		std::string host;
		int port;
	public:
		Listen();
		~Listen();
		Listen(Listen const &copy);
		Listen	&operator=(Listen const &copy);
		
		void setHost(std::string value);
		void setPort(std::string value);
		
		const std::string getHost() const;
		int getPort() const;
};

#endif
