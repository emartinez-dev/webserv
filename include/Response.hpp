#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <unordered_map>
# include <cstdlib>
# include "Config.hpp"
# include "httpRequest.hpp"
# include "Utils.hpp"

class Response
{
	private:
		std::string		version;
		std::string 	status_code;
		std::string	  	body;
		std::string 	pwd;
		std::map<std::string, std::string> headers;
		Response();
	public:
		Response(const HttpRequest &request, const Config &config);
		~Response();
		Response(Response const &copy);
		Response	&operator=(Response const &copy);

		void  setStatusCode(const std::string &status_code);
		void  setHeader(const std::string &key, const std::string &value);
		void  setBody(const std::string &body);

		const std::string getContent(void) const;
		bool readFile();
		const std::string& getPwd() const;
		void getResponse(std::string newpwd);
		std::string getFirstLine() const;
		std::string getStatusMessage() const;
};

#endif
