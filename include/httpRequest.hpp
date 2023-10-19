#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include "Utils.hpp"

class HttpRequest {
    private:
		bool		received_headers;
        std::string method_;
        std::string path_;
        std::string version_;
        std::string body;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> parameters;

        void parseFirstLine(const std::string& first_line);
        void parseHeaders(const std::string& line);
        void parseParameters(const std::string& body);
		std::string parseURL(const std::string &path);

    public:
		HttpRequest();
        HttpRequest(const std::vector<char>& request_buffer);
        HttpRequest(const HttpRequest& other);
        HttpRequest& operator=(const HttpRequest& other);
        ~HttpRequest();

		bool  receivedHeaders() const;
		bool  receivedBody() const;
        std::string getMethod() const;
        std::string getPath() const;
        std::string getVersion() const;
        const std::string &getBody() const;
        std::string getHeader(const std::string& name) const;
        std::string getHeaderKey(const std::string& name) const;
        std::map<std::string, std::string> getHeaders() const;
        std::map<std::string, std::string> getParameters() const;
        void  printRequest(void) const;
};

#endif // HTTPREQUEST_HPP
