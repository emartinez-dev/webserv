#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <unordered_map>
# include <cstdlib>
# include "Config.hpp"
# include "httpRequest.hpp"
# include "HttpPath.hpp"
# include "Utils.hpp"

// Informacionales (1xx)
#define HTTP_STATUS_CONTINUE 100              // 100 Continue
#define HTTP_STATUS_SWITCHING_PROTOCOLS 101    // 101 Switching Protocols
#define HTTP_STATUS_PROCESSING 102             // 102 Processing

// Éxito (2xx)
#define HTTP_STATUS_OK 200                    // 200 OK
#define HTTP_STATUS_CREATED 201               // 201 Created
#define HTTP_STATUS_ACCEPTED 202              // 202 Accepted
#define HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION 203 // 203 Non-Authoritative Information
#define HTTP_STATUS_NO_CONTENT 204            // 204 No Content
#define HTTP_STATUS_RESET_CONTENT 205         // 205 Reset Content
#define HTTP_STATUS_PARTIAL_CONTENT 206       // 206 Partial Content
#define HTTP_STATUS_MULTI_STATUS 207          // 207 Multi-Status
#define HTTP_STATUS_ALREADY_REPORTED 208      // 208 Already Reported
#define HTTP_STATUS_IM_USED 226               // 226 IM Used

// Redirecciones (3xx)
#define HTTP_STATUS_MULTIPLE_CHOICES 300      // 300 Multiple Choices
#define HTTP_STATUS_MOVED_PERMANENTLY 301     // 301 Moved Permanently
#define HTTP_STATUS_FOUND 302                 // 302 Found
#define HTTP_STATUS_SEE_OTHER 303             // 303 See Other
#define HTTP_STATUS_NOT_MODIFIED 304          // 304 Not Modified
#define HTTP_STATUS_USE_PROXY 305             // 305 Use Proxy
#define HTTP_STATUS_TEMPORARY_REDIRECT 307    // 307 Temporary Redirect
#define HTTP_STATUS_PERMANENT_REDIRECT 308    // 308 Permanent Redirect

// Errores del cliente (4xx)
#define HTTP_STATUS_BAD_REQUEST 400          // 400 Bad Request
#define HTTP_STATUS_UNAUTHORIZED 401         // 401 Unauthorized
#define HTTP_STATUS_PAYMENT_REQUIRED 402     // 402 Payment Required
#define HTTP_STATUS_FORBIDDEN 403            // 403 Forbidden
#define HTTP_STATUS_NOT_FOUND 404            // 404 Not Found
#define HTTP_STATUS_METHOD_NOT_ALLOWED 405   // 405 Method Not Allowed
#define HTTP_STATUS_NOT_ACCEPTABLE 406       // 406 Not Acceptable
#define HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED 407 // 407 Proxy Authentication Required
#define HTTP_STATUS_REQUEST_TIMEOUT 408      // 408 Request Timeout
#define HTTP_STATUS_CONFLICT 409             // 409 Conflict
#define HTTP_STATUS_GONE 410                // 410 Gone
#define HTTP_STATUS_LENGTH_REQUIRED 411      // 411 Length Required
#define HTTP_STATUS_PRECONDITION_FAILED 412  // 412 Precondition Failed
#define HTTP_STATUS_PAYLOAD_TOO_LARGE 413   // 413 Payload Too Large
#define HTTP_STATUS_URI_TOO_LONG 414         // 414 URI Too Long
#define HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE 415 // 415 Unsupported Media Type
#define HTTP_STATUS_RANGE_NOT_SATISFIABLE 416  // 416 Range Not Satisfiable
#define HTTP_STATUS_EXPECTATION_FAILED 417    // 417 Expectation Failed
#define HTTP_STATUS_IM_A_TEAPOT 418          // 418 I'm a teapot
#define HTTP_STATUS_MISDIRECTED_REQUEST 421  // 421 Misdirected Request
#define HTTP_STATUS_UNPROCESSABLE_ENTITY 422 // 422 Unprocessable Entity
#define HTTP_STATUS_LOCKED 423               // 423 Locked
#define HTTP_STATUS_FAILED_DEPENDENCY 424    // 424 Failed Dependency
#define HTTP_STATUS_TOO_EARLY 425            // 425 Too Early
#define HTTP_STATUS_UPGRADE_REQUIRED 426     // 426 Upgrade Required
#define HTTP_STATUS_PRECONDITION_REQUIRED 428 // 428 Precondition Required
#define HTTP_STATUS_TOO_MANY_REQUESTS 429    // 429 Too Many Requests
#define HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE 431 // 431 Request Header Fields Too Large
#define HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS 451 // 451 Unavailable For Legal Reasons

// Errores del servidor (5xx)
#define HTTP_STATUS_INTERNAL_SERVER_ERROR 500  // 500 Internal Server Error
#define HTTP_STATUS_NOT_IMPLEMENTED 501        // 501 Not Implemented
#define HTTP_STATUS_BAD_GATEWAY 502            // 502 Bad Gateway
#define HTTP_STATUS_SERVICE_UNAVAILABLE 503    // 503 Service Unavailable
#define HTTP_STATUS_GATEWAY_TIMEOUT 504        // 504 Gateway Timeout
#define HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED 505 // 505 HTTP Version Not Supported
#define HTTP_STATUS_VARIANT_ALSO_NEGOTIATES 506 // 506 Variant Also Negotiates
#define HTTP_STATUS_INSUFFICIENT_STORAGE 507    // 507 Insufficient Storage
#define HTTP_STATUS_LOOP_DETECTED 508           // 508 Loop Detected
#define HTTP_STATUS_NOT_EXTENDED 510           // 510 Not Extended
#define HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED 511 // 511 Network Authentication Required


class Response
{
	private:
		std::string		version;
		int			 	status_code;
		int			 	methods;
		std::string	  	body;
		std::streampos	body_len;
		std::string 	route_relative;
		std::string 	full_route_relative;
		std::string 	real_root;
		std::map<std::string, std::string> headers;
		Response();
	public:
		size_t			size_body;
		Response(const HttpRequest &request, const Config &config);
		~Response();
		Response(Response const &copy);
		Response	&operator=(Response const &copy);

		void  				setStatusCode(const int status_code);
		void  				setHeader(const std::string &key, const std::string &value);
		void  				setBody(const std::string &body);
		void  				setRootFinish(std::string root_main, std::string add_root);
		void  				setContentLength(std::string& key);
		void				setfull_route_relative(const std::string& request_route_relative, const std::string& root_cnf);
		void				setroute_relative(const std::string& root_cnf);

		const std::string 	getContent(void) const;
		bool 				readFileAndsetBody();
		const std::string& 	getroute_relative() const;
		const std::string&	getfull_route_relative() const;
		std::string 		getFirstLine() const;
		std::string 		getStatusMessage() const;
		bool 				getSize();
		std::string 		getContentType(const std::string& fileExtension);
        std::string 		getRealRoot() const;

		void 				printResponse()const;

		void				isAllowedMethod(int method, int met_req);
		void				setResponseMethods(std::string met_req);
		bool 				isFile() const;
		bool 				isAccessible(const std::string& root_cnf) const;
		bool 				errorroute_relative(const HttpRequest &request, const ServerConfig* server_config) ;
};

#endif
