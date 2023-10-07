#include "Response.hpp"

Response::Response()
{
}

	/* Server Response process diagram:
	 * 1. Get the server and location of the given request, we should add
	 *	  a route_relative or something like this concatenating the 'server_root' 
	 *	  with the route_relative 'root'
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


Response::Response(const HttpRequest &request, const Config &config)
{
	std::string content_length_key = "Content-Length";
	route_relative = request.getHeader("path");
	version = request.getVersion();
	ServerConfig const *server_config = config.getServer(request.getHeader("Host"));
	Location const *location = server_config->getLocation(request.getPath());
	HttpPath httpPath(request.getPath(), location);
	errorroute_relative(request, location, server_config);
	setRootFinish(server_config->getValue("root"), httpPath.getRoot());
	bool can_read = readFileAndsetBody();
	//request.printRequest();
	if (location == NULL || !can_read)
	{
			std::cout << "entra en 1: " << std::endl;

		setStatusCode("404");
		setHeader("Content-Type", getContentType(httpPath.getExtension()));
		setBody("<html><body><h1>Page not found</h1></body></html>");
		setHeader("Content-Length", itoa(getSize()));
	}
	else
	{
			std::cout << "entra en 2: " << std::endl;
		//TODO Hay que detectar la extension del archivo
		getSize();
		setStatusCode("200");
		setHeader("Content-Length", itoa(body_len));
		setHeader("Content-Type", getContentType(httpPath.getExtension()));
	}

}

Response::~Response()
{
}

Response::Response(Response const &copy):status_code(copy.status_code),
	body(copy.body), route_relative(copy.route_relative), headers(copy.headers)
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

bool Response::errorroute_relative(const HttpRequest &request, const Location* location, const ServerConfig* server_config) {
	int method_request = getResponseMethods(request.getMethod());
	// Aqui comprobamos que el metodo sea correcto, Hay que ver que hacer a 
	// partir de aqui.
	if (isAllowedMethod(location->getAllowMethods(), method_request) == "200")
		std::cout << "El metodo esta permitido" << std::endl;
	else
		std::cout << "El metodo No esta permitido" << std::endl;
	
	// ¿La ruta es Accesible?
	if (isAccessible(server_config->getValue("root"))) {
		std::cout << "Se puede acceder" << std::endl;
		setroute_relative(request.getPath());
		setFullroute_relative(request.getPath(), server_config->getValue("root"));
	} else {
		std::cout << "NO se puede acceder" << std::endl;
	}

	// ¿Es un archivo?
	if (isFile())
		std::cout << "es un archivo" << std::endl;
	else
		std::cout << "NO es un archivo" << std::endl;
	return true;	
}

/*GETTERS*/

const std::string& Response::getroute_relative() const {
	return (this->route_relative);
}

const std::string& Response::getFullroute_relative() const {
	return (this->fullroute_relative);
}

std::string Response::getFirstLine () const {
	return (version + " " + status_code + " " + getStatusMessage() + "\r\n");
}


std::string Response::getStatusMessage() const {
    int status = atoi(status_code.c_str());

    switch (status) {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 204:
            return "No Content";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        default:
            return "Unknown Status";
    }
}

bool Response::readFileAndsetBody() {
	std::cout << "root_finish que llega a read -> " << root_finish << std::endl;
	std::ifstream file(root_finish);
	if (file.is_open()) {
		char character;
		while (file.get(character)) {
			body += character;
		}
		file.close();
	} else {
		return false;
	}
	return true;
}

bool Response::getSize() {
	std::ifstream file(getRootFinish(), std::ios::binary);
	if (!file.is_open()) {
		return false;
	}
	file.seekg(0, std::ios::end);
	body_len = file.tellg();
	file.close();
	std::cout << "body_len: " << body_len << std::endl;
	return true;
}

/*SETTERS*/
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

void Response::setRootFinish(std::string root_main, std::string add_root) {
	root_finish = (root_main + add_root);
}

void Response::setFullroute_relative(const std::string& request_route_relative, const std::string& root) {
	fullroute_relative = (root + request_route_relative);
}

void Response::setroute_relative(const std::string& root_cnf) {
	route_relative = root_cnf;
}

void Response::setContentLength(std::string& key) {
	std::cout << "-------------------------" << getMapValue(key, headers) << std::endl;
	std::cout << "key: " << key << std::endl;
}


void Response::printResponse() {
	std::cout << "version: " << version << std::endl;
	std::cout << "status_code: " << status_code << std::endl;
	std::cout << "body: " << body << std::endl;
	std::cout << "route_relative: " << route_relative << std::endl;
}

std::string Response::getRootFinish() const{
	return root_finish;
}

/**
 * @brief Determines the content type of the response.
 * 
 * @param fileExtension receives the file extension of the path.
 * @return std::string string with the assigned content type.
 */
std::string Response::getContentType(const std::string& fileExtension) {
    std::string contentType = "unknown-type"; // Valor predeterminado

    if (!fileExtension.empty()) {
        std::string ext = fileExtension.substr(1); // Elimina el punto inicial de la extensión

        switch (ext[0]) {
            case 't':
                if (ext == "txt") contentType = "text/plain";
                break;
            case 'h':
                if (ext == "html") contentType = "text/html";
                break;
            case 'j':
                if (ext == "json") contentType = "application/json";
                else if (ext == "jpg" || ext == "jpeg") contentType = "image/jpeg";
                break;
            case 'x':
                if (ext == "xml") contentType = "application/xml";
                break;
            case 'p':
                if (ext == "pdf") contentType = "application/pdf";
                else if (ext == "png") contentType = "image/png";
                break;
            case 'g':
                if (ext == "gif") contentType = "image/gif";
                break;
            case 'c':
                if (ext == "css") contentType = "text/css";
                break;
            default:
                break;
        }
    }
	std::cout << "contentType: " << contentType << std::endl;
    return contentType;
}

const std::string Response::getContent(void) const
{
	std::string headers_text;
	std::string response_text;

	response_text += getFirstLine();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		headers_text += it->first + ": " + it->second + "\r\n";
	response_text += headers_text;
	response_text += "\r\n";
	response_text += body;
	return (response_text);
}

/**
 * @brief Assigns a number to the request method.
 * 
 * @param met_req Receives the request method of type string.
 * @return int with the number assigned after checking it.
 */
int	Response::getResponseMethods(std::string met_req) const{
	int methods = 0;
	if (met_req.find("GET") != LAST)
		methods = 1;
	else if (met_req.find("POST") != LAST)
		methods = 3;
	else if (met_req.find("DELETE") != LAST)
		methods = 5;
	return methods;
}

/**
 * @brief This function determines if the method is allowed
 * 
 * @param method_conf receives an integer that determines the methods allowed by the configuration file.
 * @param met_req Receive the method that the request requires.
 * @return std::string Returns the status code after checking the allowed methods.
 */
std::string Response::isAllowedMethod(int method_conf, int method_request) const{
	std::string status = "200";
	switch (method_request) {
		case 1:
			if (method_conf == 1 || method_conf == 4 || method_conf == 6 ||method_conf == 9)
				break;
			status = "405";
		case 3:
			if (method_conf == 3 || method_conf == 4 || method_conf == 8 ||method_conf == 9)
				break;
			status = "405";
		case 5:
			if (method_conf == 5 || method_conf == 6 || method_conf == 8 ||method_conf == 9)
				break;
			status = "405";
			break;
		default:
			status = "405";
    }
	return status;
}

/**
 * @brief determines whether a route_relative is accessible or not.
 * 
 * @param request_route_relative request_route_relative
 * @param root_cnf 
 * @return true 
 * @return false 
 */
bool Response::isAccessible(const std::string& root_cnf) const{
	if (access((route_relative + root_cnf).c_str(), F_OK) == 0) {
		return true;
	}
	return false;
}

/**
 * @brief determines whether a path reaches a file or a directory.
 * 
 * @return true if it is a file.
 * @return false if it is a directory.
 */
bool Response::isFile() const{
	size_t pos = route_relative.rfind('.');

	if (pos != std::string::npos) {
		if (pos < route_relative.length() - 1) {
			return true;
		}
    }
	return false;
}