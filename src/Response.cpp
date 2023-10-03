#include "Response.hpp"

Response::Response()
{
}

Response::Response(const HttpRequest &request, const Config &config)
{
	ServerConfig const *server_config = config.getServer(request.getHeader("Host"));
	Location const *location = server_config->getLocation(request.getPath());

	/* Server Response process diagram:
	 * 1. Get the server and location of the given request, we should add
	 *	  a PWD or something like this concatenating the 'server_root' 
	 *	  with the route 'root'
	 * 2. If the location is NULL or the file is not found, set status 
	 *	  code to 404.
	 * 3. Check if the method of a request is allowed, set status code
	 *	  to 405.
	 * 4. Check if the location has CGI, call Alberto function with
	 *	  the params of the request and return the string to the body
	 * METHOD GET
	 * 1. If the location has /return, return 301 with new location
	 * 2. If the location is a folder, return the index if given 
	 * 3. If no index and autoindex is on, generate html with listdir
	 * 4. If the location is found, open file and write text to the body
	 * METHOD POST
	 *	1. Check if the post is a form or a file upload
	 *	2. Check that the body is less than client_max_body_size
	 *	3. If it's a file and it's too big, return 413
	 *	4. If it's a file and it can be saved, return 201
	 *	5. If it's a file and the name is invalid, return 422
	 * METHOD DELETE
	 *	1. Check if the file exists, if not return 404
	 *	2. Delete the file with unlink and return 200
	 * GENERAL
	 * 1. After having status code, insert it to the response. If it's an
	 *	  error code and the config file has error page, add it to the	
	 *	  body of the request, else return default error page
	 * 2. Set response headers to HTTP/1.1 <STATUS> <MESSAGE>.
	 * 3. Add the body to the response, separated by '\r\n\r\n'
	 * 4. Check the body length and add it to header with Content-Lenght 
	 *	  = len
	 * 5. Check the body type and add it with Content-Type = text/html,
	 *	  json, etc
	 * */

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
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
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
