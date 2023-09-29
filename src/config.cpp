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
	ServerConfig	conf;
	while (std::getline(config_file, line)) {
		//std::cout << line << std::endl;
		if (line.empty()) continue;
		conf.splitKeyValue(line, config_file);
		//std::cout << "line fuera -> " << line << std::endl;
		if (line.find("server") != std::string::npos) {
			ServerConfig conf2;
			memcpy(&conf2, &conf, sizeof(conf));
			servers.push_back(conf2);
			bzero(&conf, sizeof(conf));
		}
	}
	// Una funcion que cuente servers en el texto y con un while cree tantos serverConfig como haya
	// luego meter todo el while anterior en una funcion en la que le envio un objeto de tipo serverConfig
	// dentro de esa funcion en casa de encontrar un server se sale fuera

	// conf.printConfig();
	config_file.close();
	return 1;
	/*
	std::string line;
	if (!config_file.is_open())
	{
		std::cerr << "The input file can't be open" << std::endl;
		return 0;
	}
	std::getline(config_file, line);
	if (line.find("server") == std::string::npos) {
		std::cerr << "error: file" << std::endl;
		return 0;
	}
	ServerConfig conf;
	while (std::getline(config_file, line))
	{
		
	}
	*/
}

std::vector<ServerConfig> Config::getServerConfigs() {
	return servers;
}
