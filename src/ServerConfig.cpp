#include "ServerConfig.hpp"
#include "ErrorPage.hpp"
#include "Utils.hpp"
#include <string>

ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(ServerConfig const &copy):conf(copy.conf),locations(copy.locations),listens(copy.listens),error_pages(copy.error_pages)
{
}

ServerConfig	&ServerConfig::operator=(const ServerConfig &copy)
{
	if (this != &copy)
	{
		conf = copy.conf;
		locations = copy.locations;
		listens = copy.listens;
		error_pages = copy.error_pages;
	}
	return *this;
}

void	ServerConfig::printConfig(void) const
{
	for(std::map<std::string, std::string>::const_iterator it=conf.begin(); it != conf.end(); ++it)
		std::cout << "Clave: " << it->first << " valor: " << it->second << std::endl;
	for (size_t i = 0; i < locations.size(); i++)
	{
		std::cout << "Location " << i << std::endl;
		locations[i].printConfig();
	}
}


void	ServerConfig::splitKeyValue(std::string &line, std::ifstream &config_file) {
	size_t pointsPos = line.find(":");
	if (line.find("server:") != std::string::npos)
		return ;
	if (pointsPos != LAST) {
		std::string key = splitKey(line);
		std::string value = splitValue(line);
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
		else if (key == "error_page")
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
		else if (key == "server:")
			return ;
		else
			conf[key] = value;
	}
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

// TODO: abstract this to a function or something, we repeat this a lot
const std::string ServerConfig::getValue(std::string const &key) const
{
	return (getMapValue(key, this->conf));
}


// This functions also remove the trailing \r
// TODO: abstract this to a function or something
static const std::string getHostname(std::string const &host)
{
	if (host.find(":") == std::string::npos)
		return (host.substr(0, host.length() - 1));
	std::string	hostname = host.substr(0, host.find(":"));
	return (hostname);
}

std::string getPort(std::string const &host)
{
	if (host.find(":") == std::string::npos)
		return ("");
	std::string	hostname = host.substr(host.find(":") + 1, host.length() - 1);
	return (hostname);

}

bool  ServerConfig::matchesHostname(std::string const &host) const
{
	std::string hostname = getHostname(host);
	std::string port = getPort(host);

	int	int_port = atoi(port.c_str());
	for (size_t i = 0; i < listens.size(); i++)
	{
		if (hostname == getValue("server_name") && int_port == listens[i].getPort())
			return (true);
	}
	return (false);
}

bool  ServerConfig::matchesIP(std::string const &host) const
{
	std::string hostname = getHostname(host);
	std::string port = getPort(host);

	int	int_port = atoi(port.c_str());
	for (size_t i = 0; i < listens.size(); i++)
	{
		if (listens[i].getPort() == int_port && listens[i].getHost() == hostname)
			return (true);
	}
	return (false);
}



/* In our webserver context, we have to match the URL given with the prefixes on
 * the config file. Example:
 * We have a URL like /projects/alumni and a route with /projects and other with 
 * /projects/alumni on the configuration file. We have to redirect the browser to
 * the most specific one, being it the longest one */

static int prefixMatch(std::string const &str, std::string const &prefix)
{
	if (str.length() < prefix.length())
		return (0);
	if (str.compare(0, prefix.length(), prefix) == 0)
		return (prefix.length());
	return (0);
}

const Location *ServerConfig::getLocation(std::string const &url) const
{
	Location const	*best_match = NULL;
	int				best_match_len = -1;
	int				temp_len;

	for (size_t i = 0; i < locations.size(); i++)
	{
		std::string const &route = locations[i].getValue("route");
		temp_len = prefixMatch(url, route);
		if (temp_len > best_match_len)
		{
			best_match_len = temp_len;
			best_match = &locations[i];
		}
	}
	if (best_match_len > 0)
		return best_match;
	else
		return (NULL);
}

std::string ServerConfig::getErrorPage(int status_code) const
{
	for (std::vector<ErrorPage>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it)
	{
		if (status_code == it->getCode())
			return it->getPath();
	}
	return "";
}

int ServerConfig::checkServerConfig() {
	if (getValue("root") == "")
		return 1;
	if (!isAccessible(getValue("cgi_path")))
		return 1;
	for (size_t i = 0; i < locations.size(); i++) {
		if ((locations[i].getValue("root") == "" && locations[i].getValue("return") == "" ) || locations[i].getValue("route") == "")
				return 1;
	}
	for (size_t i = 0; i < listens.size(); i++) {
		if (listens[i].getPort() <= 0 || listens[i].getPort() >= 65535)
			return 1;
	}
	return 0;
} 
