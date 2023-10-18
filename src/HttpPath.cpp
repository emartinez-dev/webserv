#include "../include/HttpPath.hpp"
#include <_ctype.h>


/*the extension or empty if it does not have an extension.Methods 
    for orthodox canonical class.*/
HttpPath::HttpPath(const std::string &path, const Location *location): path_(path) {
    initExtension();
    root_ = concatRoot(location);
}



HttpPath::~HttpPath() {
}

HttpPath::HttpPath(HttpPath const &copy) {
    path_ = copy.path_;
    extension_ = copy.extension_;
    root_ = copy.root_;
    
}

HttpPath	&HttpPath::operator=(const HttpPath &copy) {
    if (this != &copy) {
        *this = copy;
    }
    return *this;
}

/*GETTERS*/
std::string HttpPath::getPath() const{
    return (path_);
}
std::string HttpPath::getExtension() const{
    return (extension_);
}
std::string HttpPath::getRoot() const{
    return (root_);
}

/*SETTERS*/
void HttpPath::setPath(std::string& newPath){
    path_ = newPath;
}

void HttpPath::setExtension(std::string& newExtension){
    extension_ = newExtension;
}

/**
 * @brief obtain the extension of the route_relative given by the browser.
 * 
 * @return std::string the extension or empty if it does not have an extension.
 */
void HttpPath::initExtension(){
    size_t pos = path_.rfind(".");
    size_t slash_pos = path_.rfind("/");
    if (pos > slash_pos && pos != std::string::npos) {
        extension_ = path_.substr(pos, path_.length());
    } else {
        extension_ = "";
    }
}

bool HttpPath::URLisValid() {
	std::string valid_chars = "-._~:/?#[]@!$&'()*+,;%=";
    for (size_t i = 0; i < path_.length(); ++i) {
		if (valid_chars.find(path_[i]) == std::string::npos && !isalnum(path_[i])) {
			return false;
		}
    }
    return true;
}

/* SPLIT ROOT AND CONCATENATE */

std::string HttpPath::concatRoot(const Location *location)
{
	if (location->getValue("root") != "/")
		return (location->getValue("root") + path_);
	return (path_);
}

void HttpPath::printHttpPath() {
    std::cout << std::endl;
    std::cout << "path_: " << path_ << std::endl;
    std::cout << "extension_: " << extension_ << std::endl;
    std::cout << "root_: " << root_ << std::endl;
    std::cout << std::endl;
}
