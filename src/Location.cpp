#include "Location.hpp"

void	Location::printConfig(void)
{
	for(std::map<std::string, std::string>::iterator it=conf.begin(); it != conf.end(); ++it)
		std::cout << "\tLocation Clave: " << it->first << " valor: " << it->second << std::endl;
}

Location::Location()
{
}

Location::~Location()
{
}

Location::Location(Location const &copy)
{
	this->conf = copy.conf;
	this->allow_methods = copy.allow_methods;
}

Location	&Location::operator=(const Location &copy)
{
	this->conf = copy.conf;
	this->allow_methods = copy.allow_methods;
	return *this;
}

void	Location::setConf(std::string key, std::string value) {
	conf[key] = value;
}

std::map<std::string, std::string>	Location::getConf() {
	return(conf);
}

int	Location::getAllowMethods() {
	int methods = 0;
	if (conf["allow_methods"].find("GET") != LAST)
		methods += 1;
	if (conf["allow_methods"].find("POST") != LAST)
		methods += 3;
	if (conf["allow_methods"].find("DELETE") != LAST)
		methods += 5;
	return methods;
}