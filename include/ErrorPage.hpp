#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

# include "webserv.hpp"

class ErrorPage
{
	private:
		int			statusCode;
		std::string	path;
	public:
		ErrorPage();
		~ErrorPage();
		ErrorPage(ErrorPage const &copy);
		ErrorPage	&operator=(ErrorPage const &copy);

		void setPath(std::string value);
		void setCode(std::string value);

		int getCode() const;
		const std::string &getPath() const;
};

#endif
