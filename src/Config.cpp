#include "Config.hpp"
#include <string.h>

Config::Config(std::string &inputfile)
{
	std::ifstream config_file(inputfile);

	if (check_file(config_file))
		throw ConfigurationFileException();
}

Config::~Config() {

}

Config::Config(Config const &copy):servers(copy.servers){
}

Config	&Config::operator=(Config const &copy) {
	if (this != &copy) {
		servers = copy.servers;
	}
	return *this;
}

int	Config::check_file(std::ifstream &config_file)	{
	
	std::string line;
	if (!config_file.is_open())
		return 1;
	std::getline(config_file, line);
	if (line.find("server:") == std::string::npos) {
		std::cerr << "error: file" << std::endl;
		return 1;
	}
	else
		add_server(config_file, line);
return 0;
}

const std::vector<ServerConfig> Config::getServerConfigs() const {
	return servers;
}

void Config::add_server(std::ifstream &config_file, std::string line)
{
	ServerConfig	conf;

	while (std::getline(config_file, line))
	{
		if (line.empty()) continue;
		conf.splitKeyValue(line, config_file);
		if (line.find("server:") != std::string::npos)
			add_server(config_file, line);
	}
	servers.push_back(conf);
}

/* As the subject says, the first server will be the default if the request 
doesn't belong to another server. In our vector, the first server is the last 
one */
const ServerConfig *Config::getServer(std::string const &host) const
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].matchesHostname(host))
			return (&servers[i]);
	}
	for (ssize_t i = servers.size() - 1; i >= 0; i--)
	{
		if (servers[i].matchesIP(host))
			return (&servers[i]);
	}
	return (&servers[servers.size() - 1]);
}
