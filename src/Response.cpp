#include "Response.hpp"

Response::Response()
{
}

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

Response::Response(const HttpRequest &request, const Config &config)
{
	std::string content_length_key = "Content-Length";
	pwd = request.getHeader("path");
	version = request.getVersion();
	ServerConfig const *server_config = config.getServer(request.getHeader("Host"));
	Location const *location = server_config->getLocation(request.getPath());
	getResponse(request.getPath());
	bool can_read = readFile();
	
	getSize();
	if (location == NULL || !can_read)
	{
		setStatusCode("404");
		setHeader("Content-Type", "text/html");
		setHeader("Content-Length", itoa(getSize()));
		setBody("<html><body><h1>Page not found</h1></body></html>");
	}
	else
	{
		//TODO Hay que detectar la extension del archivo
		setStatusCode("200");
		setHeader("Content-Length", itoa(body_len));
		//setHeader("Content-Type", "image/jpeg");
	}
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
	//std::cout << body << std::endl;
	return (response_text);
}

Response::~Response()
{
}

Response::Response(Response const &copy):status_code(copy.status_code),
	body(copy.body), pwd(copy.pwd), headers(copy.headers)
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

/*GETTERS*/

const std::string& Response::getPwd() const {
	return (this->pwd);
}

void Response::getResponse(std::string newpwd) {
	pwd = "./example" + pwd + newpwd;
	
	if (access(pwd.c_str(), F_OK) == 0) {
		std::cout << "Se puede acceder" << std::endl;
	}
	if (access(pwd.c_str(), F_OK) == -1) {
		std::cout << "No se puede acceder" << std::endl;
	}
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

bool Response::readFile() {
	std::ifstream file(getPwd());
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
	std::ifstream file(getPwd(), std::ios::binary);
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

void Response::setContentLength(std::string& key) {
	std::cout << "-------------------------" << getMapValue(key, headers) << std::endl;
	std::cout << "key: " << key << std::endl;
}


void Response::printResponse() {
	std::cout << "version: " << version << std::endl;
	std::cout << "status_code: " << status_code << std::endl;
	std::cout << "body: " << body << std::endl;
	std::cout << "pwd: " << pwd << std::endl;
}

// std::string Response::getExtension() {
// Funcion Para obtener la extension////
// }

std::string Response::getContentType(const std::string& fileExtension) {
    std::string contentType = "application/octet-stream"; // Valor predeterminado

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
				contentType = "unknown-type";
                break;
        }
    }
    return contentType;
}