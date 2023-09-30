#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(ServerConfig const &copy)
{
	this->conf = copy.conf;
	this->locations = copy.locations;
	this->listens = copy.listens;
	this->error_pages = copy.error_pages;
}

ServerConfig	&ServerConfig::operator=(const ServerConfig &copy)
{
	this->conf = copy.conf;
	this->locations = copy.locations;
	this->listens = copy.listens;
	this->error_pages = copy.error_pages;
	return *this;
}

void	ServerConfig::printConfig(void)
{
	for(std::map<std::string, std::string>::iterator it=conf.begin(); it != conf.end(); ++it)
		std::cout << "Clave: " << it->first << " valor: " << it->second << std::endl;
	for (size_t i = 0; i < locations.size(); i++)
	{
		std::cout << "Location " << i << std::endl;
		locations[i].printConfig();
	}
}


void	ServerConfig::splitKeyValue(std::string &line, std::ifstream &config_file) {
	size_t pointsPos = line.find(":");
	//std::cout << line << std::endl;
	if (line.find("server") != std::string::npos)
		return ;
	if (pointsPos != LAST) {
		std::string key = splitKey(line);
		std::string value = splitValue(line);
		//std::cout << "key: " << key << " value: " << value << std::endl;
		if (key == "listen")
		{
			Listen listen;
			int i = 2;
			while (--i >= 0 && std::getline(config_file, line))
			{
				std::string key = splitKey(line);
				std::string value = splitValue(line);
				if (key == "host")
					listen.setHost(value);
				else if (key == "port")
					listen.setPort(value);
			}
			listens.push_back(listen);
		}
		else if (key == "error page")
		{
			ErrorPage error;
			int i = 2;
			while (--i >= 0 && std::getline(config_file, line))
			{
				std::string key = splitKey(line);
				std::string value = splitValue(line);
				if (key == "path")
					error.setPath(value);
				else if (key == "status_code")
					error.setCode(value);
			}
			error_pages.push_back(error);
		}
		else if (key == "location")
		{
			Location location;
			while (std::getline(config_file, line))
			{
				if ((line[0] == '\t' && line [1] == '\t') || (line.find("        ") == 0 && !std::isspace(line[8])))
					location.setConf(splitKey(line), splitValue(line));
				else
				{
					splitKeyValue(line, config_file);
					break ;
				}
			}
			locations.push_back(location);
		}
		else if (key == "server")
			return ;
		else
			conf[key] = value;
	}
}

std::string ServerConfig::splitKey(std::string const &line) {
	size_t pointsPos = line.find(":");
	std::string key = line.substr(0, pointsPos);
    key.erase(0, key.find_first_not_of(" \t"));
    key.erase(key.find_last_not_of(" \t") + 1);
	return key;
}

std::string ServerConfig::splitValue(std::string const &line) {
	size_t pointsPos = line.find(":");
	std::string value = line.substr(pointsPos + 1);
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);
	return value;
}

std::map<std::string, std::string> ServerConfig::getConf() {
	return conf;
}

std::vector<Location> ServerConfig::getLocations() {
	return locations;
}

std::vector<Listen> ServerConfig::getListens() {
	return listens;
}

std::vector<ErrorPage> ServerConfig::getErrorPages() {
	return error_pages;
}
