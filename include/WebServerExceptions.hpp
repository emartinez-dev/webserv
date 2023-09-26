#ifndef WEBSERVER_EXCEPTIONS_HPP
#define WEBSERVER_EXCEPTIONS_HPP

# include <exception>

class SocketCreationException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Socket creation failed. Unable to allocate system resources for the new socket.\n");
	}
};

class SocketOptionsException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Error setting the Socket options.\n");
	}
};

class BindingException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Binding failed. The chosen IP and port combination may be in use or restricted.\n");
	}
};

class ListeningException: public std::exception {

	virtual const char *what() const throw()
	{
		return ("Listening failed. The server cannot listen on the specified socket for incoming connections.\n");
	}
};

class ClientAcceptException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Client acceptance failed. The server could not establish a new client connection.\n");
	}
};

class InvalidIPException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Invalid IP address format. Ensure the IP address adheres to IPv4 standards.\n");
	}
};

#endif
