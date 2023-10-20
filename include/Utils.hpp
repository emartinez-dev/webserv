#ifndef UTILS_HPP
#define UTILS_HPP

# include "Server.hpp"
# include <map>
# include <sstream>
# include <string>

unsigned long	  parse_ip(const std::string &ip_address);
sockaddr_in		  config_address(std::string const &ip_address, int port);

std::string		  splitKey(std::string const &line);
std::string		  splitValue(std::string const &line);
const std::string &getMapKey(const std::string& name, const std::map<std::string, std::string>& map);
const std::string &getMapValue(const std::string& name, const std::map<std::string, std::string>& map);

std::string getFileExtension(const std::string &file);

std::string		  itoa(std::streampos number);

std::string		  removeSubstring(const std::string& original, const std::string& toRemove);
void			  replaceFirstSubstring(std::string& str, const std::string& oldSubstr, const std::string& newSubstr);

bool			  isFile(const std::string &path);
bool			  isFolder(const std::string &path);
bool			  isAccessible(const std::string &file);

#endif
