#ifndef UTILS_HPP
#define UTILS_HPP

# include "Server.hpp"

unsigned long parse_ip(const std::string &ip_address);
sockaddr_in	  config_address(std::string const &ip_address, int port);
std::string splitKey(std::string const &line);
std::string splitValue(std::string const &line);
#endif
