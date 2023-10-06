#include "../include/HttpPath.hpp"
#include "../include/Location.hpp"


/*the extension or empty if it does not have an extension.Methods 
    for orthodox canonical class.*/
HttpPath::HttpPath(std::string path, const std::string& root, const std::string& route): path_(path), isFile_(true){
    std::cout << "root -> " << root << std::endl;
    std::cout << "route -> " << route << std::endl;
    if (isCharValid()) {
        initFile();
        initExtension();
        initVector();
        concatRoot();
        isFile_ = (getFile() != "" && getExtension() != "") ? true : false;
    } else {
        std::cout << "aqui Hay que controlar el error" << std::endl;
    }
    printHttpPath();
}

HttpPath::~HttpPath() {
}

HttpPath::HttpPath(HttpPath const &copy) {
    path_ = copy.path_;
    file_ = copy.file_;
    extension_ = copy.extension_;
    isFile_ = copy.isFile_;
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
std::string HttpPath::getFile() const{
    return (file_);
}
std::string HttpPath::getExtension() const{
    return (extension_);
}

/*SETTERS*/
void HttpPath::setPath(std::string& newPath){
    path_ = newPath;
}
void HttpPath::setFile(std::string& newFile){
    file_ = newFile;
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
    size_t pos = file_.rfind(".");
    if (pos != std::string::npos) {
        extension_ = file_.substr(pos, file_.length());
    } else {
        extension_ = "";
    }
}

void HttpPath::initFile(){
    size_t pos = path_.rfind("/");
    if (pos != std::string::npos) {
        file_ = path_.substr(pos, path_.length());
    } else {
        file_ = "";
    }
}

void HttpPath::initVector() {
    std::istringstream stream(path_);
    std::string token;

    while (std::getline(stream, token, '/')) {
        if (!token.empty()) {
            splitRoute_.push_back(token);
        }
    }
}

/*PARSER PATH*/

bool HttpPath::isCharValid() {
    for (size_t i = 0; i < path_.length(); ++i) {
        if(path_[i] != '.' && path_[i] != '_' && path_[i] != '-' && path_[i] != '/' && !isalnum(path_[i])) {
            return false;
        }
    }
    return true;
}

/* SPLIT ROOT AND CONCATENATE */
void HttpPath::concatRoot() {
    for (size_t i = 0; i < splitRoute_.size(); i++) {
        std::cout << "route: " << splitRoute_[i] << std::endl;
    }
    
}


void HttpPath::printHttpPath() {
    std::cout << std::endl;
    std::cout << "path_: " << path_ << std::endl;
    std::cout << "extension_: " << extension_ << std::endl;
    std::cout << "file_: " << file_ << std::endl;
    std::cout << "isFile_: " << isFile_ << std::endl;
    for (size_t i = 0; i < splitRoute_.size(); ++i) {
        std::cout << "vector: " << splitRoute_[i] << std::endl;
    }
    std::cout << std::endl;
}