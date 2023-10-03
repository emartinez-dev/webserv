#include "../include/httpRequest.hpp"
#include <iostream>
#include <sstream>

// Constructor de la clase HttpRequest
HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const std::string& request_str) {
    size_t start = 0;
    size_t end = request_str.find("\r\n");

    // Parsea la primera línea de la solicitud HTTP
    if (end != std::string::npos) {
        std::string first_line = request_str.substr(start, end - start);
        parseFirstLine(first_line);
        parseHeaders(request_str);
    }

    // Encuentra el inicio del cuerpo de la solicitud
    start = request_str.find("\r\n\r\n");
    if (start != std::string::npos) {
        start += 4;  // Mueve el puntero al inicio del cuerpo
        std::string body = request_str.substr(start);
        parseParameters(body);
    }
}

void HttpRequest::parseHeaders(const std::string& _headers) {
     std::istringstream ss(_headers);
    std::string header;
    while (std::getline(ss, header, '\n')) {
        size_t pos = header.find(':');
        if (pos != std::string::npos) {
            std::string key = header.substr(0, pos);
            std::string value = header.substr(pos + 2);
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
    method_ = other.method_;
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

    method_ = other.method_;
    path_ = other.path_;
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

// Función para obtener la versión HTTP
std::string HttpRequest::getVersion() const {
    return version_;
}

// Función para obtener un encabezado específico por su nombre
std::string HttpRequest::getHeader(const std::string& name) const {
    std::map<std::string, std::string>::const_iterator it = headers.find(name);
    //std::cout << "en getheader recibe -> " << name << std::endl;
    if (it != headers.end()) {
        return it->second;
    } else {
        return "";
    }
}

std::string HttpRequest::getHeaderKey(const std::string& name) const {
    std::map<std::string, std::string>::const_iterator it = headers.find(name);
    
    if (it != headers.end()) {
        return it->first;
    } else {
        return "";
    }
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
    while (std::getline(ss, param, '&')) {
        size_t pos = param.find('=');
        if (pos != std::string::npos) {
            std::string key = param.substr(0, pos);
            std::string value = param.substr(pos + 1);
            parameters[key] = value;
        }
    }
}

void  HttpRequest::printRequest(void)
{
	std::cout << "\n\n------------------Request--------------------\n";
	std::cout << "Método: " << getMethod() << std::endl;
    std::cout << "Ruta: " << getPath() << std::endl;
    std::cout << "Versión HTTP: " << getVersion() << std::endl;

    std::cout << "Encabezados:" << std::endl;
    std::map<std::string, std::string> headers = getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        std::cout << "  " << it->first << "->" << it->second << std::endl;
    }

    std::cout << "Parámetros:" << std::endl;
    std::map<std::string, std::string> parameters = getParameters();
    for (std::map<std::string, std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}
