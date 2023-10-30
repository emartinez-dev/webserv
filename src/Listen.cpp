#include "Listen.hpp"

Listen::Listen()
{
}

Listen::~Listen()
{
}

Listen::Listen(Listen const &copy):host(copy.host),port(copy.port)
{	
}

Listen	&Listen::operator=(const Listen &copy)
{
	if (this != &copy) {
		host = copy.host;
		port = copy.port;
	}
	return *this;
}

void Listen::setHost(std::string value) {
	host = value;
}

void Listen::setPort(std::string value) {
	port = atoi(value.c_str());

}

const std::string &Listen::getHost() const {
	return (host);
}

int Listen::getPort() const {
	return (port);
}
