#ifndef REQUEST_HPP
#define REQUEST_HPP

# include "webserv.hpp"

class Request {
    private:
		bool		_receivedHeaders;
        std::string method;
        std::string path;
        std::string version;
        std::string body;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> parameters;

        void parseFirstLine(const std::string& firstFile);
        void parseHeaders(const std::string& line);
        void parseParameters(const std::string& body);
		std::string parseURL(const std::string& path);

    public:
		Request();
        Request(const std::vector<char>& request_buffer);
        Request(const Request& other);
        Request& operator=(const Request& other);
        ~Request();

        const std::string &getMethod() const;
        const std::string &getPath() const;
        const std::string &getVersion() const;
        const std::string &getBody() const;
        const std::string &getHeader(const std::string& name) const;
        const std::string &getHeaderKey(const std::string& name) const;
        const std::map<std::string, std::string> &getHeaders() const;
        const std::map<std::string, std::string> &getParameters() const;

		bool  receivedHeaders() const;
		bool  receivedBody() const;
        void  printRequest(void) const;
};

#endif
