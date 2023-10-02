#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include <iostream>
# include <string>
# include <unordered_map>
# include "Config.hpp"
# include "httpRequest.hpp"

class Response
{
	private:
		std::string status_code;
		std::map<std::string, std::string> headers;
		std::string	  body;
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
};

#endif
