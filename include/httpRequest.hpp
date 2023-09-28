#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>

class HttpRequest {
public:
    HttpRequest(const std::string& request_str);

    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    std::string getMethod() const;
    std::string getPath() const;
    std::string getVersion() const;
    std::string getHeader(const std::string& name) const;
    std::map<std::string, std::string> getHeaders() const;
    std::map<std::string, std::string> getParameters() const;

private:
    void parseFirstLine(const std::string& first_line);
    void parseParameters(const std::string& body);

    std::string method_;
    std::string path_;
    std::string version_;
    std::map<std::string, std::string> headers_;
    std::map<std::string, std::string> parameters_;
};

#endif // HTTPREQUEST_HPP
