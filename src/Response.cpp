#include "Response.hpp"

Response::Response()
{
}

Response::Response(const HttpRequest &request, const Config &config)
{
	ServerConfig const *server_config = config.getServer(request.getHeader("Host"));
	Location const *location = server_config->getLocation(request.getPath());
	if (location == NULL)
	{
		setStatusCode("404");
		setHeader("Content-Type", "text/html");
		setBody("<html><body><h1>Page not found</h1></body></html>");
	}
	else
	{
		setStatusCode("200");
		setHeader("Content-Type", "text/html");
		setBody("<html><body><h1>" + location->getValue("route") + location->getValue("index") + "</h1></body></html>");
	}
}

const std::string Response::getContent(void) const
{
	std::string response_text = "";

	response_text += "HTTP/1.1 ";
	response_text += status_code + " OK\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
		response_text += it->first + ": " + it->second + "\r\n";
	response_text += "\r\n\r\n";
	response_text += body;
	return (response_text);
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

void  Response::setStatusCode(const std::string &status_code)
{
	this->status_code = status_code;
}

void  Response::setHeader(const std::string &key, const std::string &value)
{
	headers[key] = value;
}

void  Response::setBody(const std::string &body)
{
	this->body = body;
}
