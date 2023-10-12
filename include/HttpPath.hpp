#ifndef HTTPPATH_HPP
#define HTTPPATH_HPP

# include <iostream>
# include <sys/stat.h>
# include <fstream>
# include <sstream>
# include <vector>
# include <string>
#include "Location.hpp"

#define HTTP_STATUS_OK 200                    // 200 OK
#define HTTP_STATUS_BAD_REQUEST 400          // 400 Bad Request


class HttpPath
{
    private:
        std::string path_;
        std::string extension_;
        std::string file_;
        std::string root_;
        bool        isFile_;
        std::vector<std::string> splitRoute_;
        std::vector<std::string> splitRoot_;
        int status_code;
        HttpPath();

    public:
        HttpPath(std::string path, const Location *location);
        ~HttpPath();
        HttpPath(HttpPath const &copy);
        HttpPath	&operator=(HttpPath const &copy);

        /*Getters and setters*/
        std::string getPath() const;
        std::string getExtension() const;
        std::string getFile() const;
        std::string getRoot() const;
        std::string getIsFile() const;
        int         getStatusCode() const;

        void setPath(std::string& newPath);
        void setFile(std::string& newFile);
        void setIsFile(bool isFile);
        void setExtension(std::string& newExtension);
        void initExtension();
        void initFile();
        void initVector();

        bool isDir();

        /* PARSER PATH */
        bool isCharValid();

        /* SPLIT ROOT AND CONCATENATE */
        std::string concatRoot(const Location *location);
        void printHttpPath();
};

#endif
