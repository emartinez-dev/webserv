#include "Listen.hpp"

Listen::Listen()
{
}

Listen::~Listen()
{
}

Listen::Listen(Listen const &copy)
{	
	this->host = copy.host;
	this->port = copy.port;
}

Listen	&Listen::operator=(const Listen &copy)
{
	this->host = copy.host;
	this->port = copy.port;
	return *this;
}

void Listen::setHost(std::string value) {
	host = value;
}

void Listen::setPort(std::string value) {
	port = atoi(value.c_str());

}

std::string Listen::getHost() {
	return(host);
}

int Listen::getPort() {
	return(port);
}
