#include "Request.hpp"
#include "Utils.hpp"

// Constructor de la clase HttpRequest
Request::Request() {
}

Request::Request(const std::vector<char>& request_buffer)
{
	std::string request_str = std::string(request_buffer.begin(), request_buffer.end());
	size_t pos = request_str.find("\r\n\r\n");
    if (request_str.find("\r\n") != std::string::npos) {
        std::string first_line = request_str.substr(0, request_str.find("\r\n"));
		std::string headers = request_str.substr(0, pos);
        parseFirstLine(first_line);
        parseHeaders(headers);
		std::string url_parameters = parseURL(path);
		parseParameters(url_parameters);
    }

    // Encuentra el inicio del cuerpo de la solicitud
    if (request_str.find("\r\n\r\n") != std::string::npos) {
		_receivedHeaders = true;
        body = request_str.substr(pos + 4);
        parseParameters(body);
    }
}

void Request::parseHeaders(const std::string& _headers) {
    std::istringstream ss(_headers);
    std::string header;
    while (std::getline(ss, header)) {
        size_t pos = header.find_first_of(": ");
        if (pos != std::string::npos) {
            std::string key = splitKey(header);
            std::string value = splitValue(header);
            headers[key] = value;
        }
    }
}

// Función privada para analizar la primera línea de la solicitud HTTP
void Request::parseFirstLine(const std::string& first_line) {
    size_t pos1 = first_line.find(' ');
    size_t pos2 = first_line.find(' ', pos1 + 1);
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        method = first_line.substr(0, pos1);
        path = first_line.substr(pos1 + 1, pos2 - pos1 - 1);
        version = first_line.substr(pos2 + 1);
    }
}


std::string Request::parseURL(const std::string &_path)
{
	std::string	  parameters;
	size_t	pos = _path.find("?");

	if (pos == std::string::npos)
		return "";
	parameters = _path.substr(pos + 1);
	path = _path.substr(0, pos);
	return parameters;
}

// Constructor de copia de la clase HttpRequest
Request::Request(const Request& other) {
	_receivedHeaders = other._receivedHeaders;
    method = other.method;
	body = other.body;
    path = other.path;
    version = other.version;
    headers = other.headers;
    parameters = other.parameters;
}

// Operador de asignación de la clase HttpRequest
Request& Request::operator=(const Request& other) {
    if (this != &other) {
		_receivedHeaders = other._receivedHeaders;
		method = other.method;
		path = other.path;
		body = other.body;
		version = other.version;
		headers = other.headers;
		parameters = other.parameters;
    }
    return *this;
}

Request::~Request() {
}

const std::string &Request::getMethod() const {
    return method;
}

// Función para obtener la ruta
const std::string &Request::getPath() const {
    return path;
}

const std::string &Request::getBody() const {
    return body;
}

// Función para obtener la versión HTTP
const std::string &Request::getVersion() const {
    return version;
}

// Función para obtener un encabezado específico por su nombre
const std::string &Request::getHeader(const std::string& name) const {
    return (getMapValue(name, this->headers));
}

const std::string &Request::getHeaderKey(const std::string& name) const {
    return (getMapKey(name, this->headers));
}

// Función para obtener todos los encabezados
const std::map<std::string, std::string> &Request::getHeaders() const {
    return headers;
}

// Función para obtener todos los parámetros
const std::map<std::string, std::string> &Request::getParameters() const {
    return parameters;
}

// Función privada para analizar los parámetros del cuerpo de la solicitud HTTP
void Request::parseParameters(const std::string& body) {
    std::istringstream ss(body);
    std::string param;
	if (getHeader("Content-Type").find("multipart/form-data") != std::string::npos)
		return ;
    while (std::getline(ss, param, '&')) {
        size_t pos = param.find('=');
        if (pos != std::string::npos) {
            std::string key = param.substr(0, pos);
            std::string value = param.substr(pos + 1);
            parameters[key] = value;
        }
    }
}

void  Request::printRequest(void) const
{
	std::cout << "\n\n------------------start Request--------------------\n";
	std::cout << "Método: " << getMethod() << std::endl;
    std::cout << "Ruta: " << getPath() << std::endl;
    std::cout << "Versión HTTP: " << getVersion() << std::endl;

    std::cout << "Encabezados:" << std::endl;
    std::map<std::string, std::string> headers = getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        std::cout << "  " << it->first << "->" << it->second << std::endl;
    }

    std::cout << "Body length: " << body.length() << std::endl;
	if (body.length() < 100)
		std::cout << "Body______________________\n" << body << "________________endbody"<< std::endl;
	else
		std::cout << "Body______________________\n" << "[ too large ]" << "________________endbody"<< std::endl;

    std::cout << "Parámetros:" << std::endl;
    std::map<std::string, std::string> parameters = getParameters();
    for (std::map<std::string, std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "\n\n------------------end Request--------------------\n";
    }
}

bool  Request::receivedHeaders(void) const
{
	return _receivedHeaders;
}

bool  Request::receivedBody(void) const
{
	unsigned int  content_length = 0;

	if (getHeader("Content-Length") != "" || getHeader("Expect") == "100-continue")
		content_length = atol(getHeader("Content-Length").c_str());
	else
		return (true);
	if (getBody().length() == content_length)
		return (true);
	return (false);
}
