#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <unordered_map>
# include <cstdlib>
# include "Config.hpp"
# include "httpRequest.hpp"
# include "HttpPath.hpp"
# include "Utils.hpp"

class Response
{
	private:
		std::string		version;
		std::string 	status_code;
		std::string	  	body;
		std::streampos	body_len;
		std::string 	route_relative;
		std::string 	fullroute_relative;
		std::map<std::string, std::string> headers;
		Response();
	public:
		size_t			size_body;
		Response(const HttpRequest &request, const Config &config);
		~Response();
		Response(Response const &copy);
		Response	&operator=(Response const &copy);

		void  				setStatusCode(const std::string &status_code);
		void  				setHeader(const std::string &key, const std::string &value);
		void  				setBody(const std::string &body);
		void  				setContentLength(std::string& key);
		void				setFullroute_relative(const std::string& request_route_relative, const std::string& root_cnf);
		void				setroute_relative(const std::string& root_cnf);

		const std::string 	getContent(void) const;
		bool 				readFileAndsetBody();
		const std::string& 	getroute_relative() const;
		const std::string&	getFullroute_relative() const;
		std::string 		getFirstLine() const;
		std::string 		getStatusMessage() const;
		bool 				getSize();
		std::string 		getContentType(const std::string& fileExtension);

		void 				printResponse();

		std::string 		isAllowedMethod(int method, int met_req) const;
		int					getResponseMethods(std::string met_req) const;
		bool 				isFile() const;
		bool 				isAccessible(const std::string& root_cnf) const;
		bool 				errorroute_relative(const HttpRequest &request, const Location* location, const ServerConfig* server_config) ;
};

#endif
