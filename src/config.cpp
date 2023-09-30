#include "Config.hpp"
#include <string.h>

Config::Config(std::string &inputfile)
{
	std::ifstream config_file(inputfile);

	check_file(config_file);
}

Config::~Config() {

}

Config::Config(Config const &copy) {
	*this = copy;
}

Config	&Config::operator=(Config const &copy) {
	(void)copy;
	return *this;
}

int	Config::check_file(std::ifstream &config_file)	{
	
	std::string line;
	if (!config_file.is_open()) {
		std::cerr << "The input file can't be open" << std::endl;
		return 0;
	}
	std::getline(config_file, line);
	if (line.find("server") == std::string::npos) {
		std::cerr << "error: file" << std::endl;
		return 0;
	}
	else
		add_server(config_file, line);
return 0;
}

std::vector<ServerConfig> Config::getServerConfigs() {
	return servers;
}

void Config::add_server(std::ifstream &config_file, std::string line)
{
	ServerConfig	conf;

	while (std::getline(config_file, line))
	{
		if (line.empty()) continue;
		conf.splitKeyValue(line, config_file);
		if (line.find("server") != std::string::npos)
			add_server(config_file, line);
	}
	servers.push_back(conf);
}