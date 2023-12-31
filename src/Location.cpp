#include "Location.hpp"
#include "Utils.hpp"

void	Location::printConfig(void) const
{
	for(std::map<std::string, std::string>::const_iterator it=conf.begin(); it != conf.end(); ++it)
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
	this->allowedMethods = copy.allowedMethods;
}

Location	&Location::operator=(const Location &copy)
{
	if (this != &copy) {
		this->conf = copy.conf;
		this->allowedMethods = copy.allowedMethods;
	}
	return *this;
}

void	Location::setConf(const std::string &key, const std::string &value) {
	conf[key] = value;
}

std::map<std::string, std::string>	Location::getConf() {
	return(conf);
}

int	Location::getAllowedMethods() const{
	int methods = 0;
	if (getValue("allow_methods").find("GET") != LAST)
		methods += 1;
	if (getValue("allow_methods").find("POST") != LAST)
		methods += 3;
	if (getValue("allow_methods").find("DELETE") != LAST)
		methods += 5;
	return methods;
}

bool  Location::matches(const std::string &path)
{
	if (path == conf["route"])
		return true;
	return false;
}

const std::string Location::getValue(std::string const &key) const
{
	return (getMapValue(key, this->conf));
}

bool  Location::hasRedirect(void) const
{
	return (getValue("return") != "");
}
