#include "httpRequest.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

// Constructor de la clase HttpRequest
HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const std::vector<char>& request_buffer)
{
	std::string request_str = std::string(request_buffer.begin(), request_buffer.end());
	size_t pos = request_str.find("\r\n\r\n");
    if (request_str.find("\r\n") != std::string::npos) {
        std::string first_line = request_str.substr(0, request_str.find("\r\n"));
		std::string headers = request_str.substr(0, pos);
        parseFirstLine(first_line);
        parseHeaders(headers);
    }

    // Encuentra el inicio del cuerpo de la solicitud
    if (request_str.find("\r\n\r\n") != std::string::npos) {
		received_headers = true;
        body = request_str.substr(pos + 4);
        parseParameters(body);
    }
}

void HttpRequest::parseHeaders(const std::string& _headers) {
    std::istringstream ss(_headers);
    std::string header;
    while (std::getline(ss, header)) {
        size_t pos = header.find(':');
        if (pos != std::string::npos) {
            std::string key = splitKey(header.substr(0, pos));
            std::string value = splitValue(header.substr(pos + 2));
            headers[key] = value;
        }
    }
}

// Función privada para analizar la primera línea de la solicitud HTTP
void HttpRequest::parseFirstLine(const std::string& first_line) {
    size_t pos1 = first_line.find(' ');
    size_t pos2 = first_line.find(' ', pos1 + 1);
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        method_ = first_line.substr(0, pos1);
        path_ = first_line.substr(pos1 + 1, pos2 - pos1 - 1);
        version_ = first_line.substr(pos2 + 1);
    }
}

// Constructor de copia de la clase HttpRequest
HttpRequest::HttpRequest(const HttpRequest& other) {
	received_headers = other.received_headers;
    method_ = other.method_;
	body = other.body;
    path_ = other.path_;
    version_ = other.version_;
    headers = other.headers;
    parameters = other.parameters;
}

// Operador de asignación de la clase HttpRequest
HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    if (this == &other) {
        return *this;
    }
	received_headers = other.received_headers;
    method_ = other.method_;
    path_ = other.path_;
	body = other.body;
    version_ = other.version_;
    headers = other.headers;
    parameters = other.parameters;

    return *this;
}

// Destructor de la clase HttpRequest
HttpRequest::~HttpRequest() {
    // No se requiere ninguna operación especial de limpieza
}

// Función para obtener el método HTTP
std::string HttpRequest::getMethod() const {
    return method_;
}

// Función para obtener la ruta
std::string HttpRequest::getPath() const {
    return path_;
}

const std::string &HttpRequest::getBody() const {
    return body;
}

// Función para obtener la versión HTTP
std::string HttpRequest::getVersion() const {
    return version_;
}

// Función para obtener un encabezado específico por su nombre
std::string HttpRequest::getHeader(const std::string& name) const {
    return (getMapValue(name, this->headers));
}

std::string HttpRequest::getHeaderKey(const std::string& name) const {
    return (getMapKey(name, this->headers));
}

// Función para obtener todos los encabezados
std::map<std::string, std::string> HttpRequest::getHeaders() const {
    return headers;
}

// Función para obtener todos los parámetros
std::map<std::string, std::string> HttpRequest::getParameters() const {
    return parameters;
}

// Función privada para analizar los parámetros del cuerpo de la solicitud HTTP
void HttpRequest::parseParameters(const std::string& body) {
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

void  HttpRequest::printRequest(void) const
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
    std::cout << "Body______________________\n" << body << "________________endbody"<< std::endl;

    std::cout << "Parámetros:" << std::endl;
    std::map<std::string, std::string> parameters = getParameters();
    for (std::map<std::string, std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "\n\n------------------end Request--------------------\n";
    }
}

bool  HttpRequest::receivedHeaders(void) const
{
	return received_headers;
}

bool  HttpRequest::receivedBody(void) const
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
