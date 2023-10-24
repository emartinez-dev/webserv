#include "Response.hpp"
#include "ServerConfig.hpp"
#include "Utils.hpp"
#include "WebServerExceptions.hpp"
#include <cstdlib>
#include <xlocale/_stdlib.h>

Response::Response()
{
}

Response::Response(const HttpRequest &request, const Config &config):version(request.getVersion()), \
	status_code(HTTP_STATUS_OK)
{
	ServerConfig const *server_config = config.getServer(request.getHeader("Host"));
	Location const *server_location = server_config->getLocation(request.getPath());
	if (server_location)
	{
		HttpPath request_path(request.getPath(), server_location);
		setFilePath(server_config->getValue("root"), request_path.getRoot());
		if (belowBodySizeLimit(*server_config, request))
		{
			if (request_path.URLisValid())
			{
				if (isAllowedMethod(server_location->getAllowMethods(), request.getMethod()))
				{
					if (server_location->hasRedirect())
						redirectionHandler(request, *server_location);
					else if (request.getMethod() == "GET")
						getHandler(request, *server_location, *server_config);
					else if (request.getMethod() == "POST")
						postHandler(request, *server_location, *server_config);
					else if (request.getMethod() == "DELETE")
						deleteHandler(request, *server_location);
					else
						setStatusCode(HTTP_STATUS_NOT_IMPLEMENTED);
				}
				else
					setStatusCode(HTTP_STATUS_METHOD_NOT_ALLOWED);
			}
			else
				setStatusCode(HTTP_STATUS_BAD_REQUEST);
		}
		else
			setStatusCode(HTTP_STATUS_PAYLOAD_TOO_LARGE);
	}
	else
		setStatusCode(HTTP_STATUS_NOT_FOUND);
	if (status_code >= 400)
		createErrorPage();
	bodyToBodyLength();
	setHeader("Content-Length", itoa(body_len));
	setHeader("Connection", "close");
}

void Response::redirectionHandler(const HttpRequest &request, const Location &location)
{
	setStatusCode(HTTP_STATUS_MOVED_PERMANENTLY);
	setHeader("Location", redirectAddress(request.getPath(), location));
}

void Response::getHandler(const HttpRequest &request, const Location &location, const ServerConfig &config)
{
	if (location.getValue("cgi_ext") != "" && getFileExtension(file_path) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), file_path, request);
	else if (isFolder(file_path) && location.getValue("cgi_ext") != "" && getFileExtension(location.getValue("index")) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), file_path + location.getValue("index"), request);
	else if (isFolder(file_path))
		index(location);
	else if (isFile(file_path) && isAccessible(file_path))
		readFileAndsetBody(file_path);
	else
		setStatusCode(HTTP_STATUS_NOT_FOUND);
}


void Response::postHandler(const HttpRequest &request, const Location &location, const ServerConfig &config)
{
	if (location.getValue("cgi_ext") != "" && getFileExtension(file_path) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), file_path, request);
	else if (isFolder(file_path) && location.getValue("cgi_ext") != "" && getFileExtension(location.getValue("index")) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), file_path + location.getValue("index"), request);
}

void Response::deleteHandler(const HttpRequest &request, const Location &location)
{
	(void) request;
	(void) location;
}

Response::Response(int error_code):version("HTTP/1.1\r"), status_code(error_code)
{
	if (status_code >= 400)
		createErrorPage();
	bodyToBodyLength();
	setHeader("Content-Length", itoa(body_len));
	setHeader("Connection", "close");
}

Response::~Response()
{
}

Response::Response(Response const &copy):version(copy.version),status_code(copy.status_code),
	body(copy.body), body_len(copy.body_len),headers(copy.headers)
{
}

Response	&Response::operator=(const Response &copy)
{
	if (this != &copy) {
		version = copy.version;
		status_code = copy.status_code;
		body = copy.body;
		body_len = copy.body_len;
		headers = copy.headers;
	}
	return *this;
}

/*GETTERS*/

std::string Response::getFirstLine () const {
	return (version + " " + itoa(status_code) + " " + getStatusMessage() + "\r\n");
}

std::string Response::getStatusMessage() const {

    switch (status_code) {
        case HTTP_STATUS_OK:
            return "OK";
        case HTTP_STATUS_CREATED:
            return "Created";
        case HTTP_STATUS_NO_CONTENT:
            return "No Content";
        case HTTP_STATUS_BAD_REQUEST:
            return "Bad Request";
        case HTTP_STATUS_UNAUTHORIZED:
            return "Unauthorized";
        case HTTP_STATUS_FORBIDDEN:
            return "Forbidden";
        case HTTP_STATUS_NOT_FOUND:
            return "Not Found";
        case HTTP_STATUS_INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
		case HTTP_STATUS_MOVED_PERMANENTLY:
			return "Moved Permanently";
        case HTTP_STATUS_NOT_IMPLEMENTED:
            return "Not Implemented";
		case HTTP_STATUS_REQUEST_TIMEOUT:
            return "Request Timeout";
		case HTTP_STATUS_METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
		case HTTP_STATUS_PAYLOAD_TOO_LARGE:
            return "Payload Too Large";
        default:
            return "Unknown Status";
    }
}

/**
 * @brief Determines the content type of the response.
 *
 * @param fileExtension receives the file extension of the path.
 * @return std::string string with the assigned content type.
 */
std::string Response::getContentType(const std::string& file_path) {
    std::string contentType = "unknown-type";
	size_t pos = file_path.rfind(".");

    if (pos != std::string::npos && file_path[pos + 1]) {
        std::string ext = file_path.substr(pos + 1);

        switch (ext[0]) {
            case 't':
                if (ext == "txt") contentType = "text/plain";
                break;
            case 'h':
                if (ext == "html") contentType = "text/html";
                break;
            case 'j':
                if (ext == "json") contentType = "application/json";
                else if (ext == "jpg" || ext == "jpeg") contentType = "image/jpeg";
                break;
            case 'x':
                if (ext == "xml") contentType = "application/xml";
                break;
            case 'p':
                if (ext == "pdf") contentType = "application/pdf";
                else if (ext == "png") contentType = "image/png";
                break;
            case 'g':
                if (ext == "gif") contentType = "image/gif";
                break;
            case 'c':
                if (ext == "css") contentType = "text/css";
                break;
            default:
                break;
        }
    }
    return contentType;
}

const std::string Response::getContent(void) const
{
	std::string headers_text;
	std::string response_text;

	response_text += getFirstLine();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		headers_text += it->first + ": " + it->second + "\r\n";
	response_text += headers_text;
	response_text += "\r\n";
	response_text += body;
	return (response_text);
}
/*SETTERS*/

void  Response::setStatusCode(const int status)
{
	this->status_code = status;
}

void  Response::setHeader(const std::string &key, const std::string &value)
{
	headers[key] = value;
}

void  Response::setBody(const std::string &body)
{
	this->body = body;
}

void Response::setFilePath(const std::string &root_main, const std::string &add_root) {
	file_path = (root_main + add_root);
}

void Response::setContentLength(std::string& key) {
	std::cout << "-------------------------" << getMapValue(key, headers) << std::endl;
	std::cout << "key: " << key << std::endl;
}

/**
 * @brief Assigns a number to the request method.
 *
 * @param met_req Receives the request method of type string.
 * @return int with the number assigned after checking it.
 */
int	Response::getRequestMethod(const std::string &request_method) {
	if (request_method.find("GET") != std::string::npos)
		return 1;
	else if (request_method.find("POST") != std::string::npos)
		return 3;
	else if (request_method.find("DELETE") != std::string::npos)
		return 5;
	else
		return 0;
}

bool Response::readFileAndsetBody(const std::string &path) {
	std::ifstream file(path);
	if (isFolder(path))
		return (false);
	if (file.is_open()) {
		char character;
		while (file.get(character)) {
			body += character;
		}
		file.close();
		setHeader("Content-Type", getContentType(path));
		return (true);
	}
	return (false);
}

/**
 * @brief This function determines if the method is allowed
 *
 * @param method_conf receives an integer that determines the methods allowed by the configuration file.
 * @param met_req Receive the method that the request requires.
 * @return std::string Returns the status code after checking the allowed methods.
 */
bool Response::isAllowedMethod(int allowed_methods, const std::string &request_method) {
	int method = getRequestMethod(request_method);
	switch (method) {
		case 1:
			if (allowed_methods == 1 || allowed_methods == 4 || allowed_methods == 6 ||allowed_methods == 9)
				return (true);
			return (false);
		case 3:
			if (allowed_methods == 3 || allowed_methods == 4 || allowed_methods == 8 ||allowed_methods == 9)
				return (true);
			return (false);
		case 5:
			if (allowed_methods == 5 || allowed_methods == 6 || allowed_methods == 8 ||allowed_methods == 9)
				return (true);
			return (false);
		default:
			return (true);
    }
}

void Response::index(const Location &location) {
	std::string const &index_file = location.getValue("index");
	std::string const &auto_index = location.getValue("autoindex");
	std::string index_path = file_path + index_file;

	if (isFolder(index_path) || !readFileAndsetBody(index_path)) {
		if (auto_index == "on")
			autoindex();
		else
			setStatusCode(HTTP_STATUS_NOT_FOUND);
	}
}

void Response::bodyToBodyLength(void){
	body_len = body.size();
}

void Response::addBody(std::string const &addString) {
	body += addString;
}

std::string Response::createHeadHtml(std::string const &title) {
	return (
		"<!DOCTYPE html>\n<html>\n<head>\n" \
		"<title>" + title + "</title>\n" \
		"</head>\n<body>\n");
}

std::string Response::createClousureHtml() {
	return("</body>\n</html>\n");
}

void Response::autoindex() {
    DIR* directory = opendir(file_path.c_str());
    if (isFolder(file_path) && directory) {
		setHeader("Content-Type", "text/html");
        struct dirent* entry;
		addBody(createHeadHtml("autoindex"));
		addBody("<h1>Contenido del directorio:</h1>\n");
		addBody("<ul>\n");
        while ((entry = readdir(directory)) != nullptr) {
            std::string name = entry->d_name;
            if (name != "." && name != "..") {
                if (entry->d_type == DT_REG) {
                      addBody("<li><a href=\"" + name + "\">" + name + "</a></li>\n");
                } else if (entry->d_type == DT_DIR) {
                      addBody("<li><a href=\"" + name + "/\">" + name + "</a></li>\n");
                }
            }
        }
		addBody("</ul>\n");
		createClousureHtml();
        closedir(directory);
    }
}

void Response::createErrorPage() {
	createHeadHtml("Error");
	addBody("<h1 style=\"text-align: center; font-size: 24px;\">" + itoa(status_code) + "</h1>");
	addBody("<h1 style=\"text-align: center; font-size: 24px;\">" + getStatusMessage() + "</h1>");
	createClousureHtml();
}

bool  Response::belowBodySizeLimit(const ServerConfig &server, const HttpRequest &request)
{
	char **ptr = NULL;
	if (server.getValue("client_max_body_size") == "")
		return (true);
	unsigned long max_body_size = strtoul(server.getValue("client_max_body_size").c_str(), ptr, 10);
	if (request.getBody().size() > max_body_size)
		return (false);
	return (true);
}

std::string Response::redirectAddress(const std::string &url, const Location &location)
{
	std::string new_url(url);

	replaceFirstSubstring(new_url, location.getValue("route"), location.getValue("return"));
	return (new_url);
}

void Response::printResponse() const {
	std::cout << "version: " << version << std::endl;
	std::cout << "status_code: " << status_code << std::endl;
	std::cout << "body_len: " << body_len << std::endl;
	std::cout << "body: " << body << std::endl;
}

void Response::runCGI(const std::string& cgi_path, const std::string& cgi_file, const HttpRequest &request) 
{
    int pipe_fd[2];
	char **env = createEnv(request.getParameters());

	try
	{
		if (pipe(pipe_fd) == -1)
			throw PipeException();

		// We should probably use like a local environment just for parameters variables
		pid_t child_pid = fork();

		if (child_pid == -1)
			throw ForkException();
		else if (child_pid == 0)
		{
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[0]);
			close(pipe_fd[1]);

			char* script_args[] = {(char*)cgi_path.c_str(), (char*)cgi_file.c_str(), nullptr};
			execve(cgi_path.c_str(), script_args, env);
			exit(1);
		}
		else
		{
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[1]);

			std::string result;
			char buffer[1024];
			ssize_t bytes_read;

			while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
				result.append(buffer, bytes_read);
			close(pipe_fd[0]);

			int status;
			waitpid(child_pid, &status, 0);

			if (WIFEXITED(status))
			{
				int exit_status = WEXITSTATUS(status);
				if (exit_status != 0)
					throw ChildReturnError();
			}
			else
				throw ChildProcessException();
			body = result;
			setStatusCode(HTTP_STATUS_OK);
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		setStatusCode(HTTP_STATUS_INTERNAL_SERVER_ERROR);
	}
	freeEnv(env);
}
