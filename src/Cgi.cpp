#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Cgi.hpp"
#include "httpRequest.hpp"

extern char** environ;
/* Should a server be able to access every variable from the environment? I don't think so, 
 * but working at the moment, we will fix it later*/

std::string runCGI(const std::string& cgi_path, const std::string& cgi_file, char **envp, const HttpRequest &request) 
{
    int pipe_fd[2];
	(void) envp;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return "Error en la creación del pipe.";
    }

	const std::map<std::string, std::string> params = request.getParameters();
	for (std::map<std::string, std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
			setenv(it->first.c_str(), it->second.c_str(), 1);
	// We should probably use like a local environment just for parameters variables
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return "Error en la creación del proceso hijo.";
    } 
	else if (child_pid == 0)
	{
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        char* script_args[] = {(char*)cgi_path.c_str(), (char*)cgi_file.c_str(), nullptr};
        execve(cgi_path.c_str(), script_args, environ);
        perror("execve");
        exit(1);
    } 
	else 
	{
        // Proceso padre:
		dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[1]);

        std::string result;
        char buffer[1024];
        ssize_t bytes_read;

        // Leer la salida del script CGI desde la tubería y construir el resultado.
        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            result.append(buffer, bytes_read);
        }
        close(pipe_fd[0]);

        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                std::cerr << "Script CGI devolvió un código de salida no nulo: " << exit_status << std::endl;
            }
        } else {
            std::cerr << "El proceso hijo no terminó con éxito." << std::endl;
        }
        return result;
    }
}
