#ifndef LOCATION_HPP
#define LOCATION_HPP

# include "webserv.hpp"

# define GET 1
# define POST 3
# define DELETE 5

# define LAST std::string::npos

class Location
{
	private:
		std::map<std::string, std::string> conf;
		int allowedMethods;
	public:
		Location();
		~Location();
		Location(Location const &copy);
		Location	&operator=(Location const &copy);

		void  printConfig(void) const;
		void  setConf(const std::string &key, const std::string &value);
		std::map<std::string, std::string>	getConf();
		bool  matches(const std::string &path);
		int	  getAllowedMethods() const;
		const std::string getValue(std::string const &key) const;
		bool  hasRedirect(void) const;
};

#endif
