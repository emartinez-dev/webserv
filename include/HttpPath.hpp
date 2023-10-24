#ifndef HTTPPATH_HPP
#define HTTPPATH_HPP

# include <iostream>
# include <sys/stat.h>
# include <fstream>
# include <sstream>
# include <vector>
# include <string>
# include "Location.hpp"

# define URL_MAX_LENGTH 2000

class HttpPath
{
    private:
        std::string path_;
        std::string extension_;
        std::string root_;
		bool  valid_path;
        HttpPath();

    public:
        HttpPath(const std::string &path, const Location *location);
        ~HttpPath();
        HttpPath(HttpPath const &copy);
        HttpPath	&operator=(HttpPath const &copy);

        /*Getters and setters*/
        std::string getPath() const;
        std::string getExtension() const;
        std::string getRoot() const;

        void setPath(std::string& newPath);
        void setExtension(std::string& newExtension);
        void initExtension();
        void initVector();

        /* PARSER PATH */
        bool URLisValid();

        /* SPLIT ROOT AND CONCATENATE */
        std::string concatRoot(const Location *location);
        void printHttpPath();
};

#endif
