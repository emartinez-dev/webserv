#include "HttpPath.hpp"
#include "Utils.hpp"

HttpPath::HttpPath()
{}

HttpPath::HttpPath(const std::string &path, const Location *location): path(path) {
    initExtension();
	if (location)
		root = concatRoot(location);
	else
		root = "";
}

HttpPath::~HttpPath() {
}

HttpPath::HttpPath(HttpPath const &copy) {
    path = copy.path;
    extension = copy.extension;
    root = copy.root;
	validPath = copy.validPath;
}

HttpPath	&HttpPath::operator=(const HttpPath &copy) {
    if (this != &copy) {
		path = copy.path;
		extension = copy.extension;
		root = copy.root;
		validPath = copy.validPath;
    }
    return *this;
}

/*GETTERS*/
const std::string &HttpPath::getPath() const{
    return (path);
}
const std::string &HttpPath::getExtension() const{
    return (extension);
}
const std::string &HttpPath::getRoot() const{
    return (root);
}

/*SETTERS*/
void HttpPath::setPath(const std::string& newPath){
    path = newPath;
}

void HttpPath::setExtension(const std::string& newExtension){
    extension = newExtension;
}

/**
 * @brief obtain the extension of the route_relative given by the browser.
 * 
 * @return std::string the extension or empty if it does not have an extension.
 */
void HttpPath::initExtension(){
    size_t pos = path.rfind(".");
    size_t slash_pos = path.rfind("/");
    if (pos > slash_pos && pos != std::string::npos) {
        extension = path.substr(pos, path.length());
    } else {
        extension = "";
    }
}

bool HttpPath::URLisValid() {
	if (path.length() > URL_MAX_LENGTH)
		return (false);
	std::string valid_chars = "-._~:/?#[]@!$&'()*+,;%=";
    for (size_t i = 0; i < path.length(); ++i) {
		if (valid_chars.find(path[i]) == std::string::npos && !isalnum(path[i])) {
			return false;
		}
    }
    return true;
}

std::string HttpPath::concatRoot(const Location *location)
{
    replaceFirstSubstring(path, location->getValue("route"), location->getValue("root"));
	return (path);
}

void HttpPath::printHttpPath() {
    std::cout << std::endl;
    std::cout << "path_: " << path << std::endl;
    std::cout << "extension_: " << extension << std::endl;
    std::cout << "root_: " << root << std::endl;
    std::cout << std::endl;
}
