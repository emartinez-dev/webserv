#ifndef CGI_HPP
#define CGI_HPP

# include "httpRequest.hpp"

std::string runCGI(const std::string& cgi_path, const std::string& cgi_file, char **envp, const HttpRequest &request);

#endif
