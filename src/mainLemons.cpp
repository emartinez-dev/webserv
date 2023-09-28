#include "../include/httpRequest.hpp"
#include <iostream>

int main() {
std::string request_str = "POST /submit_form HTTP/1.1\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n"
                          "Host: www.example.com\r\n"
                          "Content-Type: application/x-www-form-urlencoded\r\n"
                          "Content-Length: 35\r\n"
                          "Accept-Language: en-US,en;q=0.8\r\n"
                          "Custom-Header: MyCustomValue\r\n"
                          "Connection: close\r\n\r\n"
                          "param1=value1&param2=value2&param3=value3";


    HttpRequest httpRequest(request_str);

    std::cout << "Método: " << httpRequest.getMethod() << std::endl;
    std::cout << "Ruta: " << httpRequest.getPath() << std::endl;
    std::cout << "Versión HTTP: " << httpRequest.getVersion() << std::endl;

    std::cout << "Encabezados:" << std::endl;
    std::map<std::string, std::string> headers = httpRequest.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    std::cout << "Parámetros:" << std::endl;
    std::map<std::string, std::string> parameters = httpRequest.getParameters();
    for (std::map<std::string, std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return 0;
}
