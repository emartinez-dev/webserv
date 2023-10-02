#include "Response.hpp"

Response::Response()
{
}

Response::Response(const HttpRequest &request, const Config &config)
{
	ServerConfig const *server = config.getServer(request.getHeader("Host"));
	if (server == NULL)	
		std::cout << "Server not found\n";
	else
		std::cout << "Server found: " << server->getValue("name") << std::endl;
}

Response::~Response()
{
}

Response::Response(Response const &copy):status_code(copy.status_code),
	headers(copy.headers), body(copy.body)
{
}

Response	&Response::operator=(const Response &copy)
{
	if (this != &copy) {
		status_code = copy.status_code;
		headers = copy.headers;
		body = copy.body;
	}
	return *this;
}
