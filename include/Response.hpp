#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Config.hpp"
# include "ServerConfig.hpp"
# include "Request.hpp"
# include "HttpPath.hpp"
# include "Utils.hpp"
# include "webserv.hpp"
# include "Utils.hpp"
# include "WebServerExceptions.hpp"

#define HTTP_STATUS_OK 200                    // 200 OK
#define HTTP_STATUS_CREATED 201               // 201 Created
#define HTTP_STATUS_NO_CONTENT 204            // 204 No Content
#define HTTP_STATUS_MOVED_PERMANENTLY 301     // 301 Moved Permanently
#define HTTP_STATUS_BAD_REQUEST 400          // 400 Bad Request
#define HTTP_STATUS_UNAUTHORIZED 401         // 401 Unauthorized
#define HTTP_STATUS_FORBIDDEN 403            // 403 Forbidden
#define HTTP_STATUS_NOT_FOUND 404            // 404 Not Found
#define HTTP_STATUS_METHOD_NOT_ALLOWED 405   // 405 Method Not Allowed
#define HTTP_STATUS_REQUEST_TIMEOUT 408      // 408 Request Timeout
#define HTTP_STATUS_PAYLOAD_TOO_LARGE 413   // 413 Payload Too Large
#define HTTP_STATUS_URI_TOO_LONG 414         // 414 URI Too Long
#define HTTP_STATUS_INTERNAL_SERVER_ERROR 500  // 500 Internal Server Error
#define HTTP_STATUS_NOT_IMPLEMENTED 501        // 501 Not Implemented


class Response
{
	private:
		std::string		version;
		int			 	statusCode;
		std::string	  	body;
		size_t			bodyLength;
		std::string 	filePath;
		std::map<std::string, std::string> headers;

		std::string 		getFirstLine() const;
		std::string 		getContentType(const std::string& fileExtension);
		void  				setFilePath(const std::string &rootMain, const std::string &addRoot);
		void  				setContentLength(std::string& key);
		std::string 		getStatusMessage() const;
		int					getRequestMethod(const std::string &methodStr);
		bool 				getSize();

		void  				setStatusCode(const int _statusCode);
		void  				setHeader(const std::string &key, const std::string &value);
		void  				setBody(const std::string &body);

		void 				printResponse()const;

		bool 				readFileAndsetBody(const std::string &path);
		bool				isAllowedMethod(int allowedMethods, const std::string &requestMethod);
		void				index(const Location &location);
		void				autoindex();
		void 				bodyToBodyLength();

		void 				addBody(std::string const &addString);
		std::string			createHeadHtml(const std::string &title);
		std::string			createClousureHtml();
		void				createErrorPage();
		void				getErrorPage(const ServerConfig &server);

		bool				belowBodySizeLimit(const ServerConfig &server, const Request &request);
		std::string			redirectAddress(const std::string &url, const Location &location);

		void				redirectionHandler(const Request &request, const Location &location);
		void				getHandler(const Request &request, const Location &location, const ServerConfig &config);
		void				postHandler(const Request &request, const Location &location, const ServerConfig &config);
		void				deleteHandler(void);
		void				runCGI(const std::string& cgiPath, const std::string& cgiFile, const Request &request);
		void				uploadFile(const Location &location, const Request &request);


	public:
		Response();
		Response(const Request &request, const Config &config);
		Response(int errorCode);
		~Response();
		Response(Response const &copy);
		Response	&operator=(Response const &copy);

		const std::string 	getContent(void) const;
};

#endif
