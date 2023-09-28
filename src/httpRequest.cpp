#include "../include/httpRequest.hpp"
#include <sstream>

HttpRequest::HttpRequest(const std::string& request_str) {
    size_t start = 0;
    size_t end = request_str.find("\r\n");

    if (end != std::string::npos) {
        std::string first_line = request_str.substr(start, end - start);
        parseFirstLine(first_line);
    }

    start = request_str.find("\r\n\r\n");
    if (start != std::string::npos) {
        start += 4;
        std::string body = request_str.substr(start);
        parseParameters(body);
    }
}

void HttpRequest::parseFirstLine(const std::string& first_line) {
    size_t pos1 = first_line.find(' ');
    size_t pos2 = first_line.find(' ', pos1 + 1);
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        method_ = first_line.substr(0, pos1);
        path_ = first_line.substr(pos1 + 1, pos2 - pos1 - 1);
        version_ = first_line.substr(pos2 + 1);
    }
}

HttpRequest::HttpRequest(const HttpRequest& other) {
    method_ = other.method_;
    path_ = other.path_;
    version_ = other.version_;
    headers_ = other.headers_;
    parameters_ = other.parameters_;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    if (this == &other) {
        return *this;
    }

    method_ = other.method_;
    path_ = other.path_;
    version_ = other.version_;
    headers_ = other.headers_;
    parameters_ = other.parameters_;

    return *this;
}

HttpRequest::~HttpRequest() {
    // No se requiere una operación de limpieza especial
}

std::string HttpRequest::getMethod() const {
    return method_;
}

std::string HttpRequest::getPath() const {
    return path_;
}

std::string HttpRequest::getVersion() const {
    return version_;
}

std::string HttpRequest::getHeader(const std::string& name) const {
    std::map<std::string, std::string>::const_iterator it = headers_.find(name);
    return (it != headers_.end()) ? it->second : "";
}

std::map<std::string, std::string> HttpRequest::getHeaders() const {
    return headers_;
}

std::map<std::string, std::string> HttpRequest::getParameters() const {
    return parameters_;
}

void HttpRequest::parseParameters(const std::string& body) {
    std::istringstream ss(body);
    std::string param;
    while (std::getline(ss, param, '&')) {
        size_t pos = param.find('=');
        if (pos != std::string::npos) {
            std::string key = param.substr(0, pos);
            std::string value = param.substr(pos + 1);
            parameters_[key] = value;
        }
    }
}
