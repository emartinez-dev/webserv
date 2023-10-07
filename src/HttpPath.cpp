#include "../include/HttpPath.hpp"


/*the extension or empty if it does not have an extension.Methods 
    for orthodox canonical class.*/
HttpPath::HttpPath(std::string path, const Location *location): path_(path), isFile_(true){
    if (isCharValid()) {
        initFile();
        initExtension();
        initVector();
        isFile_ = (getFile() != "" && getExtension() != "") ? true : false;
    } else {
        std::cout << "aqui Hay que controlar el error" << std::endl;
    }
    root_ = concatRoot(location);
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
std::string HttpPath::getRoot() const{
    return (root_);
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
            splitRoute_.push_back("/" + token);
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

std::string HttpPath::concatRoot(const Location *location) {
    bool root = false;

    //splitRoot_.clear();

    for (size_t i = 0; i < splitRoute_.size(); i++) {
        std::string valueToAdd; // Variable auxiliar para almacenar el valor a agregar en esta iteración

        if (location->getValue("route") == "/" && root == false) {
            root = true;
            valueToAdd = location->getValue("root");
        }
        else if (location->getValue("route") != location->getValue("root") && location->getValue("route") == splitRoute_[i]) {
            valueToAdd = location->getValue("root");
        }
        else {
            valueToAdd = splitRoute_[i];
        }
        splitRoot_.push_back(valueToAdd);
        
    }
    std::string root_complet;

    for (size_t i = 0; i < splitRoot_.size(); i++) {
        root_complet += splitRoot_[i];
    }
    std::cout << "devuelve -> " << root_complet << std::endl;
    return root_complet;
}




void HttpPath::printHttpPath() {
    std::cout << std::endl;
    std::cout << "path_: " << path_ << std::endl;
    std::cout << "extension_: " << extension_ << std::endl;
    std::cout << "file_: " << file_ << std::endl;
    std::cout << "isFile_: " << isFile_ << std::endl;
    for (size_t i = 0; i < splitRoute_.size(); ++i) {
        std::cout << "splitRoute: " << splitRoute_[i] << std::endl;
    }
    for (size_t i = 0; i < splitRoot_.size(); ++i) {
        std::cout << "splitRoot: " << splitRoot_[i] << std::endl;
    }
    std::cout << "root_: " << root_ << std::endl;
    std::cout << std::endl;
}