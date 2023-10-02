#ifndef CGI_HPP
#define CGI_HPP



std::string runCGI(const std::string& cgi_path, const std::string& cgi_file, char **envp);

#endif

#ifndef element_HPP
#define element_HPP

# include <iostream>

class element
{
	private:

	public:
		element();
		~element();
		element(element const &copy);
		element	&operator=(element const &copy);
};

#endif
