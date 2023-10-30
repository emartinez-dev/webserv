#include "Config.hpp"

Config::Config(std::string &inputfile)
{
	std::ifstream config_file(inputfile);

	if (checkFile(config_file))
		throw ConfigurationFileException();
	if (checkServersConfig())
		throw IncorrectConfigurationFileException();

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

int	Config::checkFile(std::ifstream &config_file)	{
	
	std::string line;
	if (!config_file.is_open())
		return 1;
	std::getline(config_file, line);
	if (line.find("server:") == std::string::npos) {
		std::cerr << "error: file" << std::endl;
		return 1;
	}
	else
		parseServer(config_file, line);
	return 0;
}

const std::vector<ServerConfig> &Config::getServerConfigs() const {
	return servers;
}

void Config::parseServer(std::ifstream &config_file, std::string line)
{
	ServerConfig	conf;

	while (std::getline(config_file, line))
	{
		if (line.empty()) continue;
		conf.splitKeyValue(line, config_file);
		if (line.find("server:") != std::string::npos)
			parseServer(config_file, line);
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

int Config::checkServersConfig() {
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].checkServerConfig())
			return 1;
	}
	return 0;
}
