#ifndef HTTPPATH_HPP
#define HTTPPATH_HPP

# include <iostream>
# include <sys/stat.h>
# include <fstream>
# include <sstream>
# include <vector>
# include <string>

class HttpPath
{
    private:
        std::string path_;
        std::string extension_;
        std::string file_;
        bool        isFile_;
        std::vector<std::string> splitRoute_;
        HttpPath();
    public:
        HttpPath(std::string path);
        ~HttpPath();
        HttpPath(HttpPath const &copy);
        HttpPath	&operator=(HttpPath const &copy);

        /*Getters and setters*/
        std::string getPath() const;
        std::string getExtension() const;
        std::string getFile() const;
        std::string getIsFile() const;

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
        void printHttpPath();
};

#endif
