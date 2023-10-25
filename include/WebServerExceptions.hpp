#ifndef WEBSERVER_EXCEPTIONS_HPP
#define WEBSERVER_EXCEPTIONS_HPP

# include <exception>

class SocketCreationException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Socket creation failed. Unable to allocate system resources for the new socket.");
	}
};

class SocketOptionsException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Error setting the Socket options.");
	}
};

class SocketPollingError: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Socket polling failed. Unable to poll the given sockets, check that timeout is not too short.");
	}
};

class BindingException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Binding failed. The chosen IP and port combination may be in use or restricted.");
	}
};

class ListeningException: public std::exception {

	virtual const char *what() const throw()
	{
		return ("Listening failed. The server cannot listen on the specified socket for incoming connections.");
	}
};

class ClientAcceptException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Client acceptance failed. The server could not establish a new client connection.");
	}
};

class InvalidIPException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Invalid IP address format. Ensure the IP address adheres to IPv4 standards.");
	}
};

class ConfigurationFileException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Failed to open the given configuration file.");
	}
};

class PipeException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Failed to create a pipe.");
	}
};

class ForkException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Failed to fork a new process.");
	}
};

class ChildReturnError: public std::exception {
	virtual const char *what() const throw()
	{
		return ("Child process returned an error status.");
	}
};

class ChildProcessException: public std::exception {
	virtual const char *what() const throw()
	{
		return ("An exception occurred in the child process.");
	}
};

#endif
