#include "server_conf.hpp"

server_conf::server_conf() {

}

void	server_conf::splitKeyValue(std::string const &line) {
	size_t pointsPos = line.find(":");
	if(pointsPos != LAST) {
		std::string key = line.substr(0, pointsPos);
		std::string value = line.substr(pointsPos + 1);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
		conf[key] = value;
	}
}