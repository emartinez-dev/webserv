#ifndef HTTPPATH_HPP
#define HTTPPATH_HPP

# include "Location.hpp"
# include "webserv.hpp"

class HttpPath
{
    private:
        std::string path;
        std::string extension;
        std::string root;
		bool		validPath;
        HttpPath();
        void initExtension();

    public:
        HttpPath(const std::string &path, const Location *location);
        ~HttpPath();
        HttpPath(HttpPath const &copy);
        HttpPath	&operator=(HttpPath const &copy);

        /*Getters and setters*/
        const std::string &getPath() const;
        const std::string &getExtension() const;
        const std::string &getRoot() const;

        void setPath(const std::string& newPath);
        void setExtension(const std::string& newExtension);

        bool		URLisValid();
        std::string concatRoot(const Location *location);
        void		printHttpPath();
};

#endif
