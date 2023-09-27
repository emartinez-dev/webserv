#include "config.hpp"

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
	server_conf	abc;
	while (std::getline(config_file, line)) {
		std::cout << line << std::endl;
		if (line.find("server") != std::string::npos) {
			std::cout << "another server_conf conf" << std::endl;
		}
		abc.splitKeyValue(line);

	}
	config_file.close();
	return 1;
}
