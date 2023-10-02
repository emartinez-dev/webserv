#ifndef LOCATION_HPP
#define LOCATION_HPP

# include <iostream>
# include <map>

# define GET 1
# define POST 3
# define DELETE 5

# define LAST std::string::npos

class Location
{
	private:
		std::map<std::string, std::string> conf;
		int allow_methods;
	public:
		Location();
		~Location();
		Location(Location const &copy);
		Location	&operator=(Location const &copy);

		void	printConfig(void);
		void	setConf(const std::string &key, const std::string &value);
		const std::map<std::string, std::string>	getConf();
		int		getAllowMethods();
};

#endif
