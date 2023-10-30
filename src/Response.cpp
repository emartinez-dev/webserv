#include "Response.hpp"

Response::Response()
{
}

Response::Response(const Request &request, const Config &config):version(request.getVersion()), \
	statusCode(HTTP_STATUS_OK)
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
				if (isAllowedMethod(server_location->getAllowedMethods(), request.getMethod()))
				{
					if (server_location->hasRedirect())
						redirectionHandler(request, *server_location);
					else if (request.getMethod() == "GET")
						getHandler(request, *server_location, *server_config);
					else if (request.getMethod() == "POST")
						postHandler(request, *server_location, *server_config);
					else if (request.getMethod() == "DELETE")
						deleteHandler();
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
	if (statusCode >= 400)
		getErrorPage(*server_config);
	bodyToBodyLength();
	setHeader("Content-Length", itoa(bodyLength));
	setHeader("Connection", "close");
}

void Response::redirectionHandler(const Request &request, const Location &location)
{
	setStatusCode(HTTP_STATUS_MOVED_PERMANENTLY);
	setHeader("Location", redirectAddress(request.getPath(), location));
}

void Response::getHandler(const Request &request, const Location &location, const ServerConfig &config)
{
	if (location.getValue("cgi_ext") != "" && getFileExtension(filePath) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), filePath, request);
	else if (isFolder(filePath) && location.getValue("cgi_ext") != "" && getFileExtension(location.getValue("index")) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), filePath + location.getValue("index"), request);
	else if (isFolder(filePath))
		index(location);
	else if (isFile(filePath) && isAccessible(filePath))
		readFileAndsetBody(filePath);
	else
		setStatusCode(HTTP_STATUS_NOT_FOUND);
}


/* A POST request to a static HTML page is basically a GET request, it does nothing, just retrieving content */
void Response::postHandler(const Request &request, const Location &location, const ServerConfig &config)
{
	if (location.getValue("cgi_ext") != "" && getFileExtension(filePath) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), filePath, request);
	else if (isFolder(filePath) && location.getValue("cgi_ext") != "" && getFileExtension(location.getValue("index")) == location.getValue("cgi_ext"))
		runCGI(config.getValue("cgi_path"), filePath + location.getValue("index"), request);
	else if (location.getValue("allow_uploads") == "on" && request.getHeader("Content-Type").find("multipart/form-data") != std::string::npos)
		uploadFile(location, request);
	else if (request.getHeader("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos)
		getHandler(request, location, config);
	else
		setStatusCode(HTTP_STATUS_METHOD_NOT_ALLOWED);
}

void Response::deleteHandler(void)
{
	if (isFolder(filePath))
		setStatusCode(HTTP_STATUS_NOT_FOUND);
	else if (isFile(filePath) && access(filePath.c_str(), F_OK) == 0)
	{
		if (remove(filePath.c_str()) == 0)
			setStatusCode(HTTP_STATUS_NO_CONTENT);
		else
			setStatusCode(HTTP_STATUS_INTERNAL_SERVER_ERROR);
	}
	else
		setStatusCode(HTTP_STATUS_NOT_FOUND);

}

Response::Response(int error_code):version("HTTP/1.1\r"), statusCode(error_code)
{
	if (statusCode >= 400)
		createErrorPage();
	bodyToBodyLength();
	setHeader("Content-Length", itoa(bodyLength));
	setHeader("Connection", "close");
}

Response::~Response()
{
}

Response::Response(Response const &copy):version(copy.version),statusCode(copy.statusCode),
	body(copy.body), bodyLength(copy.bodyLength),headers(copy.headers)
{
}

Response	&Response::operator=(const Response &copy)
{
	if (this != &copy) {
		version = copy.version;
		statusCode = copy.statusCode;
		body = copy.body;
		bodyLength = copy.bodyLength;
		headers = copy.headers;
	}
	return *this;
}

/*GETTERS*/

std::string Response::getFirstLine () const {
	return (version + " " + itoa(statusCode) + " " + getStatusMessage() + "\r\n");
}

std::string Response::getStatusMessage() const {

    switch (statusCode) {
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
	this->statusCode = status;
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
	filePath = (root_main + add_root);
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
	std::string index_path = filePath + index_file;

	if (isFolder(index_path) || !readFileAndsetBody(index_path)) {
		if (auto_index == "on")
			autoindex();
		else
			setStatusCode(HTTP_STATUS_NOT_FOUND);
	}
}

void Response::bodyToBodyLength(void){
	bodyLength = body.size();
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
    DIR* directory = opendir(filePath.c_str());
    if (isFolder(filePath) && directory) {
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
	addBody("<h1 style=\"text-align: center; font-size: 24px;\">" + itoa(statusCode) + "</h1>");
	addBody("<h1 style=\"text-align: center; font-size: 24px;\">" + getStatusMessage() + "</h1>");
	createClousureHtml();
}

void Response::getErrorPage(const ServerConfig &server)
{
	std::string	error_path = server.getValue("root") + server.getErrorPage(statusCode);

	if (error_path != "" && isFile(error_path) && isAccessible(error_path))
		readFileAndsetBody(error_path);
	else
		createErrorPage();
}

bool  Response::belowBodySizeLimit(const ServerConfig &server, const Request &request)
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
	std::cout << "status_code: " << statusCode << std::endl;
	std::cout << "body_len: " << bodyLength << std::endl;
	std::cout << "body: " << body << std::endl;
}

void Response::runCGI(const std::string& cgi_path, const std::string& cgi_file, const Request &request) 
{
    int pipe_fd[2];
	char **env = createEnv(request.getParameters());

	try
	{
		if (pipe(pipe_fd) == -1)
			throw PipeException();

		pid_t child_pid = fork();

		if (child_pid == -1)
			throw ForkException();
		else if (child_pid == 0)
		{
			rlimit limit;
			limit.rlim_cur = CGI_TIMEOUT;
			limit.rlim_max = CGI_TIMEOUT;

			if (setrlimit(RLIMIT_CPU, &limit) == -1)
				exit(1);

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
			char buffer[READ_BUFFER_SIZE];
			ssize_t bytes_read;

			while ((bytes_read = read(pipe_fd[0], buffer, READ_BUFFER_SIZE)) > 0)
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

std::string getFileName(const std::string &request_body)
{
	size_t head_pos = request_body.find("\r\n\r\n");
	if (head_pos != std::string::npos)
	{
		std::string headers = request_body.substr(0, head_pos);
		size_t filename_pos = headers.find("filename=\"");
		std::string file = headers.substr(filename_pos + 10);
		return file.substr(0, file.find("\""));
	}
	return "";
}

void Response::uploadFile(const Location &location, const Request &request)
{
	std::string upload_path = filePath;

	if (location.getValue("uploads_path") == "")
		upload_path += DEFAULT_UPLOAD_PATH;
	else
		upload_path += location.getValue("uploads_path");
	if (upload_path[upload_path.length() - 1] != '/')
		upload_path += '/';

	std::string filename = getFileName(request.getBody());
	size_t body_separator = request.getBody().find("\r\n\r\n");

	if (filename != "" && body_separator != std::string::npos)
	{
		struct stat st;
		if (stat(upload_path.c_str(), &st) == -1)
			mkdir(upload_path.c_str(), 0700);

		std::string _file_path = upload_path + filename;
		int outfile = open(_file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile > 0)
		{
			const std::string &file_content = request.getBody().substr(body_separator + 4);
			write(outfile, file_content.c_str(), file_content.length());
			close(outfile);
			setStatusCode(HTTP_STATUS_CREATED);
		}
		else
			setStatusCode(HTTP_STATUS_INTERNAL_SERVER_ERROR);
	}
	else
		setStatusCode(HTTP_STATUS_BAD_REQUEST);
}
