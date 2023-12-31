#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# include <algorithm>
# include <cstdlib>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <dirent.h>
# include <fcntl.h>
# include <fstream>
# include <iostream>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/fcntl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unordered_map>
# include <unistd.h>
# include <vector>

# include "WebServerExceptions.hpp"

# define LISTEN_MAX_BACKLOG 128
# define POLL_TIMEOUT_MS 30000
# define CLIENT_TIMEOUT_S 30
# define CGI_TIMEOUT 5
# define URL_MAX_LENGTH 2000
# define READ_BUFFER_SIZE 1024
# define DEFAULT_UPLOAD_PATH "/files/"

#endif
